/**
 * @file Grass.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Définition de la classe Grass (Nourriture).
 * @details Ressource statique qui apparaît aléatoirement et sert de nourriture aux moutons.
 * @version 0.5
 * @date 2026-01-05
 */

#pragma once

// Bibliothèque utilisées
#include <SFML/Graphics.hpp>

/**
 * @class Grass
 * @brief Représente une unité de nourriture (Herbe).
 */
class Grass {
public:
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------
    
    /**
     * @brief Construit une touffe d'herbe.
     * @param position La position (x, y) où l'herbe pousse.
     */
    Grass(sf::Vector2f position);

    // -------------------------------------------------------------------------
    // MÉTHODES
    // -------------------------------------------------------------------------

    /**
     * @brief Met à jour l'état de l'herbe.
     * @details (Vide pour l'instant car l'herbe est statique, mais nécessaire pour l'uniformité).
     * @param dt Temps écoulé.
     */
    void update(float dt);

    /**
     * @brief Dessine l'herbe sur la fenêtre.
     * @details Affiche la forme principale et les détails décoratifs.
     * @param window La fenêtre SFML cible.
     */
    void draw(sf::RenderWindow& window);

    // -------------------------------------------------------------------------
    // ATTRIBUTS PUBLICS
    // -------------------------------------------------------------------------
    
    sf::Vector2f pos;       ///< Position exacte dans le monde.
    sf::CircleShape shape;  ///< Forme géométrique de base (Cercle).
    bool alive;             ///< État de l'herbe (true = visible, false = mangée).
};