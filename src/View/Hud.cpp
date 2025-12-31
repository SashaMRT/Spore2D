/**
 * @file Hud.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @brief Implémentation de la classe Hud : Représentation du gestionnaire d'informations
 * @details Contient le chargement des polices et de mise à jour des textes.
 * @version 0.1
 * @date 2025-12-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// Inclusion de la bibliothèque std.
#include <iostream> 

// Inclusion de la bibliothèque string.
#include <string>

// Inclusion de notre bibliothèque Hud.
// Elle permet l'utilisation de la classe Hud et de ces méthodes.
#include "../../include/View/Hud.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Hud.
 * C'est ici que l'on initialise les variables.
 */
Hud::Hud() :
    m_textFps(m_font),
    m_textInfo(m_font)
{
    m_width = 280.f; // Sinon il prend aussi hud
}

// -------------------------------------------------------------------------
// MÉTHODES PUBLIQUES
// -------------------------------------------------------------------------

/**
 * @brief Chargement de la police et configuration des textes.
 */
bool Hud::init(sf::Vector2u windowSize) {
    // Chargement de la police
    if (!m_font.openFromFile("../assets/font.ttf")) {
        // Debuggage (en cas d'erreur)
        std::cerr << "[ERREUR] Hud: Impossible de trouver 'assets/font.ttf'" << std::endl;
        return false; // Echec
    }

    // Debuggage (On suit le chargement des polices).
    std::cout << "[Hud] Police chargée avec succès." << std::endl;

    // 2. Configuration de la zone de jeu
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(windowSize.y)));

    // Configuration du texte FPS
    m_textFps.setCharacterSize(20);                     // Taille en pixels
    m_textFps.setFillColor(sf::Color::Green);           // Couleur verte
    m_textFps.setPosition(sf::Vector2f(10.f, 10.f));    // En haut à gauche
    m_textFps.setString("FPS: 0");                      // Affichage FPS

    // Configuration du texte d'information
    m_textInfo.setCharacterSize(16);                    // Taille en pixels
    m_textInfo.setFillColor(sf::Color::White);          // Couleur blanc
    m_textInfo.setPosition(sf::Vector2f(10.f, 40.f));   // Juste en dessous des FPS
    m_textInfo.setString("Simulation: Prete");          // Etat de la simulation

    return true; // Succès
}

/**
 * @brief Mise à jour du texte des FPS.
 */
void Hud::update(float fps) {
    // On convertit le float en int pour éviter les virgules (Sinon sa spam d'erreurs)
    m_textFps.setString("FPS: " + std::to_string(static_cast<int>(fps)));
}

/**
 * @brief Affichage des éléments.
 */
void Hud::draw(sf::RenderWindow& window) {
    // On dessine les textes sur la fenêtre donnée en paramètre
    window.draw(m_textFps);
    window.draw(m_textInfo);
}

/**
 * @brief Mise à jour des dimensions du HUD.
 */
void Hud::onResize(sf::Vector2u newSize) {
    // On garde la largeur fixe (m_width), mais on met à jour la hauteur
    // correspond à la nouvelle hauteur de la fenêtre.
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(newSize.y)));
}