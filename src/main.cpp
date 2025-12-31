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
 * @brief Fonction principale du programme.
 * * @param argc Le nombre d'arguments passés au programme.
 * @param argv Le tableau des chaînes de caractères contenant les arguments.
 * argv[0] est le nom du programme, argv[1] le premier argument, etc.
 * @return int (0 = Succès, 1 = Echec).
 */
int main(int argc, char* argv[]){

    // Mode test
    bool testMode = false;

    // Si on a un argument et que c'est "--test"
    if (argc > 1 && std::string(argv[1]) == "--test") {
        testMode = true;
    }

    // Démarrage de la simulation + Message Debuggage.
    std::cout << "Lancement de la simulation..." << std::endl;

    // Création de l'objet Application.
    // On reprend la classe depuis le header.
    // Et en paramètre si c'est en test mode ou non
    Application app(testMode);

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
