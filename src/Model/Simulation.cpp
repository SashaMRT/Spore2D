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
 * @brief Mise à jour principale de la logique du jeu.
 * @param dt Delta Time.
 */
void ecosystemUpdate(float dt) {
    // Initialisation unique de l'aléatoire
    static bool randInit = false;
    if (!randInit) {
        srand(static_cast<unsigned>(time(NULL)));
        initEcosystem();
        randInit = true;
    }

    // --- GESTION HERBE (Repousse) ---
    if (rand() % 100 < 5) { // 5% de chance
        float x = 400 + (rand() % 600);
        float y = 100 + (rand() % 500);
        ecosystem_grass.emplace_back(Grass({x, y}));
    }
    for (auto& g : ecosystem_grass) g.update(dt);


    // --- GESTION LOUPS ---
    std::vector<Wolf> babyWolves; // Stockage temporaire des bébés

    for (size_t i = 0; i < ecosystem_wolves.size(); ++i) {
        Wolf& w1 = ecosystem_wolves[i];
        if (!w1.alive) continue;

        w1.update(dt);
        w1.hunt(ecosystem_sheeps);
        w1.eat(ecosystem_sheeps);

        // Reproduction Loups
        if (w1.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
                Wolf& w2 = ecosystem_wolves[j];
                
                // Si l'autre loup est prêt et proche (< 30px)
                if (w2.alive && w2.canReproduce() && w1.dist(w2.pos) < 30.f) {
                    babyWolves.emplace_back(Wolf(w1.pos)); // Naissance
                    w1.resetReproduction();
                    w2.resetReproduction();
                    break; 
                }
            }
        }
    }


    // --- GESTION MOUTONS ---
    std::vector<Sheep> babySheeps; // Stockage temporaire

    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        Sheep& s1 = ecosystem_sheeps[i];
        if (!s1.alive) continue;

        s1.update(dt);
        s1.eat(ecosystem_grass);

        // -- Mouvement IA (Nourriture vs Fuite) --
        sf::Vector2f moveDir{0.f, 0.f};
        
        // Recherche Herbe
        Grass* bestGrass = nullptr;
        float bestDist = 999.f;
        for (auto& g : ecosystem_grass) {
            if (g.alive) {
                float d = s1.dist(g.pos);
                if (d < bestDist && d < 200.f) { bestDist = d; bestGrass = &g; }
            }
        }
        if (bestGrass) {
            sf::Vector2f dir = bestGrass->pos - s1.pos;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            moveDir = dir / len;
        }
        
        // Fuite Loup
        for (const auto& w : ecosystem_wolves) {
            if (w.alive && s1.dist(w.pos) < 100.f) {
                sf::Vector2f flee = s1.pos - w.pos;
                float len = std::sqrt(flee.x*flee.x + flee.y*flee.y);
                moveDir = (flee / len) * 1.8f; // Fuite rapide
                break;
            }
        }
        
        // Application
        s1.pos += moveDir * s1.speed * dt;
        s1.pos.x = std::clamp(s1.pos.x, 400.f, 1200.f);
        s1.pos.y = std::clamp(s1.pos.y, 100.f, 800.f);


        // Reproduction Moutons
        if (s1.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
                Sheep& s2 = ecosystem_sheeps[j];
                
                if (s2.alive && s2.canReproduce() && s1.dist(s2.pos) < 30.f) {
                    babySheeps.emplace_back(Sheep(s1.pos)); // Naissance
                    s1.resetReproduction();
                    s2.resetReproduction();
                    break;
                }
            }
        }
    }

    // --- NETTOYAGE ET NAISSANCES ---
    
    // Ajout des nouveaux nés
    for (auto& b : babySheeps) ecosystem_sheeps.push_back(b);
    for (auto& b : babyWolves) ecosystem_wolves.push_back(b);

    // Suppression des morts
    ecosystem_grass.erase(std::remove_if(ecosystem_grass.begin(), ecosystem_grass.end(), [](auto& g){return !g.alive;}), ecosystem_grass.end());
    ecosystem_sheeps.erase(std::remove_if(ecosystem_sheeps.begin(), ecosystem_sheeps.end(), [](auto& s){return !s.alive;}), ecosystem_sheeps.end());
    ecosystem_wolves.erase(std::remove_if(ecosystem_wolves.begin(), ecosystem_wolves.end(), [](auto& w){return !w.alive;}), ecosystem_wolves.end());

    // Sécurité anti-extinction
    if (ecosystem_sheeps.empty() && ecosystem_wolves.empty()) initEcosystem();
}

// -------------------------------------------------------------------------
// OUTILS & STATISTIQUES
// -------------------------------------------------------------------------

/**
 * @brief Récupère les stats pour le HUD.
 */
EcosystemStats getEcosystemStats() {
    return {
        static_cast<int>(ecosystem_grass.size()),
        static_cast<int>(ecosystem_sheeps.size()),
        static_cast<int>(ecosystem_wolves.size())
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