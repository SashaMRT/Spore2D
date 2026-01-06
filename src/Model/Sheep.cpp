/**
 * @file Sheep.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Sheep (Proie).
 * @version 0.6
 * @date 2026-01-06
 */

#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Wolf.hpp" // [IMPORTANT] Pour voir les loups
#include <cmath>

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Sheep::Sheep(sf::Vector2f position) 
    : Entity(position, 100.f, sf::Color(240, 240, 240), 7.f) // Blanc
{
    speed = 60.f;        
    energy = 50.f;       
    m_reproCooldown = 2.0f; 
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

void Sheep::update(float dt) {
    if (!alive) return;

    // Métabolisme
    energy -= 2.0f * dt; 
    
    if (energy <= 0) {
        alive = false;
    }

    if (m_reproCooldown > 0) {
        m_reproCooldown -= dt;
    }
}

// -------------------------------------------------------------------------
// INTELLIGENCE ARTIFICIELLE (DÉPLACEMENT)
// -------------------------------------------------------------------------

void Sheep::moveAI(float dt, const std::vector<Wolf>& wolves, const std::list<Grass>& grass, float simTime) {
    if (!alive) return;

    sf::Vector2f moveDir{0.f, 0.f};
    
    // --- 1. DÉTECTION DU DANGER (PRIORITÉ ABSOLUE) ---
    const Wolf* danger = nullptr;
    float closestDanger = 120.0f; // Distance de panique

    for (const auto& w : wolves) {
        if (w.alive) {
            float d = dist(w.pos);
            if (d < closestDanger) {
                closestDanger = d;
                danger = &w;
            }
        }
    }

    if (danger) {
        // FUITE : Vecteur opposé au danger
        sf::Vector2f flee = pos - danger->pos;
        float len = std::sqrt(flee.x*flee.x + flee.y*flee.y);
        
        if (len > 0) {
            // Fuite rapide (x1.5)
            moveDir = (flee / len) * 1.5f; 
        }
    } 
    else {
        // --- 2. RECHERCHE DE NOURRITURE ---
        float bestDist = 250.0f; // Vision nourriture
        sf::Vector2f targetGrass = pos;
        bool foundGrass = false;
        
        for (const auto& g : grass) {
            if (g.alive) {
                float d = dist(g.pos);
                if (d < bestDist) {
                    bestDist = d;
                    targetGrass = g.pos;
                    foundGrass = true;
                }
            }
        }

        if (foundGrass) {
            // MIAM : On va vers l'herbe
            sf::Vector2f dir = targetGrass - pos;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            if(len > 0) moveDir = dir / len;
        } else {
            // --- 3. ERRANCE TRANQUILLE ---
            long long seed = (long long)this % 100;
            float angle = std::sin(simTime * 0.3f + seed) * 3.14159f * 2.0f;
            sf::Vector2f wander(std::cos(angle), std::sin(angle));
            
            // Marche lente (x0.5)
            moveDir = wander * 0.5f; 
        }
    }

    // Application du mouvement
    pos += moveDir * speed * dt;
}

// -------------------------------------------------------------------------
// AUTRES MÉTHODES
// -------------------------------------------------------------------------

void Sheep::eat(std::vector<Grass>& grasses) {
    // (Géré par Simulation.cpp pour les collisions simples)
}

void Sheep::gainEnergy(float amount) {
    energy += amount;
    if (energy > maxEnergy) energy = maxEnergy;
}

bool Sheep::canReproduce() const {
    return alive && energy > 60.f && m_reproCooldown <= 0.f;
}

void Sheep::resetReproduction() {
    energy -= 30.f;      
    m_reproCooldown = 5.f; 
}

void Sheep::draw(sf::RenderWindow& window) {
    shape.setPosition(pos);
    window.draw(shape);
}