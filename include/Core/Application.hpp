/**
 * @file Application.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @brief Déclaration de la classe Application : le schéma du moteur de jeu.
 * @details Définit la structure de l'application (les méthodes nécessaires)
 * sans contenir le code (voir Application.cpp). C'est le "Corps à vide" du programme.
 * @version 0.1
 * @date 2025-12-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// Empêche d'inclure ce fichier deux fois.
#pragma once

// Inclusion de notre bibliothèque SFML.
// Elle permet l'utilisation de sf
#include <SFML/Graphics.hpp>

// Inclusion de notre bibliothèque Hud.
// Elle permet l'utilisation de la classe Hud et de ces méthodes.
#include "../View/Hud.hpp"

/**
 * @class Application
 * @brief Le moteur de jeu. 
 * Il gère la fenêtre et la boucle principale.
 * 
 */
class Application {
// Accessible depuis l'extérieur (par le main.cpp par exemple).
public:
    // Le Constructeur : Il porte TOUJOURS le même nom que la classe.
    // C'est la fonction appelée automatiquement à la création de l'objet.
    // Dans notre cas, c'est la fonction qui défini le terrain.
    Application();

    // La méthode pour lancer la boucle de jeu.
    // Retourne rien car de type "void".
    void run();

// Accessible seulement par les méthodes de cette classe.
private:
    // La fenêtre principale du jeu
    sf::RenderWindow m_window;

    // Information sur le jeu
    Hud m_hud;
};