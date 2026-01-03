/**
 * @file Simulation.cpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Gestion centrale de l'écosystème : herbe, moutons, loups (spawn, IA, rendu)
 * @details 
 *  - Orchestre la simulation complète du monde vivant
 *  - Gère les populations globales (conteneurs statiques)
 *  - Implémente l'IA des moutons : recherche d'herbe + fuite des loups
 *  - Contrôle les interactions prédateurs/proies et le respawn automatique
 * @version 0.1
 * @date 2026-01-03
 * 
 * @copyright Copyright (c) 2026
 */


// Inclut toutes les entités de l'écosystème et les utilitaires nécessaires
#include "../../include/Model/Simulation.hpp"
#include "../../include/Model/Entity.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Wolf.hpp"
#include <algorithm>     // Pour std::remove_if, emplace_back
#include <cstdlib>       // Pour rand()
#include <ctime>         // Pour time(NULL)

// ===== CONTENEURS GLOBAUX DE L'ÉCOSYSTÈME =====
// Listes statiques contenant TOUTES les entités vivantes de la simulation.
// Accessibles uniquement depuis ce fichier (encapsulation).
static std::vector<Grass> ecosystem_grass;
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

// ===== INITIALISATION =====
void initEcosystem() {
    // Vide complètement les populations existantes
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();

    // Spawn 8 touffes d'herbe dans la zone 400-1000x, 100-600y
    for (int i = 0; i < 8; i++) {
        float x = 400 + (rand() % 600);  // X: 400 → 1000
        float y = 100 + (rand() % 500);  // Y: 100 → 600
        ecosystem_grass.emplace_back(Grass({x, y}));
    }

    // Spawn 5 moutons dans la zone 450-950x, 150-550y
    for (int i = 0; i < 5; i++) {
        float x = 450 + (rand() % 500);
        float y = 150 + (rand() % 400);
        ecosystem_sheeps.emplace_back(Sheep({x, y}));
    }

    // Spawn 3 loups dans la zone 700-1000x, 200-500y
    for (int i = 0; i < 3; i++) {
        float x = 700 + (rand() % 300);
        float y = 200 + (rand() % 300);
        ecosystem_wolves.emplace_back(Wolf({x, y}));
    }
}

// ===== MISE À JOUR PRINCIPALE =====
void ecosystemUpdate(float dt) {
    // Initialisation du générateur aléatoire (une seule fois)
    static bool randInit = false;
    if (!randInit) {
        srand(static_cast<unsigned>(time(NULL)));
        initEcosystem();  // Premier spawn
        randInit = true;
    }

    // Mise à jour herbe (régénération)
    for (auto& g : ecosystem_grass) g.update(dt);

    // Mise à jour loups (mouvement + chasse)
    for (auto& w : ecosystem_wolves) { 
        w.update(dt); 
        w.hunt(ecosystem_sheeps);    // IA de poursuite
        w.eat(ecosystem_sheeps);     // Prédation
    }

    // IA MOUTONS (le cœur de la simulation !)
    for (auto& s : ecosystem_sheeps) {
        if (!s.alive) continue;  // Mouton mort : ignoré
        
        s.update(dt);  // Gestion énergie interne
        sf::Vector2f moveDir{0.f, 0.f};  // Direction de déplacement

        // Recherche de l'herbe la plus proche (max 200px)
        Grass* bestGrass = nullptr;
        float bestDist = 999.f;
        for (auto& g : ecosystem_grass)
            if (g.alive) {
                float d = s.dist(g.pos);
                if (d < bestDist && d < 200.f) { 
                    bestDist = d; 
                    bestGrass = &g; 
                }
            }

        // Attraction vers l'herbe proche (< 550px)
        if (bestGrass && bestDist < 550.f) {
            sf::Vector2f dir = bestGrass->pos - s.pos;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            moveDir = dir / len;  // Direction normalisée
        }

        // Fuite panique si loup à < 100px
        for (const auto& w : ecosystem_wolves)
            if (w.alive && s.dist(w.pos) < 100.f) {
                sf::Vector2f flee = s.pos - w.pos;
                float len = std::sqrt(flee.x * flee.x + flee.y * flee.y);
                moveDir = (flee / len) * 1.5f;  // Fuite prioritaire x1.5
                break;
            }

        // Déplacement + confinement dans la zone de jeu
        s.pos += moveDir * s.speed * dt;
        s.pos.x = std::clamp(s.pos.x, 400.f, 1200.f);  // Limite horizontale
        s.pos.y = std::clamp(s.pos.y, 100.f, 800.f);   // Limite verticale
        s.eat(ecosystem_grass);  // Manger l'herbe proche
    }

    // NETTOYAGE : supprime les entités mortes
    ecosystem_grass.erase(std::remove_if(ecosystem_grass.begin(), ecosystem_grass.end(), 
        [](auto& g){return !g.alive;}), ecosystem_grass.end());
    ecosystem_sheeps.erase(std::remove_if(ecosystem_sheeps.begin(), ecosystem_sheeps.end(), 
        [](auto& s){return !s.alive;}), ecosystem_sheeps.end());
    ecosystem_wolves.erase(std::remove_if(ecosystem_wolves.begin(), ecosystem_wolves.end(), 
        [](auto& w){return !w.alive;}), ecosystem_wolves.end());

    // RESPAWN automatique si extinction des moutons
    if (ecosystem_sheeps.empty()) initEcosystem();
}

// ===== RENDU GRAPHIQUE =====
void ecosystemDraw(sf::RenderWindow& window) {
    // Dessine TOUTES les herbes (vivantes ou mortes = transparentes)
    for (auto& g : ecosystem_grass) { 
        g.shape.setPosition(g.pos); 
        window.draw(g.shape); 
    }
    
    // Dessine SEULEMENT les moutons vivants
    for (auto& s : ecosystem_sheeps) 
        if (s.alive) { 
            s.shape.setPosition(s.pos); 
            window.draw(s.shape); 
        }
    
    // Dessine SEULEMENT les loups vivants
    for (auto& w : ecosystem_wolves) 
        if (w.alive) { 
            w.shape.setPosition(w.pos); 
            window.draw(w.shape); 
        }
}
