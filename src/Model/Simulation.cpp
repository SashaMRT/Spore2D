/**
 * @file Simulation.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Logique métier : Cerveau de la simulation.
 * @details Ce fichier gère l'ensemble des règles du jeu : 
 * naissances, morts, déplacements, collisions, intelligence artificielle et limites du monde.
 * @version 0.5
 * @date 2026-01-05
 */

#include "../../include/Model/Simulation.hpp"
#include "../../include/Model/Entity.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Wolf.hpp"

// Bibliothèques standard pour les algorithmes et les maths
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list> 
#include <map>  
#include <cmath>

// -------------------------------------------------------------------------
// VARIABLES GLOBALES (Module-Level)
// -------------------------------------------------------------------------

// Limites de la zone de jeu (Mises à jour par le Renderer)
static float g_xMin = 0.f, g_xMax = 1000.f;
static float g_yMin = 0.f, g_yMax = 1000.f;

// Temps total écoulé depuis le début
static float g_simulationTime = 0.f;

// Paramètres de configuration (Tweakables pour équilibrer le jeu)
static std::map<std::string, float> g_config = {
    {"grass_spawn_rate", 5.0f},   // % de chance qu'une herbe pousse par frame
    {"sheep_vision_food", 250.0f},// Distance à laquelle un mouton voit l'herbe
    {"sheep_panic_dist", 120.0f}, // Distance à laquelle un mouton panique devant un loup
    {"wolf_vision", 400.0f}       // Distance de chasse du loup
};

// Listes des entités
static std::list<Grass> ecosystem_grass;      // Liste chaînée pour l'herbe (suppression rapide)
static std::vector<Sheep> ecosystem_sheeps;   // Vecteur pour les moutons
static std::vector<Wolf> ecosystem_wolves;    // Vecteur pour les loups

// Compteurs statistiques cumulés
static int g_deadSheepCount = 0;
static int g_deadWolvesCount = 0;
static int g_bornSheepCount = 0;
static int g_bornWolvesCount = 0;

// -------------------------------------------------------------------------
// FONCTIONS UTILITAIRES
// -------------------------------------------------------------------------

/**
 * @brief Met à jour les coordonnées de la zone de jeu.
 * @details Appelée par le Renderer quand la fenêtre change de taille.
 */
void setWorldBounds(float xMin, float xMax, float yMin, float yMax) {
    g_xMin = xMin; g_xMax = xMax;
    g_yMin = yMin; g_yMax = yMax;
}

/**
 * @brief Gestion intelligente des limites du monde.
 * @details Gère deux cas :
 * 1. L'entité essaie de sortir (Déplacement) -> On la bloque (Clamp).
 * 2. Le mur se déplace sur l'entité (Redimensionnement fenêtre) -> On la tue.
 * @param e L'entité à vérifier.
 */
void checkWorldBounds(Entity& e) {
    float padding = 12.0f;          // Marge de sécurité (le rayon de l'animal)
    float killThreshold = 50.0f;    // Tolérance avant de tuer (Si le mur dépasse de 50px)

    // Vérification Limite GAUCHE
    if (e.pos.x < g_xMin + padding) {
        if (e.pos.x < g_xMin - killThreshold) e.alive = false; // Le mur a écrasé l'animal -> Mort
        else e.pos.x = g_xMin + padding;                       // L'animal touche le mur -> Bloqué
    }
    // Vérification Limite DROITE
    else if (e.pos.x > g_xMax - padding) {
        if (e.pos.x > g_xMax + killThreshold) e.alive = false;
        else e.pos.x = g_xMax - padding;
    }

    // Vérification Limite HAUT
    if (e.pos.y < g_yMin + padding) {
        if (e.pos.y < g_yMin - killThreshold) e.alive = false;
        else e.pos.y = g_yMin + padding;
    }
    // Vérification Limite BAS
    else if (e.pos.y > g_yMax - padding) {
        if (e.pos.y > g_yMax + killThreshold) e.alive = false;
        else e.pos.y = g_yMax - padding;
    }
}

/**
 * @brief Résout les collisions physiques entre les animaux (Répulsion).
 * @details Empêche les animaux de se superposer en les poussant doucement s'ils sont trop proches.
 */
void solveCollisions() {
    // Répulsion Mouton <-> Mouton
    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
            Sheep& s1 = ecosystem_sheeps[i];
            Sheep& s2 = ecosystem_sheeps[j];
            if (!s1.alive || !s2.alive) continue;
            
            float d = s1.dist(s2.pos);
            // Si trop proches (< 14px) mais pas superposés (0.1px)
            if (d < 14.f && d > 0.1f) {
                sf::Vector2f push = (s1.pos - s2.pos) / d; // Direction de la poussée
                float overlap = 14.f - d;                  // Force de la poussée
                s1.pos += push * (overlap * 0.5f);
                s2.pos -= push * (overlap * 0.5f);
            }
        }
    }
    // Répulsion Loup <-> Loup (Rayon un peu plus gros : 18px)
    for (size_t i = 0; i < ecosystem_wolves.size(); ++i) {
        for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
            Wolf& w1 = ecosystem_wolves[i];
            Wolf& w2 = ecosystem_wolves[j];
            if (!w1.alive || !w2.alive) continue;
            
            float d = w1.dist(w2.pos);
            if (d < 18.f && d > 0.1f) {
                sf::Vector2f push = (w1.pos - w2.pos) / d;
                float overlap = 18.f - d;
                w1.pos += push * (overlap * 0.5f);
                w2.pos -= push * (overlap * 0.5f);
            }
        }
    }
}

/**
 * @brief Calcule une direction de déplacement aléatoire mais fluide (Errance).
 * @param index Un identifiant unique pour désynchroniser les mouvements des animaux.
 * @param time Le temps actuel.
 * @return sf::Vector2f Un vecteur directionnel.
 */
sf::Vector2f getWanderDir(int index, float time) {
    // Utilise le sinus pour créer des courbes fluides plutôt que des changements brusques
    float angle = std::sin(time * 0.5f + index) * 3.14159f * 2.0f;
    return sf::Vector2f(std::cos(angle), std::sin(angle));
}

// -------------------------------------------------------------------------
// LOGIQUE PRINCIPALE
// -------------------------------------------------------------------------

/**
 * @brief Réinitialise complètement la simulation (Reset).
 */
void initEcosystem() {
    // Nettoyage
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();
    
    // Reset des compteurs
    g_simulationTime = 0.f;
    g_deadSheepCount = 0; g_deadWolvesCount = 0;
    g_bornSheepCount = 0; g_bornWolvesCount = 0;

    // Génération Herbe (50 unités)
    for (int i = 0; i < 50; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_grass.emplace_back(Grass({x, y}));
    }
    // Génération Moutons (20 unités)
    for (int i = 0; i < 20; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_sheeps.emplace_back(Sheep({x, y}));
    }
    // Génération Loups (3 unités)
    for (int i = 0; i < 3; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_wolves.emplace_back(Wolf({x, y}));
    }
}

/**
 * @brief Met à jour toute la logique de l'écosystème à chaque frame.
 * @param dt Delta Time (Temps écoulé).
 */
void ecosystemUpdate(float dt) {
    // Initialisation unique au premier appel
    static bool randInit = false;
    if (!randInit) { srand(time(NULL)); initEcosystem(); randInit = true; }

    g_simulationTime += dt;

    // -----------------------------------------------------------
    // GESTION DE L'HERBE
    // -----------------------------------------------------------
    if ((rand() % 100) < g_config["grass_spawn_rate"]) { 
        // Apparition aléatoire dans les limites
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_grass.push_back(Grass({x, y}));
    }
    
    for (auto& g : ecosystem_grass) {
        g.update(dt);
        // HERBE : Si elle se retrouve hors des limites (redimensionnement), elle meurt instantanément
        if (g.pos.x < g_xMin || g.pos.x > g_xMax || g.pos.y < g_yMin || g.pos.y > g_yMax) {
            g.alive = false;
        }
    }

    // -----------------------------------------------------------
    // GESTION DES LOUPS
    // -----------------------------------------------------------
    std::vector<Wolf> babyWolves; // Liste temporaire pour les naissances

    for (size_t i = 0; i < ecosystem_wolves.size(); ++i) {
        Wolf& w = ecosystem_wolves[i];
        if (!w.alive) continue;

        w.update(dt);
        checkWorldBounds(w); // Tue ou Bloque selon la situation

        // --- IA LOUP ---
        const Sheep* target = nullptr;
        float minDist = g_config["wolf_vision"];

        // Recherche du mouton le plus proche
        for (const auto& s : ecosystem_sheeps) {
            if (s.alive) {
                float d = w.dist(s.pos);
                if (d < minDist) { minDist = d; target = &s; }
            }
        }

        // Action : Chasser ou Errer
        if (target) {
            // Se dirige vers la cible
            sf::Vector2f dir = target->pos - w.pos;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            w.pos += (dir / len) * w.speed * dt;
        } else {
            // Errance aléatoire (plus lent)
            sf::Vector2f wander = getWanderDir(i, g_simulationTime * 0.5f);
            w.pos += wander * (w.speed * 0.6f) * dt;
        }

        // Manger
        w.eat(ecosystem_sheeps);

        // Reproduction
        if (w.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
                if (ecosystem_wolves[j].alive && ecosystem_wolves[j].canReproduce() && w.dist(ecosystem_wolves[j].pos) < 30.f) {
                    babyWolves.emplace_back(Wolf(w.pos)); 
                    w.resetReproduction(); 
                    ecosystem_wolves[j].resetReproduction(); 
                    g_bornWolvesCount++; 
                    break;
                }
            }
        }
    }

    // -----------------------------------------------------------
    // GESTION DES MOUTONS
    // -----------------------------------------------------------
    std::vector<Sheep> babySheeps;

    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        Sheep& s = ecosystem_sheeps[i];
        if (!s.alive) continue;

        s.update(dt);
        
        // Manger l'herbe (contact très proche < 15px)
        for (auto& g : ecosystem_grass) {
            if (g.alive && s.dist(g.pos) < 15.f) { g.alive = false; s.gainEnergy(20.f); }
        }

        // --- IA MOUTON ---
        sf::Vector2f moveDir{0.f, 0.f};
        const Wolf* danger = nullptr;
        float closestDanger = g_config["sheep_panic_dist"];

        // Détection du danger (Priorité absolue)
        for (const auto& w : ecosystem_wolves) {
            if (w.alive) {
                float d = s.dist(w.pos);
                if (d < closestDanger) { closestDanger = d; danger = &w; }
            }
        }

        if (danger) {
            // FUITE : Vecteur opposé au loup
            sf::Vector2f flee = s.pos - danger->pos;
            float len = std::sqrt(flee.x*flee.x + flee.y*flee.y);
            if (len > 0) moveDir = (flee / len) * 1.5f; // Fuite rapide (x1.5)
        } 
        else {
            // Recherche de nourriture
            float bestDist = g_config["sheep_vision_food"];
            sf::Vector2f targetGrass = s.pos;
            bool foundGrass = false;
            
            for (const auto& g : ecosystem_grass) {
                if (g.alive) {
                    float d = s.dist(g.pos);
                    if (d < bestDist) { bestDist = d; targetGrass = g.pos; foundGrass = true; }
                }
            }
            if (foundGrass) {
                sf::Vector2f dir = targetGrass - s.pos;
                float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
                if(len > 0) moveDir = dir / len;
            } else {
                // Errance si rien à faire
                moveDir = getWanderDir(i + 100, g_simulationTime * 0.3f) * 0.5f;
            }
        }

        s.pos += moveDir * s.speed * dt;
        checkWorldBounds(s); // Tue ou Bloque

        // Reproduction
        if (s.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
                if (ecosystem_sheeps[j].alive && ecosystem_sheeps[j].canReproduce() && s.dist(ecosystem_sheeps[j].pos) < 30.f) {
                    babySheeps.emplace_back(Sheep(s.pos)); 
                    s.resetReproduction(); 
                    ecosystem_sheeps[j].resetReproduction(); 
                    g_bornSheepCount++; 
                    break;
                }
            }
        }
    }

    // -----------------------------------------------------------
    // NETTOYAGE ET PHYSIQUE
    // -----------------------------------------------------------

    solveCollisions(); // Applique la physique de répulsion

    // Ajout des bébés aux listes principales
    for (auto& b : babySheeps) ecosystem_sheeps.push_back(b);
    for (auto& b : babyWolves) ecosystem_wolves.push_back(b);

    // Suppression de l'herbe morte
    ecosystem_grass.remove_if([](const Grass& g){ return !g.alive; });
    
    // Suppression des moutons morts (+ update stats)
    ecosystem_sheeps.erase(std::remove_if(ecosystem_sheeps.begin(), ecosystem_sheeps.end(), 
        [](auto& s) { 
            if (!s.alive) g_deadSheepCount++; 
            return !s.alive; 
        }
    ), ecosystem_sheeps.end());

    // Suppression des loups morts (+ update stats)
    ecosystem_wolves.erase(std::remove_if(ecosystem_wolves.begin(), ecosystem_wolves.end(), 
        [](auto& w) { 
            if (!w.alive) g_deadWolvesCount++; 
            return !w.alive; 
        }
    ), ecosystem_wolves.end());
}

/**
 * @brief Récupère les statistiques pour le HUD.
 * @return EcosystemStats Structure contenant tous les chiffres du jeu.
 */
EcosystemStats getEcosystemStats() {
    return { 
        (int)ecosystem_grass.size(), 
        (int)ecosystem_sheeps.size(), 
        (int)ecosystem_wolves.size(), 
        g_deadSheepCount, 
        g_deadWolvesCount, 
        g_bornSheepCount, 
        g_bornWolvesCount, 
        g_simulationTime 
    };
}

/**
 * @brief Dessine toutes les entités vivantes.
 * @param window La fenêtre SFML.
 */
void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) if (g.alive) g.draw(window);
    for (auto& s : ecosystem_sheeps) if (s.alive) s.draw(window);
    for (auto& w : ecosystem_wolves) if (w.alive) w.draw(window);
}