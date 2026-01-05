/**
 * @file Sheep.hpp
 * @brief Définition du Mouton
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.hpp"

// Forward declaration
class Grass;

class Sheep : public Entity {
public:
    Sheep(sf::Vector2f position);

    void update(float dt);
    
    // Dessin simple
    void draw(sf::RenderWindow& window);
    
    // Gestion nourriture
    void eat(std::vector<Grass>& grasses);
    void gainEnergy(float amount);

    bool canReproduce() const; 
    void resetReproduction();  

    float speed;

private:
    float m_reproCooldown;
};