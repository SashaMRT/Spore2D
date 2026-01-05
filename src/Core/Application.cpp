/**
 * @file Application.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation du moteur de jeu (Classe Principale).
 * @details C'est le chef d'orchestre du programme. Il gère :
 * - La fenêtre et les événements (Clavier, Souris).
 * - La boucle principale (Update / Render).
 * - La gestion du temps (Delta Time).
 * - La coordination entre le HUD, le Renderer et la Simulation.
 * @version 0.5
 * @date 2026-01-05
 */

// Bibliothèques standard
#include <iostream> 

// Inclusions internes
#include "../../include/Core/Application.hpp"
#include "../../include/Model/Simulation.hpp"

// Variable globale pour la gestion du temps
// Définie ici pour être accessible dans la boucle run()
sf::Clock clockFps;

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Initialise l'application, la fenêtre et les sous-systèmes.
 * @param testMode Si true, l'application se ferme automatiquement après quelques frames (pour les tests auto).
 */
Application::Application(bool testMode) : m_isTestMode(testMode), m_isPaused(false) {
    std::cout << "[Application] Initialisation..." << std::endl;

    // CRÉATION DE LA FENÊTRE
    // On récupère la résolution de l'écran de l'utilisateur pour lancer en "Plein Écran fenêtré"
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    m_window.create(desktopMode, "Spore2D Simulation", sf::Style::Default);
    
    // On limite à 60 FPS pour ne pas surchauffer le processeur inutilement
    m_window.setFramerateLimit(60);

    // INITIALISATION DU HUD (Interface Utilisateur)
    if (!m_hud.init(m_window.getSize())) {
        std::cerr << "Attention: Problème chargement HUD (Police introuvable ?)" << std::endl;
    }

    // INITIALISATION DU RENDERER (Affichage du jeu)
    // On lui donne la taille de la fenêtre et la largeur du menu pour qu'il calcule la zone noire.
    m_renderer.init(m_window.getSize(), m_hud.getWidth());
    
    // DÉFINITION DES LIMITES DU MONDE (Physique)
    // On dit à la Simulation (Model) où sont les murs.
    float gap = 5.f; // Petite marge de sécurité
    // X min = Largeur du menu + marge
    // X max = Largeur fenêtre - marge
    setWorldBounds(m_hud.getWidth() + gap, (float)desktopMode.size.x - gap, gap, (float)desktopMode.size.y - 10.f);

    std::cout << "[Application] Prêt." << std::endl;
}

// -------------------------------------------------------------------------
// BOUCLE PRINCIPALE (RUN)
// -------------------------------------------------------------------------

/**
 * @brief Lance la boucle de jeu infinie.
 * @details Ne s'arrête que si l'utilisateur ferme la fenêtre ou appuie sur Echap.
 */
void Application::run() {
    int frameCount = 0;             // Compteur pour le mode test
    bool gameOverTriggered = false; // Flag pour ne pas spammer le message de fin dans la console

    // Tant que la fenêtre est ouverte...
    while (m_window.isOpen()) {
        
        // --- GESTION DU TEMPS (Delta Time) ---
        // restart() renvoie le temps écoulé depuis le dernier tour de boucle.
        // C'est CRUCIAL pour que le jeu tourne à la même vitesse sur tous les PC.
        float currentTime = clockFps.restart().asSeconds();
        float fps = 1.0f / currentTime; // Calcul approximatif des FPS pour l'affichage

        // --- GESTION DES ÉVÉNEMENTS (Inputs) ---
        while (const auto event = m_window.pollEvent()) {
            
            // Clic sur la croix rouge
            if (event->is<sf::Event::Closed>()) m_window.close();

            // Gestion du Clavier
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                
                // [Echap] -> Quitter
                if (keyEvent->code == sf::Keyboard::Key::Escape) m_window.close();
                
                // [P] -> Pause manuelle
                if (keyEvent->code == sf::Keyboard::Key::P) {
                    m_isPaused = !m_isPaused;
                    // Si on relance le jeu manuellement, on reset le Game Over pour pouvoir continuer
                    if (!m_isPaused) gameOverTriggered = false; 
                }
                
                // [R] -> Reset (Relancer la simulation)
                if (keyEvent->code == sf::Keyboard::Key::R) {
                    initEcosystem();    // Fonction globale de Simulation.cpp
                    gameOverTriggered = false;
                    m_isPaused = false; // On enlève la pause automatiquement
                    std::cout << ">> RESET COMPLET" << std::endl;
                }
            }

            // Gestion du Redimensionnement de la fenêtre
            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()) {
                sf::Vector2u newSize = resizeEvent->size;
                
                // Mise à jour de la Vue SFML (pour éviter que l'image ne s'étire/déforme)
                sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)newSize.x, (float)newSize.y));
                m_window.setView(sf::View(visibleArea));
                
                // Prévenir les composants visuels
                m_hud.onResize(newSize);
                m_renderer.onResize(newSize, m_hud.getWidth());
                
                // Prévenir la Simulation (Physique) que les murs ont bougé
                float gap = 5.f;
                setWorldBounds(m_hud.getWidth() + gap, (float)newSize.x - gap, gap, (float)newSize.y - 10.f);
            }
        }

        // Mode Test (Github Actions) : Ferme l'appli après 30 images
        if (m_isTestMode) { frameCount++; if (frameCount > 30) m_window.close(); }


        // --- LOGIQUE MÉTIER (UPDATE) ---
        
        // On récupère les stats pour vérifier les conditions de victoire/défaite
        EcosystemStats stats = getEcosystemStats();

        // CONDITION DE DÉFAITE (Game Over)
        // Si tous les moutons sont morts, mais qu'il reste des loups.
        if (stats.sheep == 0 && stats.wolves > 0 && !gameOverTriggered) {
            m_isPaused = true;       // On gèle le jeu
            gameOverTriggered = true;// On note que c'est fini
            std::cout << "=== FIN DE SIMULATION : TOUS LES MOUTONS SONT MORTS ===" << std::endl;
            std::cout << "Appuyez sur [R] pour relancer une simulation." << std::endl;
        }

        // Si le jeu n'est pas en pause, on fait avancer la simulation
        if (!m_isPaused) {
            ecosystemUpdate(currentTime); // Appel au Modèle (Simulation.cpp)
        }


        // --- AFFICHAGE (RENDER) ---
        
        // On récupère les stats une dernière fois (au cas où elles aient changé pendant l'update)
        stats = getEcosystemStats(); 
        
        // Mise à jour des textes du HUD
        m_hud.update(fps, stats.grass, stats.sheep, stats.wolves, stats.deadSheep, stats.deadWolves, stats.bornSheep, stats.bornWolves);

        // Dessin
        m_window.clear(sf::Color(30, 30, 30)); // Effacer (Gris foncé)
        m_renderer.draw(m_window);             // Dessiner le jeu (Carré noir + Animaux)
        m_hud.draw(m_window);                  // Dessiner l'interface par-dessus
        m_window.display();                    // Afficher à l'écran
    }
}