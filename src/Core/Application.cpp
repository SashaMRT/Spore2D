/**
 * @file Application.cpp
 * @author sasha.marieterehorst (sasha.marieterehorst@gmail.com)
 * @brief Implémentation de la classe Application : La représentation du moteur du jeu.
 * @details Ce fichier contient le code exécutable du moteur.
 * C'est ici que la fenêtre est créée et que la boucle de jeu tourne.
 * C'est aussi ce qui compose le corps (Voir Application.hpp).
 * @version 0.1
 * @date 2025-12-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// Inclusion de la bibliothèque std
#include <iostream> 

// Inclusion de notre bibliothèque Application.
// Elle permet l'utilisation de la classe Applications et de ces méthodes.
#include "../../include/Core/Application.hpp"


// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Application.
 * C'est ici que l'on initialise les variables et que l'on crée la fenêtre.
 */
Application::Application() {
    // Debuggage (On suit l'éxécution du programme)
    std::cout << "[Application] Initialisation en cours..." << std::endl;

    // TODO (À faire plus tard)
}

// -------------------------------------------------------------------------
// MÉTHODES PUBLIQUES
// -------------------------------------------------------------------------

/**
 * @brief Methode run.
 * Cette fonction ne se termine que lorsque l'utilisateur ferme l'application.
 * @details Retourne rien car de type "void".
 *
 */
void Application::run() {

    // Simulation d'une boucle.

    // Étape 0 : Initialisation de la boucle (Application::run()).
    std::cout << "[Application] Lancement de la boucle principale (run)." << std::endl;
    
    // Étape 1 : Gestion des événements (Input).
    std::cout << "   -> [Input]  Vérification clavier/souris..." << std::endl;

    // Étape 2 : Mise à jour de la logique (Update).
    std::cout << "   -> [Update] Déplacement des entités (Loups, Moutons)..." << std::endl;

    // Étape 3 : Affichage (Render).
    std::cout << "   -> [Render] Dessin sur l'écran..." << std::endl;

    // Étape 4 : Fin de la boucle (void donc pas de return s'arrête dès la fin du message).
    std::cout << "[Application] Fin de la boucle. Fermeture du programme." << std::endl;
}