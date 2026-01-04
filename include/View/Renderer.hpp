/**
 * @file Renderer.hpp
 * @author Sasha Marie te Rehorst
 * @brief Déclaration de la classe Renderer : Schéma de l'affichage du monde du jeu.
 * @details Dessiner la zone de jeu.
 * @version 0.1
 * @date 2025-12-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// Empêche d'inclure ce fichier deux fois.
#pragma once

// Inclusion de notre bibliothèque SFML.
// Elle permet l'utilisation de sf.
#include <SFML/Graphics.hpp>

/**
 * @class Renderer
 * @brief Classe responsable du rendu du terrain.
 */
class Renderer {
// Accessible depuis l'extérieur (par le main.cpp par exemple).
public:
    // Le Constructeur : Il porte TOUJOURS le même nom que la classe.
    // C'est la fonction appelée automatiquement à la création de l'objet.
    // Dans notre cas, c'est la fonction qui défini le rendu du terrain.
    Renderer();

    /**
     * @brief Configure la zone de jeu.
     * @param windowSize La taille totale de la fenêtre.
     * @param hudWidth La largeur du menu (pour savoir où dessiner).
     */
    void init(sf::Vector2u windowSize, float hudWidth);

    /**
     * @brief Dessine le terrain.
     * @param window La fenêtre en paramètre.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Réagit au redimensionnement de la fenêtre.
     * @details Recalcule la taille de la zone de jeu pour qu'il occupe
     * tout l'espace disponible après le menu.
     * @param newSize La nouvelle taille totale de la fenêtre.
     * @param hudWidth La largeur du menu.
     */
    void onResize(sf::Vector2u newSize, float hudWidth);

private:
    // Représente le terrain de jeu
    sf::RectangleShape m_gameArea;
    sf::Font m_font;      // Pour charger la police (arial.ttf)
    sf::Text m_statsText; // Pour afficher le texte des stats
};