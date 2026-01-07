/**
 * @file Application.cpp
 * @brief Synchronisation des bordures graphiques et physiques.
 */

// AUCUN INCLUDE (PCH)

sf::Clock clockFps;

Application::Application(bool testMode) : m_isTestMode(testMode), m_isPaused(false) {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    m_window.create(desktopMode, "Spore2D Evolution", sf::Style::Default);
    m_window.setFramerateLimit(60);

    m_hud.init(m_window.getSize());
    m_renderer.init(m_window.getSize(), m_hud.getWidth());
    
    // Initialisation des bordures physiques basées sur le Renderer
    // On laisse une marge de 5 pixels par rapport à la zone noire
    float margin = 5.f;
    float xMin = m_hud.getWidth() + margin;
    float xMax = (float)m_window.getSize().x - margin;
    float yMin = margin;
    float yMax = (float)m_window.getSize().y - 15.f; // Marge basse un peu plus grande

    setWorldBounds(xMin, xMax, yMin, yMax);
}

void Application::run() {
    while (m_window.isOpen()) {
        float currentTime = clockFps.restart().asSeconds();
        float fps = 1.0f / currentTime;

        while (const auto event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_window.close();
            
            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()) {
                sf::Vector2u newSize = resizeEvent->size;
                m_window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {(float)newSize.x, (float)newSize.y})));
                
                m_hud.onResize(newSize);
                m_renderer.onResize(newSize, m_hud.getWidth());

                // On recalcule les bordures physiques lors du redimensionnement !
                float margin = 5.f;
                setWorldBounds(m_hud.getWidth() + margin, (float)newSize.x - margin, margin, (float)newSize.y - 15.f);
            }

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) m_window.close();
                if (keyEvent->code == sf::Keyboard::Key::P) m_isPaused = !m_isPaused;
                if (keyEvent->code == sf::Keyboard::Key::R) initEcosystem();
            }
        }

        if (!m_isPaused) ecosystemUpdate(currentTime);

        EcosystemStats stats = getEcosystemStats();
        m_hud.update(fps, stats.grass, stats.sheep, stats.lambs, stats.wolves, stats.pups, 
                     stats.deadSheep, stats.deadWolves, stats.bornSheep, stats.bornWolves);

        m_window.clear(sf::Color(30, 30, 30));
        m_renderer.draw(m_window);
        m_hud.draw(m_window);
        m_window.display();
    }
}