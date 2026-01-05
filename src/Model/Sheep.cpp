/**
 * @file Sheep.cpp
 * @brief Implémentation du Mouton
 */

#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Grass.hpp"
#include <cmath>

Sheep::Sheep(sf::Vector2f position) 
    : Entity(position, 100.f, sf::Color(240, 240, 240), 7.f) // BLANC
{
    speed = 60.f;
    energy = 50.f; 
    m_reproCooldown = 2.0f; 
}

void Sheep::update(float dt) {
    if (!alive) return;

    energy -= 2.0f * dt; 
    if (energy <= 0) alive = false; 
    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Sheep::eat(std::vector<Grass>& grasses) {
    // Géré par la simulation principalement
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
    // DESSIN SIMPLE : Juste le cercle
    shape.setPosition(pos);
    window.draw(shape);
}