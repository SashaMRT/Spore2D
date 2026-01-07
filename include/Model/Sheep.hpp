/**
 * @file Sheep.hpp
 * @brief Déclaration de la classe Sheep avec gestion du niveau (Agneau/Mouton).
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "Entity.hpp"

class Grass;
class Wolf; 

class Sheep : public Entity {
public:
    Sheep(sf::Vector2f position);
    void update(float dt);
    void moveAI(float dt, const std::vector<Wolf>& wolves, const std::list<Grass>& grass, float simTime);
    void draw(sf::RenderWindow& window);
    
    void eatGrass(); 
    int getLevel() const { return m_level; }

    bool canReproduce() const; 
    void resetReproduction();  

public:
    float speed;

private:
    float m_reproCooldown;
    int m_eatenGrass; 
    int m_level;      
};