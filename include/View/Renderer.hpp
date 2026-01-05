/**
 * @file Renderer.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de la classe Renderer : Gestion de l'affichage du monde.
 * @details Cette classe est responsable de l'affichage de la zone de simulation (le rectangle noir)
 * et de l'appel aux fonctions de dessin des entités.
 * Elle a été allégée pour ne plus gérer le texte (rôle transféré au Hud).
 * @version 0.5
 * @date 2026-01-05
 */

#pragma once

// Inclusion de la bibliothèque graphique SFML
#include <SFML/Graphics.hpp>

/**
 * @class Renderer
 * @brief Gère le rendu graphique de la partie "Jeu" (hors interface).
 */
class Renderer {
public:
    /**
     * @brief Constructeur par défaut.
     */
    Renderer();

    /**
     * @brief Initialise la zone de jeu (Géométrie et Style).
     * @param windowSize La taille totale de la fenêtre.
     * @param hudWidth La largeur du menu pour calculer le décalage.
     */
    void init(sf::Vector2u windowSize, float hudWidth);

    /**
     * @brief Dessine le fond et les entités.
     * @details Affiche d'abord le rectangle noir, puis appelle ecosystemDraw() pour les animaux.
     * @param window La fenêtre cible.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Recalcule la taille du rectangle noir lors du redimensionnement.
     * @details Permet au jeu de s'adapter dynamiquement si l'utilisateur change la taille de la fenêtre.
     * @param newSize La nouvelle taille de la fenêtre.
     * @param hudWidth La largeur du menu (qui reste fixe).
     */
    void onResize(sf::Vector2u newSize, float hudWidth);

private:
    // La zone de jeu (Rectangle noir avec bordure grise)
    // C'est le "tapis" sur lequel les animaux se déplacent.
    sf::RectangleShape m_gameArea;
};