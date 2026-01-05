/**
 * @file Hud.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de la classe Hud (Heads-Up Display).
 * @details Gère l'affichage de l'interface utilisateur latérale :
 * Textes, Statistiques (Vivants, Morts, Naissances), FPS et Titres.
 * @version 0.5
 * @date 2026-01-05
 */

#pragma once

// Inclusion de la bibliothèque graphique SFML
#include <SFML/Graphics.hpp>
#include <iostream>

/**
 * @class Hud
 * @brief Classe gérant la partie Interface Utilisateur (Menu de gauche).
 */
class Hud {
public:
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------

    /**
     * @brief Constructeur par défaut.
     * @details Initialise les variables membres (comme la largeur du menu).
     * Note : SFML 3 demande que les sf::Text soient initialisés avec une font dans le constructeur.
     */
    Hud();
    
    // -------------------------------------------------------------------------
    // MÉTHODES PRINCIPALES
    // -------------------------------------------------------------------------

    /**
     * @brief Charge les ressources et configure l'apparence du HUD.
     * @details Charge la police d'écriture (font.ttf ou arial.ttf) et positionne
     * les textes et le fond gris sur l'écran.
     * @param windowSize La taille actuelle de la fenêtre (pour adapter la hauteur du fond).
     * @return true si l'initialisation a réussi, false sinon (ex: police introuvable).
     */
    bool init(sf::Vector2u windowSize);

    /**
     * @brief Met à jour les données affichées à l'écran.
     * @details Cette fonction reçoit toutes les statistiques du jeu pour actualiser le texte.
     * * @param fps Le nombre d'images par seconde (Performance).
     * @param grass Nombre d'herbes vivantes.
     * @param sheep Nombre de moutons vivants.
     * @param wolves Nombre de loups vivants.
     * @param deadS Nombre total de moutons morts (Cimetière).
     * @param deadW Nombre total de loups morts (Cimetière).
     * @param bornS Nombre total de naissances de moutons.
     * @param bornW Nombre total de naissances de loups.
     */   
    void update(float fps, int grass, int sheep, int wolves, int deadS, int deadW, int bornS, int bornW);

    /**
     * @brief Dessine l'interface complète sur la fenêtre.
     * @details Affiche le fond, puis les textes par-dessus.
     * @param window La fenêtre de rendu SFML.
     */
    void draw(sf::RenderWindow& window);

    // -------------------------------------------------------------------------
    // UTILITAIRES & GETTERS
    // -------------------------------------------------------------------------

    /**
     * @brief Récupère la largeur fixe du menu.
     * @details Utilisé par le Renderer pour savoir où commencer à dessiner la zone de jeu.
     * @return float La largeur en pixels.
     */
    float getWidth() const { return m_width; }

    /**
     * @brief Adapte le HUD lors du redimensionnement de la fenêtre.
     * @details Étire le fond gris pour qu'il touche toujours le bas de la fenêtre.
     * @param newSize La nouvelle taille de la fenêtre.
     */
    void onResize(sf::Vector2u newSize);

private:
    // -------------------------------------------------------------------------
    // MEMBRES PRIVÉS
    // -------------------------------------------------------------------------

    // --- Ressources ---
    sf::Font m_font;        ///< La police d'écriture (chargée depuis un fichier .ttf).

    // --- Éléments Textuels ---
    sf::Text m_textFps;     ///< Affiche les frames par seconde (en haut à gauche).
    sf::Text m_textTitle;   ///< Le titre "STATISTIQUES".
    sf::Text m_textInfo;    ///< Le bloc de texte principal contenant toutes les stats.

    // --- Éléments Graphiques ---
    sf::RectangleShape m_background; ///< Le rectangle gris vertical (fond du menu).
    sf::RectangleShape m_separator;  ///< La petite barre blanche sous le titre.

    // --- Propriétés ---
    float m_width;          ///< Largeur du menu en pixels (fixe).
};