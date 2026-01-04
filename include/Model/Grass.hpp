/**
 * @file Grass.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Définition de la classe Grass (L'herbe).
 * @details Ressource statique mangée par les moutons.
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

class Grass {
public:
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------
    
    /**
     * @brief Crée une touffe d'herbe.
     * @param position Où l'herbe pousse (X, Y).
     */
    Grass(sf::Vector2f position);

    // -------------------------------------------------------------------------
    // MÉTHODES
    // -------------------------------------------------------------------------

    /**
     * @brief Mise à jour (vide pour l'instant, l'herbe est statique).
     */
    void update(float dt);

    /**
     * @brief Dessine l'herbe sur la fenêtre.
     * C'est la fonction qui manquait !
     */
    void draw(sf::RenderWindow& window);

    // -------------------------------------------------------------------------
    // ATTRIBUTS PUBLICS
    // -------------------------------------------------------------------------
    sf::Vector2f pos;       // Position
    sf::CircleShape shape;  // Forme de base
    bool alive;             // Est-elle mangée ou non ?
};