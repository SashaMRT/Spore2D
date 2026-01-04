/**
 * @file Simulation.cpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Gestionnaire global de la simulation (Moteur).
 * @details Gère les listes d'entités, les mises à jour, les collisions et les naissances.
 * @version 0.2
 * @date 2026-01-04
 *
 * @copyright Copyright (c) 2026
 *
 */

// Bibliothèque utilisées
#include "../../include/Model/Simulation.hpp"
#include "../../include/Model/Entity.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Wolf.hpp"
#include <algorithm> // Pour std::remove_if
#include <cstdlib>   // Pour rand()
#include <ctime>     // Pour time()
#include <vector>    // obligatoire avec SFML 3
#include <cmath>     // Pour les calculs

// -------------------------------------------------------------------------
// POPULATIONS (VARIABLES GLOBALES)
// -------------------------------------------------------------------------

static std::vector<Grass> ecosystem_grass;
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

// Elles gardent le compte tant que le programme tourne
static int g_deadSheepCount = 0;
static int g_deadWolvesCount = 0;

static int g_bornSheepCount = 0;
static int g_bornWolvesCount = 0;

// -------------------------------------------------------------------------
// INITIALISATION
// -------------------------------------------------------------------------

/**
 * @brief Initialise l'écosystème avec des valeurs par défaut.
 */
void initEcosystem() {
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();

    // Génération Herbe
    for (int i = 0; i < 20; i++) { 
        float x = 400 + (rand() % 600);
        float y = 100 + (rand() % 500);
        ecosystem_grass.emplace_back(Grass({x, y}));
    }

    // Génération Moutons
    for (int i = 0; i < 10; i++) { 
        float x = 450 + (rand() % 500);
        float y = 150 + (rand() % 400);
        ecosystem_sheeps.emplace_back(Sheep({x, y}));
    }

    // Génération Loups
    for (int i = 0; i < 2; i++) { 
        float x = 700 + (rand() % 300);
        float y = 200 + (rand() % 300);
        ecosystem_wolves.emplace_back(Wolf({x, y}));
    }
}

// -------------------------------------------------------------------------
// BOUCLE PRINCIPALE (UPDATE)
// -------------------------------------------------------------------------

/**
 * @brief Met à jour toute la logique de l'écosystème (Cerveau du jeu).
 * @details Cette fonction est appelée à chaque image. Elle gère :
 * 1. La repousse aléatoire de l'herbe.
 * 2. Le comportement des Loups (Chasse, Manger, Reproduction).
 * 3. Le comportement des Moutons (Manger, Fuite, Reproduction, IA de déplacement).
 * 4. La naissance des bébés et la suppression des morts (Nettoyage).
 * * @param dt Delta Time (Temps écoulé depuis la dernière image) pour fluidifier les mouvements.
 */
void ecosystemUpdate(float dt) {
    
    // -----------------------------------------------------------
    // 1. INITIALISATION DE L'ALÉATOIRE
    // -----------------------------------------------------------
    // "static" permet de garder la variable en mémoire entre les appels.
    // On ne veut lancer srand() qu'une seule fois au tout début.
    static bool randInit = false;
    if (!randInit) {
        srand(static_cast<unsigned>(time(NULL)));
        initEcosystem(); // Si c'est la première fois, on crée le monde
        randInit = true;
    }


    // -----------------------------------------------------------
    // 2. GESTION DE L'HERBE (Repousse)
    // -----------------------------------------------------------
    // À chaque frame, il y a 5% de chance qu'une nouvelle herbe pousse.
    if (rand() % 100 < 5) { 
        // Position aléatoire contrainte (pour ne pas apparaître hors champ)
        float x = 400 + (rand() % 600);
        float y = 100 + (rand() % 500);
        ecosystem_grass.emplace_back(Grass({x, y}));
    }
    // On met à jour l'animation ou l'état de l'herbe existante
    for (auto& g : ecosystem_grass) g.update(dt);


    // -----------------------------------------------------------
    // 3. GESTION DES LOUPS (Prédateurs)
    // -----------------------------------------------------------
    std::vector<Wolf> babyWolves; // Liste temporaire pour stocker les bébés nés cette frame

    for (size_t i = 0; i < ecosystem_wolves.size(); ++i) {
        Wolf& w1 = ecosystem_wolves[i];
        
        // Si le loup est mort, on l'ignore (il sera supprimé à la fin)
        if (!w1.alive) continue;

        // Comportement de base
        w1.update(dt);             // Diminue la faim / cooldown reproduction
        w1.hunt(ecosystem_sheeps); // Cherche un mouton
        w1.eat(ecosystem_sheeps);  // Mange si assez proche

        // --- REPRODUCTION DES LOUPS ---
        if (w1.canReproduce()) {
            // On cherche un partenaire (double boucle)
            for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
                Wolf& w2 = ecosystem_wolves[j];
                
                // Conditions : Partenaire vivant + prêt + proche (< 30px)
                if (w2.alive && w2.canReproduce() && w1.dist(w2.pos) < 30.f) {
                    // Naissance !
                    babyWolves.emplace_back(Wolf(w1.pos)); 
                    
                    // Reset des compteurs
                    w1.resetReproduction();
                    w2.resetReproduction();
                    g_bornWolvesCount++; // Statistique pour le HUD
                    break; // Un seul bébé à la fois
                }
            }
        }
    }


    // -----------------------------------------------------------
    // 4. GESTION DES MOUTONS (Proies)
    // -----------------------------------------------------------
    std::vector<Sheep> babySheeps; // Liste temporaire

    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        Sheep& s1 = ecosystem_sheeps[i];
        if (!s1.alive) continue;

        s1.update(dt);          // Vieillissement / faim
        s1.eat(ecosystem_grass);// Manger l'herbe

        // --- INTELLIGENCE ARTIFICIELLE (IA) DE MOUVEMENT ---
        sf::Vector2f moveDir{0.f, 0.f};
        
        // A. RECHERCHE DE NOURRITURE (Priorité 2)
        Grass* bestGrass = nullptr;
        float bestDist = 999.f;
        
        // On cherche l'herbe la plus proche dans un rayon de 200px
        for (auto& g : ecosystem_grass) {
            if (g.alive) {
                float d = s1.dist(g.pos);
                if (d < bestDist && d < 200.f) { bestDist = d; bestGrass = &g; }
            }
        }
        // Si on a trouvé de l'herbe, on se dirige vers elle
        if (bestGrass) {
            sf::Vector2f dir = bestGrass->pos - s1.pos;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y); // Normalisation du vecteur
            moveDir = dir / len;
        }
        
        // B. FUITE DEVANT LE LOUP (Priorité 1 - Absolue)
        for (const auto& w : ecosystem_wolves) {
            // Si un loup est vivant et trop proche (< 100px)
            if (w.alive && s1.dist(w.pos) < 100.f) {
                sf::Vector2f flee = s1.pos - w.pos; // Vecteur opposé au loup
                float len = std::sqrt(flee.x*flee.x + flee.y*flee.y);
                moveDir = (flee / len) * 1.8f; // On fuit 1.8x plus vite que la marche normale
                break; // On ne fuit que le premier loup trouvé (simplification)
            }
        }
        
        // C. APPLICATION DU MOUVEMENT
        s1.pos += moveDir * s1.speed * dt;

        // D. LIMITES DU MONDE (Clamp)
        // Empêche le mouton de sortir de la zone définie
        s1.pos.x = std::clamp(s1.pos.x, 400.f, 1200.f);
        s1.pos.y = std::clamp(s1.pos.y, 100.f, 800.f);


        // --- REPRODUCTION DES MOUTONS ---
        if (s1.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
                Sheep& s2 = ecosystem_sheeps[j];
                
                if (s2.alive && s2.canReproduce() && s1.dist(s2.pos) < 30.f) {
                    babySheeps.emplace_back(Sheep(s1.pos)); // Naissance
                    s1.resetReproduction();
                    s2.resetReproduction();
                    g_bornSheepCount++; // Statistique
                    break;
                }
            }
        }
    }

    // -----------------------------------------------------------
    // 5. NETTOYAGE ET NAISSANCES (Fin de cycle)
    // -----------------------------------------------------------
    
    // On ajoute les bébés nés durant cette image aux listes principales
    for (auto& b : babySheeps) ecosystem_sheeps.push_back(b);
    for (auto& b : babyWolves) ecosystem_wolves.push_back(b);

    // SUPPRESSION DES MORTS (et mise à jour des stats de décès)
    
    // Herbe : On enlève juste l'herbe mangée
    ecosystem_grass.erase(std::remove_if(ecosystem_grass.begin(), ecosystem_grass.end(), [](auto& g){return !g.alive;}), ecosystem_grass.end());
    
    // Moutons : On compte les morts avant de supprimer
    ecosystem_sheeps.erase(std::remove_if(ecosystem_sheeps.begin(), ecosystem_sheeps.end(), 
        [](auto& s) {
            if (!s.alive) { g_deadSheepCount++; return true; }
            return false;
        }
    ), ecosystem_sheeps.end());

    // Loups : On compte les morts avant de supprimer
    ecosystem_wolves.erase(std::remove_if(ecosystem_wolves.begin(), ecosystem_wolves.end(), 
        [](auto& w) {
            if (!w.alive) { g_deadWolvesCount++; return true; }
            return false;
        }
    ), ecosystem_wolves.end());

    // SÉCURITÉ ANTI-EXTINCTION
    // Si tout le monde est mort, on relance la simulation pour ne pas regarder un écran vide.
    if (ecosystem_sheeps.empty() && ecosystem_wolves.empty()) initEcosystem();
}

// -------------------------------------------------------------------------
// OUTILS & STATISTIQUES
// -------------------------------------------------------------------------

/**
 * @brief Récupère les statistiques complètes de l'écosystème.
 * @details Cette fonction rassemble les données actuelles (populations vivantes)
 * et les données historiques (morts et naissances cumulés) pour les envoyer au HUD.
 * * @return EcosystemStats Une structure contenant les 7 compteurs du jeu.
 */
EcosystemStats getEcosystemStats() {
    // On retourne une structure initialisée directement avec les valeurs (Liste d'initialisation).
    return {
        // --- 1. POPULATIONS ACTUELLES (Vivants) ---
        // On utilise static_cast<int> car .size() renvoie un "unsigned long" (size_t),
        // mais notre structure attend des "int" classiques.
        static_cast<int>(ecosystem_grass.size()),  // Nombre d'herbes
        static_cast<int>(ecosystem_sheeps.size()), // Nombre de moutons vivants
        static_cast<int>(ecosystem_wolves.size()), // Nombre de loups vivants

        // --- 2. STATISTIQUES CUMULÉES (Historique) ---
        // Ces variables globales comptent depuis le lancement du jeu.
        g_deadSheepCount,  // Total des moutons morts
        g_deadWolvesCount, // Total des loups morts
        g_bornSheepCount,  // Total des naissances de moutons
        g_bornWolvesCount  // Total des naissances de loups
    };
}

// -------------------------------------------------------------------------
// AFFICHAGE
// -------------------------------------------------------------------------

/**
 * @brief Dessine tout le monde.
 */
void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) if (g.alive) g.draw(window);
    for (auto& s : ecosystem_sheeps) if (s.alive) s.draw(window);
    for (auto& w : ecosystem_wolves) if (w.alive) w.draw(window);
}