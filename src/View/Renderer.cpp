/**
 * @file Application.cpp
 * @author sasha.marieterehorst (sasha.marieterehorst@gmail.com)
 * @brief Implémentation de la classe Renderer : Représentation de l'affichage du monde du jeu.
 * @details Place la zone de jeu à droite du menu (HUD) et dessine les bordures.
 * @version 0.1
 * @date 2025-12-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// Inclusion de notre bibliothèque Renderer.
// Elle permet l'utilisation de la classe Renderer et de ces méthodes.
#include "../../include/View/Renderer.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Renderer.
 * C'est ici que l'on initialise les variables.
 */
Renderer::Renderer() {
    // Vide pour l'instant
}

// -------------------------------------------------------------------------
// MÉTHODES PUBLIQUES
// -------------------------------------------------------------------------

/**
 * @brief Initialise la zone de jeu.
 * @details Calcule la taille et la position de la zone de jeu en fonction de la taille
 * de la fenêtre et de la largeur du menu.
 * @param windowSize La taille totale de la fenêtre.
 * @param hudWidth La largeur du menu en pixels.
 */
void Renderer::init(sf::Vector2u windowSize, float hudWidth) {
    // Calcul de la place restante pour le jeu
    // On convertit les unsigned int en float pour les calculs de position (sinon erreurs).
    float gameWidth = static_cast<float>(windowSize.x) - hudWidth;
    float gameHeight = static_cast<float>(windowSize.y);

    // Configuration de la zone de jeu
    // On retire 20 pixels en largeur et hauteur pour laisser une marge (sinon colle le menu)
    m_gameArea.setSize(sf::Vector2f(gameWidth - 20.f, gameHeight - 20.f));
    
    // Positionnement du rectangle
    // X = On commence après le menu (+10px de marge)
    // Y = On commence un peu en bas (+10px de marge)
    m_gameArea.setPosition(sf::Vector2f(hudWidth + 10.f, 10.f));
    
    // Style
    m_gameArea.setFillColor(sf::Color::Black);            // Fond noir
    m_gameArea.setOutlineThickness(2.f);                  // Épaisseur de la bordure
    m_gameArea.setOutlineColor(sf::Color(100, 100, 100)); // Gris clair pour la bordure
}

/**
 * @brief Dessine le terrain sur la fenêtre.
 * @param window La fenêtre SFML où l'on doit dessiner.
 */
void Renderer::draw(sf::RenderWindow& window) {
    // On dessine le fond noir prédéfinit
    window.draw(m_gameArea);
}

/**
 * @brief Recalcul de la zone de jeu.
 */
void Renderer::onResize(sf::Vector2u newSize, float hudWidth) {
    // Calcul des nouvelles dimensions
    // Largeur = Largeur fenêtre - Largeur Menu
    float gameWidth = static_cast<float>(newSize.x) - hudWidth;
    float gameHeight = static_cast<float>(newSize.y);

    // Mise à jour de la zone de jeu
    // On garde toujours la petite marge de 20 pixels (-20.f)
    m_gameArea.setSize(sf::Vector2f(gameWidth - 20.f, gameHeight - 20.f));
}