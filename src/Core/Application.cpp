/**
 * @file Application.cpp
 * @brief Implémentation du moteur de jeu (Avec Game Over)
 */

#include <iostream> 
#include "../../include/Core/Application.hpp"
#include "../../include/Model/Simulation.hpp"

sf::Clock clockFps;

Application::Application(bool testMode) : m_isTestMode(testMode), m_isPaused(false) {
    std::cout << "[Application] Initialisation..." << std::endl;

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    m_window.create(desktopMode, "Spore2D Simulation", sf::Style::Default);
    m_window.setFramerateLimit(60);

    if (!m_hud.init(m_window.getSize())) {
        std::cerr << "Attention: Problème chargement HUD" << std::endl;
    }

    m_renderer.init(m_window.getSize(), m_hud.getWidth());
    
    // Définition des bordures
    float gap = 5.f;
    setWorldBounds(m_hud.getWidth() + gap, (float)desktopMode.size.x - gap, gap, (float)desktopMode.size.y - 10.f);

    std::cout << "[Application] Prêt." << std::endl;
}

void Application::run() {
    int frameCount = 0;
    bool gameOverTriggered = false; // Pour ne pas spammer le message console

    while (m_window.isOpen()) {
        float currentTime = clockFps.restart().asSeconds();
        float fps = 1.0f / currentTime;

        // --- GESTION ÉVÉNEMENTS ---
        while (const auto event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_window.close();

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) m_window.close();
                
                // Pause manuelle
                if (keyEvent->code == sf::Keyboard::Key::P) {
                    m_isPaused = !m_isPaused;
                    // Si on relance manuellement, on reset le flag de game over pour permettre de jouer
                    if (!m_isPaused) gameOverTriggered = false; 
                }
                
                // Reset
                if (keyEvent->code == sf::Keyboard::Key::R) {
                    initEcosystem();
                    gameOverTriggered = false;
                    m_isPaused = false; // On enlève la pause si on reset
                    std::cout << ">> RESET" << std::endl;
                }
            }

            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()) {
                sf::Vector2u newSize = resizeEvent->size;
                sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)newSize.x, (float)newSize.y));
                m_window.setView(sf::View(visibleArea));
                m_hud.onResize(newSize);
                m_renderer.onResize(newSize, m_hud.getWidth());
                
                float gap = 5.f;
                setWorldBounds(m_hud.getWidth() + gap, (float)newSize.x - gap, gap, (float)newSize.y - 10.f);
            }
        }

        if (m_isTestMode) { frameCount++; if (frameCount > 30) m_window.close(); }

        // --- LOGIQUE (UPDATE) ---
        EcosystemStats stats = getEcosystemStats();

        // CHECK GAME OVER : Si plus de moutons, on arrête tout.
        if (stats.sheep == 0 && stats.wolves > 0 && !gameOverTriggered) {
            m_isPaused = true;
            gameOverTriggered = true;
            std::cout << "=== FIN DE SIMULATION : TOUS LES MOUTONS SONT MORTS ===" << std::endl;
            std::cout << "Appuyez sur [R] pour relancer." << std::endl;
        }

        if (!m_isPaused) {
            ecosystemUpdate(currentTime);
        }

        // --- AFFICHAGE (RENDER) ---
        // On récupère les stats mises à jour (si pas en pause)
        stats = getEcosystemStats(); 
        
        m_hud.update(fps, stats.grass, stats.sheep, stats.wolves, stats.deadSheep, stats.deadWolves, stats.bornSheep, stats.bornWolves);

        m_window.clear(sf::Color(30, 30, 30));
        m_renderer.draw(m_window);
        m_hud.draw(m_window);
        m_window.display();
    }
}