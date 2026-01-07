/**
 * @file Wolf.cpp
 * @brief Implémentation du Requin : Prédateur des poissons.
 */

// AUCUN INCLUDE ICI

Wolf::Wolf(sf::Vector2f position) 
    : Entity(position, 150.f, sf::Color(100, 100, 120), 12.f) // Gris requin
{
    shape.setOutlineThickness(2); shape.setOutlineColor(sf::Color::Black);
    speed = 100.f; energy = 80.f; m_reproCooldown = 8.0f;
}

void Wolf::update(float dt) {
    if (!alive) return;
    energy -= 4.0f * dt; // Le requin se fatigue vite
    if (energy <= 0) alive = false;
    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Wolf::moveAI(float dt, const std::vector<Sheep>& prey, float simTime) {
    if (!alive) return;
    const Sheep* target = nullptr; float minDist = 500.f;
    for (const auto& s : prey) {
        if (s.alive && s.getLevel() == 2) { // Ne chasse que les poissons adultes
            float d = dist(s.pos);
            if (d < minDist) { minDist = d; target = &s; }
        }
    }
    sf::Vector2f moveDir(0.f, 0.f);
    if (target) {
        sf::Vector2f diff = target->pos - pos;
        if (minDist > 0.1f) moveDir = diff / minDist;
    } else {
        long long seed = (long long)this % 100;
        float angle = std::sin(simTime * 0.3f + seed) * 6.28f;
        moveDir = {std::cos(angle), std::sin(angle)};
    }
    pos += moveDir * speed * dt;
}

void Wolf::eat(std::vector<Sheep>& prey) {
    if (!alive) return;
    for (auto& s : prey) {
        if (s.alive && s.getLevel() == 2 && dist(s.pos) < 25.f) {
            s.alive = false; energy += 60.f;
            if (energy > maxEnergy) energy = maxEnergy;
            break;
        }
    }
}

bool Wolf::canReproduce() const { return alive && energy > 100.f && m_reproCooldown <= 0.f; }
void Wolf::resetReproduction() { energy -= 60.f; m_reproCooldown = 12.f; }
void Wolf::draw(sf::RenderWindow& window) { shape.setPosition(pos); window.draw(shape); }