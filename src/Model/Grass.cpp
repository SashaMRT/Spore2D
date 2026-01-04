/**
 * @file Grass.cpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Grass.
 * @version 0.2
 * @date 2026-01-04
 *
 * @copyright Copyright (c) 2026
 *
 */

// Bibliothèque utilisées
#include "../../include/Model/Grass.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Grass::Grass(sf::Vector2f position) : pos(position), alive(true) {
    // On configure la forme principale
    shape.setRadius(4.f);
    
    // SFML 3 oblige l'utilisation de vecteur à l'origine
    shape.setOrigin(sf::Vector2f(4.f, 4.f));
    
    shape.setFillColor(sf::Color(50, 200, 50)); // Vert
    shape.setPosition(pos);
}

// -------------------------------------------------------------------------
// MISE À JOUR
// -------------------------------------------------------------------------

void Grass::update(float dt) {
    // L'herbe ne fait rien de spécial, elle attend d'être mangée.
    // On garde la fonction pour respecter la structure du projet.
}

// -------------------------------------------------------------------------
// AFFICHAGE
// -------------------------------------------------------------------------

void Grass::draw(sf::RenderWindow& window) {
    if (!alive) return;

    // On dessine la feuille centrale
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(50, 200, 50)); 
    window.draw(shape);

    // On dessine une petite feuille à gauche (déco)
    sf::CircleShape leafLeft = shape;
    leafLeft.setRadius(3.f);
    leafLeft.setOrigin(sf::Vector2f(3.f, 3.f));
    leafLeft.setPosition(sf::Vector2f(pos.x - 4.f, pos.y + 2.f));
    leafLeft.setFillColor(sf::Color(30, 180, 30)); // Un peu plus foncé
    window.draw(leafLeft);

    // On dessine une petite feuille à droite (déco)
    sf::CircleShape leafRight = shape;
    leafRight.setRadius(3.f);
    leafRight.setOrigin(sf::Vector2f(3.f, 3.f));
    leafRight.setPosition(sf::Vector2f(pos.x + 4.f, pos.y + 2.f));
    leafRight.setFillColor(sf::Color(70, 220, 70)); // Un peu plus clair
    window.draw(leafRight);
}