/**
 * @file Wolf.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Wolf (Prédateur).
 * @version 0.6
 * @date 2026-01-06
 */

#include "../../include/Model/Wolf.hpp"
#include "../../include/Model/Sheep.hpp" // Nécessaire pour voir les moutons
#include <cmath> // Pour std::sqrt, std::cos, std::sin

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Wolf::Wolf(sf::Vector2f position) 
    : Entity(position, 100.f, sf::Color(200, 50, 50), 9.f) // Rouge
{
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(100, 0, 0)); 
    
    speed = 95.f;     // Rapide
    energy = 60.f;    // Énergie moyenne
    m_reproCooldown = 5.0f;
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

void Wolf::update(float dt) {
    if (!alive) return;

    // Métabolisme
    energy -= 3.0f * dt; 
    
    // Mort de faim
    if (energy <= 0) {
        alive = false;
        energy = 0;
    }

    // Cooldown reproduction
    if (m_reproCooldown > 0) {
        m_reproCooldown -= dt;
    }
}

// -------------------------------------------------------------------------
// INTELLIGENCE ARTIFICIELLE (DÉPLACEMENT)
// -------------------------------------------------------------------------

void Wolf::moveAI(float dt, const std::vector<Sheep>& sheeps, float simTime) {
    if (!alive) return;

    // 1. CHERCHER LA CIBLE
    const Sheep* target = nullptr;
    float minDist = 400.f; // Vision du loup

    for (const auto& s : sheeps) {
        if (s.alive) {
            float d = dist(s.pos);
            if (d < minDist) {
                minDist = d;
                target = &s;
            }
        }
    }

    // 2. SE DÉPLACER
    if (target) {
        // Mode Chasse : On court vers la cible
        sf::Vector2f dir = target->pos - pos;
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        
        if (len > 0) {
            pos += (dir / len) * speed * dt; 
        }
    } else {
        // Mode Errance : On se balade
        // Astuce : On utilise l'adresse mémoire (this) pour que chaque loup ait un mouvement unique
        long long seed = (long long)this % 100; 
        float angle = std::sin(simTime * 0.5f + seed) * 3.14159f * 2.0f;
        sf::Vector2f wander(std::cos(angle), std::sin(angle));

        // Vitesse réduite quand on erre (60%)
        pos += wander * (speed * 0.6f) * dt;
    }
}

// -------------------------------------------------------------------------
// ACTIONS (MANGER / REPRODUIRE)
// -------------------------------------------------------------------------

void Wolf::eat(std::vector<Sheep>& sheeps) {
    if (!alive) return;

    for (auto& s : sheeps) {
        if (s.alive && dist(s.pos) < 20.f) { 
            s.alive = false;  // Miam
            energy += 50.f; 
            if (energy > maxEnergy) energy = maxEnergy;
        }
    }
}

bool Wolf::canReproduce() const {
    return alive && energy > 80.f && m_reproCooldown <= 0.f;
}

void Wolf::resetReproduction() {
    energy -= 40.f;         
    m_reproCooldown = 10.f; 
}

void Wolf::draw(sf::RenderWindow& window) {
    shape.setPosition(pos); 
    window.draw(shape);     
}