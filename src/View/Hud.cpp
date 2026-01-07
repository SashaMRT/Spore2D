/**
 * @file Hud.cpp
 * @brief Affichage des statistiques de l'écosystème marin.
 */

// AUCUN INCLUDE ICI

Hud::Hud() : m_textFps(m_font), m_textInfo(m_font), m_textTitle(m_font) { m_width = 260.f; }

bool Hud::init(sf::Vector2u windowSize) {
    if (!m_font.openFromFile("assets/font.ttf")) return false;

    m_background.setSize({m_width, (float)windowSize.y});
    m_background.setFillColor(sf::Color(0, 10, 30, 200)); // Bleu très foncé

    m_textFps.setCharacterSize(14); m_textFps.setFillColor(sf::Color::Cyan);
    m_textFps.setPosition({10.f, 10.f});

    m_textTitle.setString("OCEAN STATS");
    m_textTitle.setCharacterSize(20); m_textTitle.setFillColor(sf::Color::White);
    m_textTitle.setStyle(sf::Text::Bold);
    m_textTitle.setPosition({40.f, 60.f});

    m_textInfo.setCharacterSize(15); m_textInfo.setFillColor(sf::Color(200, 230, 255));
    m_textInfo.setPosition({20.f, 100.f});
    m_textInfo.setLineSpacing(1.4f);

    return true;
}

void Hud::update(float fps, int grass, int totalPrey, int bac, int fish, int sharks, 
                int deadP, int deadS, int bornP, int bornS) 
{
    m_textFps.setString("FPS: " + std::to_string((int)fps));
    
    std::string info = "";
    info += "Algues:    " + std::to_string(grass) + "\n\n";
    info += "BACTERIES: " + std::to_string(bac) + "\n";
    info += "POISSONS:  " + std::to_string(fish) + "\n";
    info += "REQUINS:   " + std::to_string(sharks) + "\n\n";
    info += "--- MORTS ---\n";
    info += "Poissons:  " + std::to_string(deadP) + "\n";
    info += "Requins:   " + std::to_string(deadS) + "\n\n";
    info += "[P] Pause  [R] Reset";
    
    m_textInfo.setString(info);
}

void Hud::draw(sf::RenderWindow& window) {
    window.draw(m_background); window.draw(m_textTitle);
    window.draw(m_textFps); window.draw(m_textInfo);
}

void Hud::onResize(sf::Vector2u newSize) { m_background.setSize({m_width, (float)newSize.y}); }