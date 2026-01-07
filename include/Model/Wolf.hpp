/**
 * @file Wolf.hpp
 * @brief Déclaration de la classe Wolf avec gestion du niveau (Louveteau/Loup).
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.hpp"

class Sheep; 

class Wolf : public Entity {
public:
    Wolf(sf::Vector2f position);
    void update(float dt);
    void moveAI(float dt, const std::vector<Sheep>& sheeps, float simTime);
    void draw(sf::RenderWindow& window);
    void eat(std::vector<Sheep>& sheeps);
    bool canReproduce() const;
    void resetReproduction();
    
    int getLevel() const { return m_level; }

public:
    float speed;

private:
    float m_reproCooldown;
    int m_eatenSheep;
    int m_level;
};