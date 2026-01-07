/**
 * @file Hud.cpp
 * @brief Implémentation du HUD.
 */

// AUCUN INCLUDE ICI (Géré par CMake PCH)

Hud::Hud() : m_textFps(m_font), m_textInfo(m_font), m_textTitle(m_font) {
    m_width = 250.f; 
}

bool Hud::init(sf::Vector2u windowSize) {
    bool loaded = false;
    if (m_font.openFromFile("assets/font.ttf")) loaded = true;
    else if (m_font.openFromFile("../assets/font.ttf")) loaded = true;
    else if (m_font.openFromFile("../../assets/font.ttf")) loaded = true;

    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(windowSize.y)));
    m_background.setFillColor(sf::Color(0, 0, 0, 150)); 
    m_background.setOutlineThickness(-1.f); 
    m_background.setOutlineColor(sf::Color(255, 255, 255, 30));

    m_textFps.setCharacterSize(14);
    m_textFps.setFillColor(sf::Color::Green);
    m_textFps.setPosition({10.f, 10.f});

    m_textTitle.setString("STATISTIQUES");
    m_textTitle.setCharacterSize(18);
    m_textTitle.setFillColor(sf::Color::White);
    m_textTitle.setStyle(sf::Text::Bold);
    
    sf::FloatRect bounds = m_textTitle.getLocalBounds();
    m_textTitle.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    m_textTitle.setPosition({m_width / 2.0f, 65.f}); 

    m_separator.setSize({m_width - 40.f, 2.f});
    m_separator.setFillColor(sf::Color::White);
    m_separator.setOrigin({m_separator.getSize().x / 2.0f, m_separator.getSize().y / 2.0f});
    m_separator.setPosition({m_width / 2.0f, 85.f}); 

    m_textInfo.setCharacterSize(15);
    m_textInfo.setFillColor(sf::Color(220, 220, 220));
    m_textInfo.setPosition({20.f, 105.f});
    m_textInfo.setLineSpacing(1.3f);

    return true;
}

void Hud::update(float fps, int grass, int sheep, int lambs, int wolves, int pups, 
                int deadS, int deadW, int bornS, int bornW) 
{
    m_textFps.setString("FPS: " + std::to_string(static_cast<int>(fps)));

    int adultSheep = sheep - lambs;
    int adultWolves = wolves - pups;

    std::string info = "";
    info += "Herbe:   " + std::to_string(grass) + "\n\n";

    info += "MOUTONS: " + std::to_string(sheep) + "\n";
    info += " - Adultes: " + std::to_string(adultSheep) + "\n";
    info += " - Agneaux: " + std::to_string(lambs) + "\n\n";

    info += "LOUPS:   " + std::to_string(wolves) + "\n";
    info += " - Adultes: " + std::to_string(adultWolves) + "\n";
    info += " - Louveteaux: " + std::to_string(pups) + "\n\n";

    info += "--- MORTS ---\n";
    info += "Moutons: " + std::to_string(deadS) + "\n";
    info += "Loups:   " + std::to_string(deadW) + "\n\n";
    
    info += "[P] Pause  [R] Reset";

    m_textInfo.setString(info);
}

void Hud::draw(sf::RenderWindow& window) {
    window.draw(m_background);
    window.draw(m_separator);
    window.draw(m_textTitle);
    window.draw(m_textFps);
    window.draw(m_textInfo);
}

void Hud::onResize(sf::Vector2u newSize) {
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(newSize.y)));
}