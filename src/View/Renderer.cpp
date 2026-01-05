/**
 * @file Renderer.cpp
 * @brief Gestion de l'affichage du terrain de jeu (Version Nettoyée)
 */

#include "../../include/View/Renderer.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "../../include/Model/Simulation.hpp"

Renderer::Renderer() {
    // Plus de texte à initialiser ici
}

void Renderer::init(sf::Vector2u windowSize, float hudWidth) {
    // 1. Aléatoire
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 2. Géométrie (Zone de jeu)
    onResize(windowSize, hudWidth);

    // Style de la zone de jeu
    m_gameArea.setFillColor(sf::Color::Black);           
    m_gameArea.setOutlineThickness(-2.f); 
    m_gameArea.setOutlineColor(sf::Color(100, 100, 100)); 

    // 3. Lancement Simulation
    initEcosystem();

    std::cout << "[Renderer] Initialise (Overlay texte supprime)." << std::endl;
}

void Renderer::draw(sf::RenderWindow& window) {
    // 1. Fond noir
    window.draw(m_gameArea);

    // 2. Dessin des entités (Moutons, Loups, Herbe)
    ecosystemDraw(window);

    // NOTE : On a supprimé le bloc de texte "m_statsText" qui faisait doublon.
    // Désormais, seul le HUD à gauche affiche les infos.
}

void Renderer::onResize(sf::Vector2u newSize, float hudWidth) {
    float gap = 5.f;
    float gapBottom = 10.f;

    float posX = hudWidth + gap;
    float posY = gap;

    float rectWidth = static_cast<float>(newSize.x) - posX - gap;
    float rectHeight = static_cast<float>(newSize.y) - posY - gapBottom;

    if (rectWidth < 0) rectWidth = 100.f;
    if (rectHeight < 0) rectHeight = 100.f;

    m_gameArea.setSize(sf::Vector2f(rectWidth, rectHeight));
    m_gameArea.setPosition(sf::Vector2f(posX, posY));
}