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
     * Charge la police d'écriture et configure les textes.
     * @return true (succés sinon false).
     */
    bool init();

    /**
     * @brief Met à jour les données affichées.
     * @param fps Le nombre d'images par seconde (calculé dans Application).
     */    
    void update(float fps);

    /**
     * @brief Dessine l'interface sur la fenêtre.
     * @param window Reprend la bibliothèque de rendu SFML.
     * pour le rendu de la fenêtre.
     */
    void draw(sf::RenderWindow& window);

// Accessible seulement par les méthodes de cette classe.
private:
    // La police d'écriture (sinon bug sur Windows).
    sf::Font m_font;
    // Le texte affichant les FPS.
    sf::Text m_textFps;
    // Le texte affichant les infos.
    sf::Text m_textInfo;
};