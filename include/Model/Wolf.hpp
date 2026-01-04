/**
 * @file Wolf.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Définition de la classe Wolf (Loup).
 * @details Prédateur qui chasse les moutons pour gagner de l'énergie.
 * @version 0.2
 * @date 2026-01-04
 *
 * @copyright Copyright (c) 2026
 *
 */

// Comme ifndef mais plus moderne
#pragma once

// Bibliothèque utilisées
#include <SFML/Graphics.hpp>
#include <vector>
#include "Sheep.hpp" 

class Wolf {
public:
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------
    Wolf(sf::Vector2f position);

    // -------------------------------------------------------------------------
    // MÉTHODES DE MISE À JOUR & DESSIN
    // -------------------------------------------------------------------------
    void update(float dt);
    void draw(sf::RenderWindow& window);
    
    // -------------------------------------------------------------------------
    // CHASSE & ALIMENTATION
    // -------------------------------------------------------------------------
    /**
     * @brief Trouve et se déplace vers le mouton le plus proche.
     */
    void hunt(const std::vector<Sheep>& sheeps);

    /**
     * @brief Mange les moutons à proximité.
     */
    void eat(std::vector<Sheep>& sheeps);

    float dist(sf::Vector2f otherPos) const;

    // -------------------------------------------------------------------------
    // REPRODUCTION
    // -------------------------------------------------------------------------
    bool canReproduce() const;
    void resetReproduction();

    // -------------------------------------------------------------------------
    // ATTRIBUTS PUBLICS
    // -------------------------------------------------------------------------
    sf::Vector2f pos;
    sf::CircleShape shape;
    bool alive;
    float speed;

private:
    // -------------------------------------------------------------------------
    // ATTRIBUTS PRIVÉS
    // -------------------------------------------------------------------------
    float m_energy;
    float m_reproCooldown;
};