/**
 * @file Hud.hpp
 * @author Sasha Marie te Rehorst
 * @author Gael Guinaliu
 * @brief Déclaration de la classe Hud avec support pour Agneaux et Louveteaux.
 * @version 1.0
 * @date 2026-01-07
 */

#pragma once

#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

/**
 * @class Hud
 * @brief Gère l'affichage des informations et statistiques à l'écran.
 */
class Hud {
public:
    /**
     * @brief Constructeur par défaut.
     */
    Hud();

    /**
     * @brief Initialise les ressources graphiques du HUD.
     * @param windowSize Taille de la fenêtre.
     * @return true si l'initialisation réussit.
     */
    bool init(sf::Vector2u windowSize);

    /**
     * @brief Met à jour les textes affichés.
     * @note Signature mise à jour pour inclure lambs et pups (10 arguments total).
     */
    void update(float fps, int grass, int sheep, int lambs, int wolves, int pups, 
                int deadS, int deadW, int bornS, int bornW);

    /**
     * @brief Dessine le HUD sur la fenêtre.
     * @param window Fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Repositionne les éléments lors d'un redimensionnement.
     * @param newSize Nouvelle taille de la fenêtre.
     */
    void onResize(sf::Vector2u newSize);

    /**
     * @brief Récupère la largeur du panneau latéral.
     * @return float Largeur en pixels.
     */
    float getWidth() const { return m_width; }

private:
    sf::Font m_font;
    sf::RectangleShape m_background;
    sf::RectangleShape m_separator;
    sf::Text m_textFps;
    sf::Text m_textTitle;
    sf::Text m_textInfo;
    float m_width;
};

#endif