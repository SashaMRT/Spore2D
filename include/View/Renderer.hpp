/**
 * @file Renderer.hpp
 * @brief Déclaration de la classe Renderer (Nettoyée)
 */

#pragma once

#include <SFML/Graphics.hpp>

class Renderer {
public:
    Renderer();

    // Initialise la zone de jeu
    void init(sf::Vector2u windowSize, float hudWidth);

    // Dessine le fond et appelle le dessin des entités
    void draw(sf::RenderWindow& window);

    // Recalcule la taille du rectangle noir lors du redimensionnement
    void onResize(sf::Vector2u newSize, float hudWidth);

private:
    // Zone de jeu (Rectangle noir avec bordure)
    sf::RectangleShape m_gameArea;
    
    // NOTE : J'ai supprimé m_font et m_statsText car ils ne servent plus.
    // L'affichage du texte est géré uniquement par le HUD maintenant.
};