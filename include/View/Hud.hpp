/**
 * @file Hud.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @brief Déclaration de la classe Hud : le schéma du gestionnaire d'informations.
 * @details Gère l'affichage de l'interface utilisateur (Textes, FPS, Infos).
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

// Inclusion de la bibliothèque std.
#include <iostream>

/**
 * @class Hud
 * @brief Interface gérant les information durant la simulation.
 * 
 */
class Hud {
// Accessible depuis l'extérieur (par le main.cpp par exemple).
public:
    // Le Constructeur : Il porte TOUJOURS le même nom que la classe.
    // C'est la fonction appelée automatiquement à la création de l'objet.
    // Dans notre cas, c'est la fonction qui défini les informations.
    Hud();
    
    /**
     * @brief Initialise les ressources du HUD.
     * @param windowSize La taille totale de la fenêtre (pour calculer la hauteur).
     * Charge la police d'écriture et configure les textes.
     * @return true (succés sinon false).
     */
    bool init(sf::Vector2u windowSize);

    /**
     * @brief Met à jour les données affichées (FPS + Entités).
     */   
    void update(float fps, int grass, int sheep, int wolves);

    /**
     * @brief Dessine l'interface sur la fenêtre.
     * @param window Reprend la bibliothèque de rendu SFML.
     * pour le rendu de la fenêtre.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Récupère la largeur du menu.
     * @return float La largeur en pixels.
     * Retourne la largeur du menu pour éviter que le jeu ne soit caché par l'interface.
     */
    float getWidth() const { return m_width; }

    /**
     * @brief Recalcule la taille de la zone de jeu
     * lors d'un redimensionnement de fenêtre.
     * @param newSize La nouvelle taille de la fenêtre.
     */
    void onResize(sf::Vector2u newSize);

// Accessible seulement par les méthodes de cette classe.
private:
    // La police d'écriture (sinon bug sur Windows).
    sf::Font m_font;
    // Le texte affichant les FPS.
    sf::Text m_textFps;
    // Le titre.
    sf::Text m_textTitle;
    // Les stats.
    sf::Text m_textInfo;
    // Le rendu du menu.
    sf::RectangleShape m_background;
    // La barre de soulignement.
    sf::RectangleShape m_separator;
    // La largeur définie du menu.
    float m_width;
};