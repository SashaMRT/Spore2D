/**
 * @file Renderer.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Gestion de l'affichage du terrain de jeu (Zone de simulation).
 * @details S'occupe uniquement du rendu graphique de la zone noire et des entités (Moutons, Loups, Herbe).
 * Le texte et les statistiques sont désormais gérés par la classe Hud.
 * @version 0.3
 * @date 2026-01-05
 */

#include "../../include/View/Renderer.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "../../include/Model/Simulation.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur par défaut de l'objet Renderer.
 * @details Initialise une instance vide. Les ressources sont chargées via init().
 */
Renderer::Renderer() {
    // Plus besoin d'initialiser de texte ici, le HUD s'en charge.
}

// -------------------------------------------------------------------------
// INITIALISATION
// -------------------------------------------------------------------------

/**
 * @brief Prépare la zone de jeu et lance la simulation.
 * @details Cette fonction configure l'aléatoire, calcule la taille du rectangle de jeu
 * et lance la création initiale des animaux.
 * * @param windowSize La taille totale de la fenêtre pour le calcul des dimensions.
 * @param hudWidth La largeur du menu (HUD) pour décaler la zone de jeu.
 */
void Renderer::init(sf::Vector2u windowSize, float hudWidth) {
    // 1. Initialisation de l'aléatoire
    // Indispensable pour que les positions des animaux soient différentes à chaque lancement.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 2. Calcul de la Géométrie
    // On appelle directement onResize pour éviter de dupliquer le code de calcul des dimensions.
    onResize(windowSize, hudWidth);

    // 3. Application du Style visuel
    m_gameArea.setFillColor(sf::Color::Black);            // Fond noir pour le terrain
    m_gameArea.setOutlineThickness(-2.f);                 // Bordure intérieure (pour ne pas dépasser)
    m_gameArea.setOutlineColor(sf::Color(100, 100, 100)); // Gris clair pour délimiter

    // 4. Lancement de la Simulation (Modèle)
    // Crée les premiers moutons, loups et herbes.
    initEcosystem();

    std::cout << "[Renderer] Initialise (Mode graphique pur, sans overlay texte)." << std::endl;
}

// -------------------------------------------------------------------------
// RENDU (DRAW)
// -------------------------------------------------------------------------

/**
 * @brief Dessine la scène de jeu dans la fenêtre.
 * @details Affiche d'abord le fond noir, puis appelle le modèle pour dessiner
 * les entités par-dessus.
 * * @param window La fenêtre SFML cible.
 */
void Renderer::draw(sf::RenderWindow& window) {
    // 1. On dessine le fond noir (la zone délimitée)
    window.draw(m_gameArea);

    // 2. On dessine les entités vivantes (Moutons, Loups, Herbe)
    // Cette fonction provient de Simulation.cpp
    ecosystemDraw(window);

    // NOTE : L'affichage des textes (stats) a été retiré d'ici pour être géré
    // exclusivement par le HUD afin d'éviter les doublons et nettoyer le code.
}

// -------------------------------------------------------------------------
// GESTION DU REDIMENSIONNEMENT
// -------------------------------------------------------------------------

/**
 * @brief Recalcule les dimensions de la zone de jeu.
 * @details Appelée à l'initialisation et à chaque fois que l'utilisateur redimensionne la fenêtre.
 * Elle assure que le jeu reste à droite du menu avec des marges correctes.
 * * @param newSize La nouvelle taille de la fenêtre.
 * @param hudWidth La largeur actuelle du HUD (Menu latéral).
 */
void Renderer::onResize(sf::Vector2u newSize, float hudWidth) {
    // Définition des marges (espace vide autour du jeu)
    float gap = 5.f;         // Marge Haut / Gauche / Droite
    float gapBottom = 10.f;  // Marge du bas un peu plus grande

    // Calcul de la position de départ (Coin Haut-Gauche)
    // On se décale de la largeur du menu + la marge.
    float posX = hudWidth + gap;
    float posY = gap;

    // Calcul de la taille du rectangle
    // Largeur = Tout l'écran - (Menu + Marge Gauche) - Marge Droite
    float rectWidth = static_cast<float>(newSize.x) - posX - gap;
    // Hauteur = Tout l'écran - Marge Haut - Marge Bas
    float rectHeight = static_cast<float>(newSize.y) - posY - gapBottom;

    // Sécurité : On évite les plantages si la fenêtre est réduite à une taille minuscule
    if (rectWidth < 0) rectWidth = 100.f;
    if (rectHeight < 0) rectHeight = 100.f;

    // Application des nouvelles dimensions au rectangle SFML
    m_gameArea.setSize(sf::Vector2f(rectWidth, rectHeight));
    m_gameArea.setPosition(sf::Vector2f(posX, posY));
}