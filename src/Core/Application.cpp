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

// Inclusion de notre bibliothèque Simulation.
// Elle permet l'utilisation de la classe Simulation et de ces méthodes.
#include "../../include/Model/Simulation.hpp"

// Horloge pour les FPS
sf::Clock clockFps;

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Application.
 * C'est ici que l'on initialise les variables et que l'on crée la fenêtre.
 */
Application::Application(bool testMode) : m_isTestMode(testMode) {
    // Debuggage (On suit l'éxécution du programme).
    std::cout << "[Application] Initialisation en cours..." << std::endl;

    // On récupère la résolution de l'écran de l'utilisateur.
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // On crée ici la fenêtre avec la taille definit ci-dessus.
    m_window.create(desktopMode, "Spore2D Simulation");

    // Limitation de FPS pour éviter la surcharge CPU.
    m_window.setFramerateLimit(60);

    // On initialise le HUD au démarrage.
    if (!m_hud.init(m_window.getSize())) {
        std::cerr << "Erreur init HUD" << std::endl;
    }

    // On initialise la zone de jeu en lui donnant la largeur du menu pour mettre le terrain à droite.
    m_renderer.init(m_window.getSize(), m_hud.getWidth());

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

    // Compteur pour le test
    int frameCount = 0;

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
            // Redimensionnement
            // Lancer quand l'utilisateur tire sur les bords de la fenêtre.
            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()) {
                
                // Récupération de la nouvelle taille
                sf::Vector2u newSize = resizeEvent->size;

                // On définit un vecteur qui part de (0,0) et au autre qui fait la taille de la fenêtre
                // C'est notre "nouvelle vue".
                sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)newSize.x, (float)newSize.y));

                // On applique cette nouvelle vue à la fenêtre.
                // Maintenant, 1 pixel du jeu = 1 pixel de l'écran. Pas d'étirement.
                m_window.setView(sf::View(visibleArea));

                // Donne l'info aux composants
                // On prévient le HUD qu'il doit s'allonger
                m_hud.onResize(newSize);
                
                // On prévient le Renderer qu'il a plus (ou moins) de place
                m_renderer.onResize(newSize, m_hud.getWidth());
            }
        }

        // Fix pour github action
        if (m_isTestMode) {
            frameCount++;
            // Après 30 "tours" (environ 0.5 seconde), on ferme tout proprement.
            if (frameCount > 30) {
                m_window.close(); 
            }
        }

        // Mise à jour de la logique (Update).
        EcosystemStats stats = getEcosystemStats(); // On récupére les informations
        m_hud.update(fps, stats.grass, stats.sheep, stats.wolves, stats.deadSheep, stats.deadWolves, stats.bornSheep, stats.bornWolves);

        // Affichage (Render).
        // On efface l'image précédente (Bug graphique sinon).
        m_window.clear(sf::Color(30, 30, 30)); // Gris foncé en fond

        // On dessine le terrain de jeu (le fond noir).
        m_renderer.draw(m_window);

        // On affiche le HUD à l'écran.
        m_hud.draw(m_window);

        // On affiche à l'écran.
        m_window.display();
    }

    // Fin de la boucle (void donc pas de return s'arrête dès la fin du message).
    std::cout << "[Application] Fin de la boucle. Fermeture du programme." << std::endl;
}