/**
 * @file Simulation.cpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Gestion centrale de l'écosystème : herbe, moutons, loups (spawn, IA, rendu)
 * @details 
 *  - Orchestre la simulation complète du monde vivant
 *  - Gère les populations globales (conteneurs statiques)
 *  - Implémente l'IA : recherche herbe, fuite loups, évitement même espèce
 *  - LIMITES CARRÉ RENDERER : pile dans la zone de jeu noire
 * @version 0.1
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 */

// ===== INCLUSIONS =====
// Dépendances principales de la simulation écosystème
#include "../../include/Model/Simulation.hpp"  // Interface publique (init/update/draw)
#include "../../include/Model/Entity.hpp"      // Classe de base (pos, energy, shape)
#include "../../include/Model/Grass.hpp"       // Touffes d'herbe régénératives
#include "../../include/Model/Sheep.hpp"       // Proies : pâturage + fuite loups
#include "../../include/Model/Wolf.hpp"        // Prédateurs : chasse + prédation
#include <algorithm>     // std::remove_if (nettoyage entités mortes)
#include <cstdlib>       // rand() (spawn aléatoire)
#include <ctime>         // time(NULL) (seed aléatoire)
#include <cmath>         // sqrt() (distances, normalisation vecteurs)


// ===== CONTENEURS GLOBAUX =====
static std::vector<Grass> ecosystem_grass;
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

// ===== LIMITES CARRÉ RENDERER (EXACTES) =====
static void keepInMapBounds(sf::Vector2f& pos) {
    // DIMENSIONS CALCULÉES depuis Renderer::init()
    // hudWidth+10+5 = gauche | window.x-10 = droite | 10+5 = haut | window.y-10 = bas
    const float MAP_LEFT = 265.f;     
        // ↑ DIMINUER (260, 255) = entités PLUS À GAUCHE (touche bord)
        // ↓ AUGMENTER (270, 275) = entités PLUS À DROITE (écart bord)
    
    const float MAP_RIGHT = 1255.f;   
        // ↑ DIMINUER (1250, 1245) = entités PLUS À GAUCHE (touche bord droit)
        // ↓ AUGMENTER (1260, 1265) = entités PLUS À DROITE (écart bord droit)
    
    const float MAP_TOP = 15.f;       
        // ↑ DIMINUER (10, 5) = entités PLUS HAUT
        // ↓ AUGMENTER (20, 25) = entités PLUS BAS
    
    const float MAP_BOTTOM = 700.f;   
        // ↑ DIMINUER (695, 690) = entités PLUS HAUT (touche bord bas)
        // ↓ AUGMENTER (705, 710) = entités PLUS BAS (écart bord bas)
    
    // BLOCAGE STRICT dans le carré noir
    if (pos.x < MAP_LEFT) pos.x = MAP_LEFT;
    if (pos.x > MAP_RIGHT) pos.x = MAP_RIGHT;
    if (pos.y < MAP_TOP) pos.y = MAP_TOP;
    if (pos.y > MAP_BOTTOM) pos.y = MAP_BOTTOM;
}

// ===== INITIALISATION (dans le carré) =====
void initEcosystem() {
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();

    // 8 herbes DANS le carré
    for (int i = 0; i < 8; i++) {
        float x = 350 + (rand() % 900);  // 350-1250 → clampé 315-1270
        float y = 50 + (rand() % 630);   // 50-680 → clampé 15-695
        ecosystem_grass.emplace_back(Grass({x, y}));
    }

    // 5 moutons DANS le carré
    for (int i = 0; i < 5; i++) {
        float x = 400 + (rand() % 800);
        float y = 100 + (rand() % 550);
        ecosystem_sheeps.emplace_back(Sheep({x, y}));
    }

    // 3 loups DANS le carré
    for (int i = 0; i < 3; i++) {
        float x = 600 + (rand() % 600);
        float y = 150 + (rand() % 450);
        ecosystem_wolves.emplace_back(Wolf({x, y}));
    }
}

// ===== MISE À JOUR PRINCIPALE =====
void ecosystemUpdate(float dt) {
    static bool randInit = false;
    if (!randInit) {
        srand(static_cast<unsigned>(time(NULL)));
        initEcosystem();
        randInit = true;
    }

    // HERBE
    for (auto& g : ecosystem_grass) g.update(dt);

    // LOUPS
    for (auto& w : ecosystem_wolves) {
        if (!w.alive) continue;
        
        w.update(dt);
        sf::Vector2f moveDir{0.f, 0.f};
        
        w.hunt(ecosystem_sheeps);
        
        // Évitement loups (45px)
        sf::Vector2f avoidDir{0.f, 0.f};
        for (const auto& other : ecosystem_wolves) {
            if (&other == &w || !other.alive) continue;
            float d = w.dist(other.pos);
            if (d < 45.f && d > 0.01f) {
                sf::Vector2f away = w.pos - other.pos;
                float strength = (45.f - d) / 45.f;
                avoidDir += (away / d) * strength * 2.f;
            }
        }
        
        moveDir += avoidDir;
        float len = std::sqrt(moveDir.x*moveDir.x + moveDir.y*moveDir.y);
        if (len > 0.01f) {
            moveDir /= len;
            w.pos += moveDir * w.speed * dt;
        }
        
        w.eat(ecosystem_sheeps);
        keepInMapBounds(w.pos);  // DANS LE CARRÉ
    }

    // MOUTONS
    for (auto& s : ecosystem_sheeps) {
        if (!s.alive) continue;
        
        s.update(dt);
        sf::Vector2f moveDir{0.f, 0.f};

        // Herbe proche
        Grass* bestGrass = nullptr;
        float bestDist = 999.f;
        for (auto& g : ecosystem_grass) {
            if (g.alive) {
                float d = s.dist(g.pos);
                if (d < bestDist && d < 200.f) {
                    bestDist = d;
                    bestGrass = &g;
                }
            }
        }

        if (bestGrass && bestDist < 550.f) {
            sf::Vector2f dir = bestGrass->pos - s.pos;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            moveDir = dir / len;
        }

        // Fuite loups
        for (const auto& w : ecosystem_wolves) {
            if (w.alive && s.dist(w.pos) < 100.f) {
                sf::Vector2f flee = s.pos - w.pos;
                float len = std::sqrt(flee.x*flee.x + flee.y*flee.y);
                moveDir = (flee / len) * 1.5f;
                break;
            }
        }

        // Évitement moutons
        sf::Vector2f avoidSheep{0.f, 0.f};
        for (const auto& other : ecosystem_sheeps) {
            if (&other == &s || !other.alive) continue;
            float d = s.dist(other.pos);
            if (d < 35.f && d > 0.01f) {
                sf::Vector2f away = s.pos - other.pos;
                float strength = (35.f - d) / 35.f;
                avoidSheep += (away / d) * strength * 1.5f;
            }
        }
        
        moveDir += avoidSheep;
        
        float len = std::sqrt(moveDir.x*moveDir.x + moveDir.y*moveDir.y);
        if (len > 0.01f) {
            moveDir /= len;
            s.pos += moveDir * s.speed * dt;
        }
        
        keepInMapBounds(s.pos);  // DANS LE CARRÉ
        s.eat(ecosystem_grass);
    }

    // NETTOYAGE
    ecosystem_grass.erase(std::remove_if(ecosystem_grass.begin(), ecosystem_grass.end(), 
        [](auto& g){return !g.alive;}), ecosystem_grass.end());
    ecosystem_sheeps.erase(std::remove_if(ecosystem_sheeps.begin(), ecosystem_sheeps.end(), 
        [](auto& s){return !s.alive;}), ecosystem_sheeps.end());
    ecosystem_wolves.erase(std::remove_if(ecosystem_wolves.begin(), ecosystem_wolves.end(), 
        [](auto& w){return !w.alive;}), ecosystem_wolves.end());

    // RESPAWN
    if (ecosystem_sheeps.empty()) initEcosystem();
}

// ===== RENDU =====
void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) {
        g.shape.setPosition(g.pos);
        window.draw(g.shape);
    }
    
    for (auto& s : ecosystem_sheeps) {
        if (s.alive) {
            s.shape.setPosition(s.pos);
            window.draw(s.shape);
        }
    }
    
    for (auto& w : ecosystem_wolves) {
        if (w.alive) {
            w.shape.setPosition(w.pos);
            window.draw(w.shape);
        }
    }
}

// ===== STATS =====
EcosystemStats getEcosystemStats() {
    return {
        static_cast<int>(ecosystem_grass.size()),
        static_cast<int>(ecosystem_sheeps.size()),
        static_cast<int>(ecosystem_wolves.size())
    };
}
