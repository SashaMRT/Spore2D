/**
 * @file Simulation.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Logique métier : Cerveau de la simulation.
 * @version 0.8
 * @date 2026-01-06
 */

#include "../../include/Model/Simulation.hpp"
#include "../../include/Model/Entity.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Wolf.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list> 
#include <map>  
#include <cmath>

// -------------------------------------------------------------------------
// VARIABLES GLOBALES
// -------------------------------------------------------------------------

static float g_xMin = 0.f, g_xMax = 1000.f;
static float g_yMin = 0.f, g_yMax = 1000.f;
static float g_simulationTime = 0.f;

// Configuration
static std::map<std::string, float> g_config = {
    {"grass_spawn_rate", 5.0f}
};

static std::list<Grass> ecosystem_grass; 
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

// Stats
static int g_deadSheepCount = 0;
static int g_deadWolvesCount = 0;
static int g_bornSheepCount = 0;
static int g_bornWolvesCount = 0;

// -------------------------------------------------------------------------
// FONCTIONS UTILITAIRES
// -------------------------------------------------------------------------

void setWorldBounds(float xMin, float xMax, float yMin, float yMax) {
    g_xMin = xMin; g_xMax = xMax;
    g_yMin = yMin; g_yMax = yMax;
}

// NOTE : La fonction checkWorldBounds a été déplacée dans Entity.cpp !

void solveCollisions() {
    // Moutons entre eux
    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
            // On utilise la nouvelle méthode de l'entité
            ecosystem_sheeps[i].resolveCollision(ecosystem_sheeps[j]);
        }
    }
    // Loups entre eux
    for (size_t i = 0; i < ecosystem_wolves.size(); ++i) {
        for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
            ecosystem_wolves[i].resolveCollision(ecosystem_wolves[j]);
        }
    }
}

// -------------------------------------------------------------------------
// LOGIQUE PRINCIPALE
// -------------------------------------------------------------------------

void initEcosystem() {
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();
    g_simulationTime = 0.f;
    g_deadSheepCount = 0; g_deadWolvesCount = 0;
    g_bornSheepCount = 0; g_bornWolvesCount = 0;

    // Population initiale
    for (int i = 0; i < 50; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_grass.emplace_back(Grass({x, y}));
    }
    for (int i = 0; i < 20; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_sheeps.emplace_back(Sheep({x, y}));
    }
    for (int i = 0; i < 3; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_wolves.emplace_back(Wolf({x, y}));
    }
}

void ecosystemUpdate(float dt) {
    static bool randInit = false;
    if (!randInit) { srand(time(NULL)); initEcosystem(); randInit = true; }

    g_simulationTime += dt;

    // 1. HERBE
    if ((rand() % 100) < g_config["grass_spawn_rate"]) { 
        float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
        float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
        ecosystem_grass.push_back(Grass({x, y}));
    }
    for (auto& g : ecosystem_grass) {
        g.update(dt);
        if (g.pos.x < g_xMin || g.pos.x > g_xMax || g.pos.y < g_yMin || g.pos.y > g_yMax) {
            g.alive = false;
        }
    }

    // 2. LOUPS
    std::vector<Wolf> babyWolves;
    for (size_t i = 0; i < ecosystem_wolves.size(); ++i) {
        Wolf& w = ecosystem_wolves[i];
        if (!w.alive) continue;

        w.update(dt);
        w.moveAI(dt, ecosystem_sheeps, g_simulationTime);
        w.eat(ecosystem_sheeps);
        
        // [MODIFICATION] On appelle la méthode sur l'entité en lui donnant les limites globales
        w.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax);

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

    // 3. MOUTONS
    std::vector<Sheep> babySheeps;
    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        Sheep& s = ecosystem_sheeps[i];
        if (!s.alive) continue;

        s.update(dt);
        s.moveAI(dt, ecosystem_wolves, ecosystem_grass, g_simulationTime);
        
        for (auto& g : ecosystem_grass) {
            if (g.alive && s.dist(g.pos) < 15.f) { g.alive = false; s.gainEnergy(20.f); }
        }

        // [MODIFICATION] Pareil ici
        s.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax);

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

    // 4. NETTOYAGE
    solveCollisions();

    for (auto& b : babySheeps) ecosystem_sheeps.push_back(b);
    for (auto& b : babyWolves) ecosystem_wolves.push_back(b);

    ecosystem_grass.remove_if([](const Grass& g){ return !g.alive; });
    
    ecosystem_sheeps.erase(std::remove_if(ecosystem_sheeps.begin(), ecosystem_sheeps.end(), 
        [](auto& s) { if (!s.alive) g_deadSheepCount++; return !s.alive; }
    ), ecosystem_sheeps.end());

    ecosystem_wolves.erase(std::remove_if(ecosystem_wolves.begin(), ecosystem_wolves.end(), 
        [](auto& w) { if (!w.alive) g_deadWolvesCount++; return !w.alive; }
    ), ecosystem_wolves.end());
}

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

void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) if (g.alive) g.draw(window);
    for (auto& s : ecosystem_sheeps) if (s.alive) s.draw(window);
    for (auto& w : ecosystem_wolves) if (w.alive) w.draw(window);
}

// -------------------------------------------------------------------------
// SPAWN MANUEL (MODE DIEU)
// -------------------------------------------------------------------------

void spawnEntity(EntityType type, float x, float y) {
    sf::Vector2f pos(x, y);

    switch (type) {
        case EntityType::Grass: ecosystem_grass.emplace_back(Grass(pos)); break;
        case EntityType::Sheep: ecosystem_sheeps.emplace_back(Sheep(pos)); break;
        case EntityType::Wolf: ecosystem_wolves.emplace_back(Wolf(pos)); break;
    }
}