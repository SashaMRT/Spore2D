/**
 * @file Simulation.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Logique métier optimisée (Cerveau de la simulation).
 * @version 0.9
 * @date 2026-01-06
 */

#include "../../include/Model/Simulation.hpp"
#include "../../include/Model/Entity.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Wolf.hpp"

#include <algorithm>
#include <vector>
#include <list> 
#include <cmath>

// -------------------------------------------------------------------------
// DONNÉES GLOBALES
// -------------------------------------------------------------------------

static float g_xMin = 0.f, g_xMax = 1000.f;
static float g_yMin = 0.f, g_yMax = 1000.f;
static float g_simulationTime = 0.f;

static std::list<Grass> ecosystem_grass; 
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

// Compteurs de statistiques
static int g_deadSheep = 0, g_deadWolves = 0;
static int g_bornSheep = 0, g_bornWolves = 0;

// -------------------------------------------------------------------------
// UTILITAIRES
// -------------------------------------------------------------------------

void setWorldBounds(float xMin, float xMax, float yMin, float yMax) {
    g_xMin = xMin; g_xMax = xMax;
    g_yMin = yMin; g_yMax = yMax;
}

// Helper : Génère une position aléatoire valide dans la carte
static sf::Vector2f randomPos() {
    float x = g_xMin + 20 + (rand() % (int)(std::max(1.f, g_xMax - g_xMin - 40)));
    float y = g_yMin + 20 + (rand() % (int)(std::max(1.f, g_yMax - g_yMin - 40)));
    return {x, y};
}

void solveCollisions() {
    // Lambda générique pour gérer les collisions d'un vecteur d'entités
    auto check = [](auto& entities) {
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                entities[i].resolveCollision(entities[j]);
            }
        }
    };
    check(ecosystem_sheeps);
    check(ecosystem_wolves);
}

// -------------------------------------------------------------------------
// LOGIQUE PRINCIPALE
// -------------------------------------------------------------------------

void initEcosystem() {
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();
    g_simulationTime = 0.f;
    g_deadSheep = 0; g_deadWolves = 0;
    g_bornSheep = 0; g_bornWolves = 0;

    // Création initiale simplifiée grâce à randomPos()
    for (int i = 0; i < 50; i++) ecosystem_grass.emplace_back(randomPos());
    for (int i = 0; i < 20; i++) ecosystem_sheeps.emplace_back(randomPos());
    for (int i = 0; i < 3;  i++) ecosystem_wolves.emplace_back(randomPos());
}

void ecosystemUpdate(float dt) {
    // Initialisation unique au premier appel (Astuce C++)
    static bool _init = [](){ srand(time(NULL)); initEcosystem(); return true; }();
    g_simulationTime += dt;

    // 1. HERBE (Apparition + Mise à jour + Nettoyage immédiat)
    if ((rand() % 100) < 5) ecosystem_grass.emplace_back(randomPos());

    for (auto it = ecosystem_grass.begin(); it != ecosystem_grass.end(); ) {
        it->update(dt);
        // Si hors map, on tue
        if (it->pos.x < g_xMin || it->pos.x > g_xMax || it->pos.y < g_yMin || it->pos.y > g_yMax) {
            it = ecosystem_grass.erase(it); // Suppression efficace
        } else {
            ++it;
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
        w.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax);

        // Reproduction
        if (w.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
                Wolf& partner = ecosystem_wolves[j];
                if (partner.alive && partner.canReproduce() && w.dist(partner.pos) < 30.f) {
                    babyWolves.emplace_back(w.pos);
                    w.resetReproduction();
                    partner.resetReproduction();
                    g_bornWolves++;
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
        
        // Manger herbe (Optimisation : break dès qu'on mange une herbe)
        for (auto& g : ecosystem_grass) {
            if (s.dist(g.pos) < 15.f) { 
                g.alive = false; // L'herbe sera supprimée au prochain tour
                s.gainEnergy(20.f); 
                break; 
            }
        }

        s.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax);

        // Reproduction
        if (s.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
                Sheep& partner = ecosystem_sheeps[j];
                if (partner.alive && partner.canReproduce() && s.dist(partner.pos) < 30.f) {
                    babySheeps.emplace_back(s.pos);
                    s.resetReproduction();
                    partner.resetReproduction();
                    g_bornSheep++;
                    break;
                }
            }
        }
    }

    // 4. NETTOYAGE ET FINALISATION
    solveCollisions();

    // Ajout des naissances
    ecosystem_sheeps.insert(ecosystem_sheeps.end(), babySheeps.begin(), babySheeps.end());
    ecosystem_wolves.insert(ecosystem_wolves.end(), babyWolves.begin(), babyWolves.end());

    // Suppression des morts (Lambda de nettoyage générique)
    auto cleanDead = [](auto& vec, int& counter) {
        vec.erase(std::remove_if(vec.begin(), vec.end(), [&](auto& e){
            if (!e.alive) { counter++; return true; }
            return false;
        }), vec.end());
    };

    cleanDead(ecosystem_sheeps, g_deadSheep);
    cleanDead(ecosystem_wolves, g_deadWolves);
    
    // Pour l'herbe, on nettoie les herbes mangées (marquées alive=false par les moutons)
    ecosystem_grass.remove_if([](const Grass& g){ return !g.alive; });
}

EcosystemStats getEcosystemStats() {
    return { 
        (int)ecosystem_grass.size(), (int)ecosystem_sheeps.size(), (int)ecosystem_wolves.size(), 
        g_deadSheep, g_deadWolves, g_bornSheep, g_bornWolves, g_simulationTime 
    };
}

void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) g.draw(window);
    for (auto& s : ecosystem_sheeps) if (s.alive) s.draw(window);
    for (auto& w : ecosystem_wolves) if (w.alive) w.draw(window);
}

void spawnEntity(EntityType type, float x, float y) {
    switch (type) {
        case EntityType::Grass: ecosystem_grass.emplace_back(sf::Vector2f(x, y)); break;
        case EntityType::Sheep: ecosystem_sheeps.emplace_back(sf::Vector2f(x, y)); break;
        case EntityType::Wolf:  ecosystem_wolves.emplace_back(sf::Vector2f(x, y)); break;
    }
}