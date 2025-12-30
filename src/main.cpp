/**
 * @file main.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @brief Point d'entrée du programme Spore2D.
 * @details Instancie les objets et lance le moteur.
 * @version 0.1
 * @date 2025-12-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// Inclusion de la bibliothèque std.
#include <iostream>

// Inclusion de notre bibliothèque Application.
// Elle permet l'utilisation de la classe Applications et de ces méthodes.
#include "../include/Core/Application.hpp"

/**
 * @brief Fonction principale.
 * @return int 
 * @details Code de sortie (0 = Succès)
 */
int main(int, char**){
    // Démarrage de la simulation + Message Debuggage.
    std::cout << "Lancement de la simulation..." << std::endl;

    // Création de l'objet Application.
    // On reprend la classe depuis le header.
    Application app;

    // Lancement du moteur du jeu.
    // On appelle la méthode "run" de la classe "Application"
    // via l'objet créé juste au-dessus.
    // Parcours : Objet (app) -> Classe (Application) -> Méthode (run).
    app.run();

    // Fin de la simulation + Message Debuggage.
    std::cout << "Fin du programme." << std::endl;

    // On retourne 0 (car int).
    // 0 correspond au succès de l'éxecution de la fonction.
    return 0;
}
