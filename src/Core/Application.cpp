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

// Inclusion de la bibliothèque std.
#include <iostream> 

// Inclusion de notre bibliothèque Application.
// Elle permet l'utilisation de la classe Applications et de ces méthodes.
#include "../../include/Core/Application.hpp"

// Horloge pour les FPS
sf::Clock clockFps;

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Application.
 * C'est ici que l'on initialise les variables et que l'on crée la fenêtre.
 */
Application::Application() {
    // Debuggage (On suit l'éxécution du programme).
    std::cout << "[Application] Initialisation en cours..." << std::endl;

    // On récupère la résolution de l'écran de l'utilisateur.
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // On crée ici la fenêtre avec la taille definit ci-dessus.
    m_window.create(desktopMode, "Spore2D Simulation");

    // Limitation de FPS pour éviter la surcharge CPU.
    m_window.setFramerateLimit(60);

    // On initialise le HUD au démarrage.
    if (!m_hud.init()) {
        std::cerr << "Erreur init HUD" << std::endl;
    }

    // Debuggage (On suit la création de la fenêtre).
    std::cout << "[Application] Fenêtre créée en " << desktopMode.size.x << "x" << desktopMode.size.y << std::endl;
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

    // Initialisation de la boucle (Application::run()).
    std::cout << "[Application] Lancement de la boucle principale (run)." << std::endl;

    // Tant que la fenêtre est ouverte.
    while (m_window.isOpen()) {

        // Calcul du temps écoulé (pour les FPS).
        float currentTime = clockFps.restart().asSeconds();
        float fps = 1.0f / currentTime;

        // Gestion des événements (Input).
        while (const auto event = m_window.pollEvent()) {
            // Si la croix de fermeture est pressée.
            if (event->is<sf::Event::Closed>()) {
                m_window.close(); // On ferme la fenêtre, ce qui arrête la boucle while.
            }

            // Fermer avec Echap (Bug Hyprland).
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    m_window.close();
                }
            }
        }

        // Mise à jour de la logique (Update).
        m_hud.update(fps);

        // Affichage (Render).
        // On efface l'image précédente (Bug graphique sinon).
        m_window.clear(sf::Color::Black);

        // On affiche le HUD à l'écran.
        m_hud.draw(m_window);

        // On affiche à l'écran.
        m_window.display();
    }

    // Fin de la boucle (void donc pas de return s'arrête dès la fin du message).
    std::cout << "[Application] Fin de la boucle. Fermeture du programme." << std::endl;
}