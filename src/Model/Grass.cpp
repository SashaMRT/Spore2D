/**
 * @file Grass.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Grass (Nourriture).
 * @details Représente une entité statique (plante) qui sert de nourriture aux moutons.
 * Elle possède un rendu visuel légèrement détaillé (3 cercles pour faire une touffe).
 * @version 0.4
 * @date 2026-01-05
 */

// Bibliothèques utilisées
#include "../../include/Model/Grass.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Construit une nouvelle entité Herbe.
 * @details Initialise la forme graphique de base (cercle vert).
 * @param position La position (x, y) où l'herbe pousse.
 */
Grass::Grass(sf::Vector2f position) : pos(position), alive(true) {
    // Configuration de la forme principale (Cœur de la plante)
    shape.setRadius(4.f);
    
    // Centrage de l'origine (requis par SFML 3 pour les rotations/positions correctes)
    shape.setOrigin(sf::Vector2f(4.f, 4.f));
    
    // Couleur verte standard
    shape.setFillColor(sf::Color(50, 200, 50)); 
    shape.setPosition(pos);
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

/**
 * @brief Met à jour l'état de l'herbe.
 * @details Cette fonction est quasi-vide car l'herbe est statique.
 * Elle ne bouge pas et ne vieillit pas (dans cette version simple).
 * @param dt Temps écoulé (Delta Time).
 */
void Grass::update(float dt) {
    // L'herbe ne fait rien de spécial, elle attend d'être mangée par un mouton.
    // On conserve cette fonction pour respecter l'interface commune des entités.
}

// -------------------------------------------------------------------------
// AFFICHAGE (DRAW)
// -------------------------------------------------------------------------

/**
 * @brief Affiche l'herbe avec des détails décoratifs.
 * @details Pour rendre le jeu plus joli, on dessine 3 cercles au lieu d'un seul :
 * 1. La tige centrale.
 * 2. Une petite feuille à gauche (plus foncée).
 * 3. Une petite feuille à droite (plus claire).
 * @param window La fenêtre SFML cible.
 */
void Grass::draw(sf::RenderWindow& window) {
    if (!alive) return; // Si mangée, on ne dessine rien

    // 1. On dessine la feuille centrale (Base)
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(50, 200, 50)); 
    window.draw(shape);

    // 2. On dessine une petite feuille à gauche (Déco)
    // On copie la forme principale pour garder les propriétés
    sf::CircleShape leafLeft = shape;
    leafLeft.setRadius(3.f);                   // Plus petite
    leafLeft.setOrigin(sf::Vector2f(3.f, 3.f)); // Recentrage
    leafLeft.setPosition(sf::Vector2f(pos.x - 4.f, pos.y + 2.f)); // Décalage gauche
    leafLeft.setFillColor(sf::Color(30, 180, 30)); // Vert plus foncé (ombre)
    window.draw(leafLeft);

    // 3. On dessine une petite feuille à droite (Déco)
    sf::CircleShape leafRight = shape;
    leafRight.setRadius(3.f);
    leafRight.setOrigin(sf::Vector2f(3.f, 3.f));
    leafRight.setPosition(sf::Vector2f(pos.x + 4.f, pos.y + 2.f)); // Décalage droite
    leafRight.setFillColor(sf::Color(70, 220, 70)); // Vert plus clair (lumière)
    window.draw(leafRight);
}