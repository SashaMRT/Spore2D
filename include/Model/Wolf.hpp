/**
 * @file Wolf.hpp
 * @brief Définition du Loup
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.hpp"

// FORWARD DECLARATION (Indispensable pour casser la boucle d'inclusion)
class Sheep; 

class Wolf : public Entity {
public:
    Wolf(sf::Vector2f position);

    void update(float dt);
    
    // On dessine simplement (sans fioritures)
    void draw(sf::RenderWindow& window);
    
    void hunt(const std::vector<Sheep>& sheeps);
    void eat(std::vector<Sheep>& sheeps);

    bool canReproduce() const;
    void resetReproduction();

    float speed;

private:
    float m_reproCooldown;
};