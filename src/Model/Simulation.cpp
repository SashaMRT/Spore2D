/**
 * @file Simulation.cpp
 * @brief Logique métier : Gestion intelligente des bordures (Tuer vs Bloquer)
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

// CONFIGURATION
static std::map<std::string, float> g_config = {
    {"grass_spawn_rate", 5.0f},
    {"sheep_vision_food", 250.0f},
    {"sheep_panic_dist", 120.0f},
    {"wolf_vision", 400.0f}
};

static std::list<Grass> ecosystem_grass; 
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

// STATS
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

/**
 * @brief Gestion intelligente des limites du monde.
 * @details 
 * - Si l'entité dépasse un peu (elle marche vers le mur) -> On la bloque (Clamp).
 * - Si l'entité dépasse BEAUCOUP (la fenêtre a été réduite sur elle) -> On la tue.
 */
void checkWorldBounds(Entity& e) {
    float padding = 12.0f;          // Marge pour ne pas toucher le trait
    float killThreshold = 50.0f;    // Si on dépasse de 50px, c'est que la fenêtre a rétréci -> Mort.

    // 1. Vérification Limite GAUCHE
    if (e.pos.x < g_xMin + padding) {
        if (e.pos.x < g_xMin - killThreshold) e.alive = false; // Trop loin -> Mort
        else e.pos.x = g_xMin + padding;                       // Juste au bord -> Bloqué
    }
    // 2. Vérification Limite DROITE
    else if (e.pos.x > g_xMax - padding) {
        if (e.pos.x > g_xMax + killThreshold) e.alive = false;
        else e.pos.x = g_xMax - padding;
    }

    // 3. Vérification Limite HAUT
    if (e.pos.y < g_yMin + padding) {
        if (e.pos.y < g_yMin - killThreshold) e.alive = false;
        else e.pos.y = g_yMin + padding;
    }
    // 4. Vérification Limite BAS
    else if (e.pos.y > g_yMax - padding) {
        if (e.pos.y > g_yMax + killThreshold) e.alive = false;
        else e.pos.y = g_yMax - padding;
    }
}

// Physique de répulsion (Pas de changement ici)
void solveCollisions() {
    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
            Sheep& s1 = ecosystem_sheeps[i];
            Sheep& s2 = ecosystem_sheeps[j];
            if (!s1.alive || !s2.alive) continue;
            float d = s1.dist(s2.pos);
            if (d < 14.f && d > 0.1f) {
                sf::Vector2f push = (s1.pos - s2.pos) / d;
                float overlap = 14.f - d;
                s1.pos += push * (overlap * 0.5f);
                s2.pos -= push * (overlap * 0.5f);
            }
        }
    }
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

sf::Vector2f getWanderDir(int index, float time) {
    float angle = std::sin(time * 0.5f + index) * 3.14159f * 2.0f;
    return sf::Vector2f(std::cos(angle), std::sin(angle));
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

    for (int i = 0; i < 50; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(g_xMax - g_xMin - 40));
        float y = g_yMin + 20 + (rand() % (int)(g_yMax - g_yMin - 40));
        ecosystem_grass.emplace_back(Grass({x, y}));
    }
    for (int i = 0; i < 20; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(g_xMax - g_xMin - 40));
        float y = g_yMin + 20 + (rand() % (int)(g_yMax - g_yMin - 40));
        ecosystem_sheeps.emplace_back(Sheep({x, y}));
    }
    for (int i = 0; i < 3; i++) { 
        float x = g_xMin + 20 + (rand() % (int)(g_xMax - g_xMin - 40));
        float y = g_yMin + 20 + (rand() % (int)(g_yMax - g_yMin - 40));
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
        // HERBE : Si elle est hors des limites (même un peu), elle meurt.
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
        checkWorldBounds(w); // Utilise la nouvelle fonction (Tue ou Bloque)

        // IA
        const Sheep* target = nullptr;
        float minDist = g_config["wolf_vision"];

        for (const auto& s : ecosystem_sheeps) {
            if (s.alive) {
                float d = w.dist(s.pos);
                if (d < minDist) { minDist = d; target = &s; }
            }
        }

        if (target) {
            sf::Vector2f dir = target->pos - w.pos;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            w.pos += (dir / len) * w.speed * dt;
        } else {
            sf::Vector2f wander = getWanderDir(i, g_simulationTime * 0.5f);
            w.pos += wander * (w.speed * 0.6f) * dt;
        }

        w.eat(ecosystem_sheeps);

        if (w.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
                if (ecosystem_wolves[j].alive && ecosystem_wolves[j].canReproduce() && w.dist(ecosystem_wolves[j].pos) < 30.f) {
                    babyWolves.emplace_back(Wolf(w.pos)); w.resetReproduction(); ecosystem_wolves[j].resetReproduction(); g_bornWolvesCount++; break;
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
        
        for (auto& g : ecosystem_grass) {
            if (g.alive && s.dist(g.pos) < 15.f) { g.alive = false; s.gainEnergy(20.f); }
        }

        // IA
        sf::Vector2f moveDir{0.f, 0.f};
        const Wolf* danger = nullptr;
        float closestDanger = g_config["sheep_panic_dist"];

        for (const auto& w : ecosystem_wolves) {
            if (w.alive) {
                float d = s.dist(w.pos);
                if (d < closestDanger) { closestDanger = d; danger = &w; }
            }
        }

        if (danger) {
            sf::Vector2f flee = s.pos - danger->pos;
            float len = std::sqrt(flee.x*flee.x + flee.y*flee.y);
            if (len > 0) moveDir = (flee / len) * 1.5f;
        } 
        else {
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
                moveDir = getWanderDir(i + 100, g_simulationTime * 0.3f) * 0.5f;
            }
        }

        s.pos += moveDir * s.speed * dt;
        checkWorldBounds(s); // Utilise la nouvelle fonction (Tue ou Bloque)

        if (s.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
                if (ecosystem_sheeps[j].alive && ecosystem_sheeps[j].canReproduce() && s.dist(ecosystem_sheeps[j].pos) < 30.f) {
                    babySheeps.emplace_back(Sheep(s.pos)); s.resetReproduction(); ecosystem_sheeps[j].resetReproduction(); g_bornSheepCount++; break;
                }
            }
        }
    }

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
    return { (int)ecosystem_grass.size(), (int)ecosystem_sheeps.size(), (int)ecosystem_wolves.size(), g_deadSheepCount, g_deadWolvesCount, g_bornSheepCount, g_bornWolvesCount, g_simulationTime };
}

void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) if (g.alive) g.draw(window);
    for (auto& s : ecosystem_sheeps) if (s.alive) s.draw(window);
    for (auto& w : ecosystem_wolves) if (w.alive) w.draw(window);
}