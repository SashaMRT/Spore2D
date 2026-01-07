/**
 * @file Sheep.cpp
 * @brief Implémentation de la Bactérie (Niv 1) évoluant en Poisson (Niv 2).
 */

// AUCUN INCLUDE ICI

Sheep::Sheep(sf::Vector2f position) 
    : Entity(position, 50.f, sf::Color(0, 255, 100, 150), 4.f) // Vert translucide (Bactérie)
{
    speed = 40.f; energy = 50.f; m_reproCooldown = 3.0f;
    m_eatenGrass = 0; m_level = 1; 
}

void Sheep::update(float dt) {
    if (!alive) return;
    float loss = (m_level == 1) ? 1.0f : 2.5f;
    energy -= loss * dt;
    if (energy <= 0) alive = false;
    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Sheep::moveAI(float dt, const std::vector<Wolf>& sharks, const std::list<Grass>& plants, float simTime) {
    if (!alive) return;
    sf::Vector2f moveDir(0.f, 0.f);
    
    // 1. FUIR LES REQUINS (Uniquement si on est un Poisson)
    const Wolf* danger = nullptr;
    if (m_level == 2) {
        float minDangerSq = 150.f * 150.f;
        for (const auto& w : sharks) {
            float d = (pos.x - w.pos.x)*(pos.x - w.pos.x) + (pos.y - w.pos.y)*(pos.y - w.pos.y);
            if (d < minDangerSq) { minDangerSq = d; danger = &w; }
        }
    }

    if (danger) {
        sf::Vector2f diff = pos - danger->pos;
        float len = std::sqrt((pos.x - danger->pos.x)*(pos.x - danger->pos.x) + (pos.y - danger->pos.y)*(pos.y - danger->pos.y));
        if (len > 0.1f) moveDir = (diff / len) * 1.8f;
    } else {
        // 2. CHERCHER DES PLANTES
        sf::Vector2f targetPos = pos; bool found = false; float minSq = 350.f * 350.f;
        for (const auto& p : plants) {
            float d = (pos.x - p.pos.x)*(pos.x - p.pos.x) + (pos.y - p.pos.y)*(pos.y - p.pos.y);
            if (d < minSq) { minSq = d; targetPos = p.pos; found = true; }
        }
        if (found) {
            sf::Vector2f diff = targetPos - pos; float len = std::sqrt(minSq);
            if (len > 0.1f) moveDir = diff / len;
        } else {
            long long seed = (long long)this % 100;
            float angle = std::sin(simTime * 0.4f + seed) * 6.28f;
            moveDir = {std::cos(angle), std::sin(angle)};
        }
    }
    pos += moveDir * speed * dt;
}

void Sheep::eatGrass() {
    energy += 25.f; if (energy > maxEnergy) energy = maxEnergy;
    m_eatenGrass++;

    if (m_eatenGrass >= 5) {
        m_eatenGrass = 0;
        m_level++;
        if (m_level == 2) { // Devenir Poisson
            speed = 80.f; maxEnergy = 100.f; radius = 8.f;
            shape.setRadius(radius); shape.setOrigin({radius, radius});
            shape.setFillColor(sf::Color(0, 150, 255)); // Bleu
            std::cout << "EVOLUTION : Poisson !" << std::endl;
        }
        // Le passage au niveau 3 (Requin) est capté par Simulation.cpp
    }
}

bool Sheep::canReproduce() const { return alive && m_level >= 2 && energy > 70.f && m_reproCooldown <= 0.f; }
void Sheep::resetReproduction() { energy -= 40.f; m_reproCooldown = 6.f; }
void Sheep::draw(sf::RenderWindow& window) { shape.setPosition(pos); window.draw(shape); }