/**
 * @file Application.cpp
 * @brief Chef d'orchestre de la simulation marine.
 */

#include "../../include/Model/Simulation.hpp"

sf::Clock clockFps;

Application::Application(bool testMode) : m_isTestMode(testMode), m_isPaused(false) {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    m_window.create(desktopMode, "Spore2D : Marine Evolution", sf::Style::Default);
    m_window.setFramerateLimit(60);

    m_hud.init(m_window.getSize());
    m_renderer.init(m_window.getSize(), m_hud.getWidth());
    
    // Définition des bordures
    setWorldBounds(m_hud.getWidth() + 5.f, (float)m_window.getSize().x - 5.f, 5.f, (float)m_window.getSize().y - 15.f);
}

void Application::run() {
    while (m_window.isOpen()) {
        float dt = clockFps.restart().asSeconds();
        while (const auto event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_window.close();
            if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {
                if (k->code == sf::Keyboard::Key::Escape) m_window.close();
                if (k->code == sf::Keyboard::Key::P) m_isPaused = !m_isPaused;
                if (k->code == sf::Keyboard::Key::R) initEcosystem();
            }
        }

        if (!m_isPaused) ecosystemUpdate(dt);

        EcosystemStats s = getEcosystemStats();
        m_hud.update(1.f/dt, s.plants, s.preyTotal, s.bacteria, s.fish, s.sharks, 
                     s.deadPrey, s.deadSharks, s.bornPrey, s.bornSharks);

        m_window.clear(sf::Color(5, 15, 30));
        m_renderer.draw(m_window);
        m_hud.draw(m_window);
        m_window.display();
    }
}