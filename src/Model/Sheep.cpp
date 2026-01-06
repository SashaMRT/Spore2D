/**
 * @file Sheep.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation optimisée du Mouton.
 * @version 0.7
 * @date 2026-01-06
 */

#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Wolf.hpp" 
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
// LOGIQUE
// -------------------------------------------------------------------------

void Sheep::update(float dt) {
    if (!alive) return;

    energy -= 2.0f * dt; 
    if (energy <= 0) alive = false;

    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Sheep::moveAI(float dt, const std::vector<Wolf>& wolves, const std::list<Grass>& grass, float simTime) {
    if (!alive) return;

    sf::Vector2f moveDir(0.f, 0.f);
    
    // 1. DANGER (Priorité Absolue)
    const Wolf* danger = nullptr;
    // On compare les distances au carré pour éviter les racines carrées (Optimisation CPU)
    float minDangerSq = 120.f * 120.f; 

    for (const auto& w : wolves) {
        if (!w.alive) continue;
        
        float dx = pos.x - w.pos.x;
        float dy = pos.y - w.pos.y;
        float distSq = dx*dx + dy*dy;

        if (distSq < minDangerSq) {
            minDangerSq = distSq;
            danger = &w;
        }
    }

    if (danger) {
        // FUITE : Vecteur opposé au danger
        sf::Vector2f diff = pos - danger->pos;
        // Ici on a besoin de la vraie longueur pour normaliser
        float len = std::sqrt(minDangerSq); 
        if (len > 0.1f) moveDir = (diff / len) * 1.5f; // Fuite rapide (x1.5)
    } 
    else {
        // 2. NOURRITURE
        sf::Vector2f targetPos = pos;
        bool found = false;
        float minFoodSq = 250.f * 250.f;

        for (const auto& g : grass) {
            if (!g.alive) continue;

            float dx = pos.x - g.pos.x;
            float dy = pos.y - g.pos.y;
            float distSq = dx*dx + dy*dy;

            if (distSq < minFoodSq) {
                minFoodSq = distSq;
                targetPos = g.pos;
                found = true;
            }
        }

        if (found) {
            // MIAM : On va vers l'herbe
            sf::Vector2f diff = targetPos - pos;
            float len = std::sqrt(minFoodSq);
            if (len > 0.1f) moveDir = diff / len;
        } else {
            // 3. ERRANCE
            long long seed = (long long)this % 100;
            float angle = std::sin(simTime * 0.3f + seed) * 6.28f;
            
            // Marche lente
            moveDir = {std::cos(angle), std::sin(angle)};
            moveDir *= 0.5f; 
        }
    }

    pos += moveDir * speed * dt;
}

// -------------------------------------------------------------------------
// ACTIONS
// -------------------------------------------------------------------------

void Sheep::eat(std::vector<Grass>& grasses) {
    // Cette méthode est vide car la détection de "manger" est faite 
    // directement dans la boucle principale (Simulation.cpp) pour optimiser les performances.
    // On la garde ici pour satisfaire le fichier .hpp.
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