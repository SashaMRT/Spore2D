/**
 * @file Wolf.cpp
 * @brief Implémentation du Loup
 */

#include "../../include/Model/Wolf.hpp"
#include "../../include/Model/Sheep.hpp" // Nécessaire ici pour le code
#include <cmath>

Wolf::Wolf(sf::Vector2f position) 
    : Entity(position, 100.f, sf::Color(200, 50, 50), 9.f) // ROUGE pour être visible
{
    // On remet la forme simple demandée
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(100, 0, 0)); 
    
    speed = 95.f;
    energy = 60.f; // Un peu plus d'énergie au départ
    m_reproCooldown = 5.0f;
}

void Wolf::update(float dt) {
    if (!alive) return;

    energy -= 3.0f * dt; 
    if (energy <= 0) alive = false;
    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Wolf::hunt(const std::vector<Sheep>& sheeps) {
    if (!alive) return;
    
    const Sheep* target = nullptr;
    float minDist = 350.f; // Bonne vision

    for (const auto& s : sheeps) {
        if (s.alive) {
            float d = dist(s.pos);
            if (d < minDist) {
                minDist = d;
                target = &s;
            }
        }
    }

    if (target) {
        sf::Vector2f dir = target->pos - pos;
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (len > 0) pos += (dir / len) * speed * 0.016f; 
    }
}

void Wolf::eat(std::vector<Sheep>& sheeps) {
    if (!alive) return;

    for (auto& s : sheeps) {
        if (s.alive && dist(s.pos) < 20.f) { // Distance de contact
            s.alive = false;  
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
    // DESSIN SIMPLE : Juste le cercle de base
    shape.setPosition(pos);
    window.draw(shape);
}