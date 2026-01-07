/**
 * @file Sheep.cpp
 * @brief Implémentation de la croissance de l'agneau.
 */

// AUCUN INCLUDE ICI

Sheep::Sheep(sf::Vector2f position) 
    : Entity(position, 60.f, sf::Color(240, 240, 240), 5.f) 
{
    speed = 50.f;        
    energy = 60.f;       
    m_reproCooldown = 2.0f;
    m_eatenGrass = 0;
    m_level = 1; 
}

void Sheep::update(float dt) {
    if (!alive) return;
    float loss = (m_level == 1) ? 1.5f : 2.5f;
    energy -= loss * dt; 
    if (energy <= 0) alive = false;
    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Sheep::moveAI(float dt, const std::vector<Wolf>& wolves, const std::list<Grass>& grass, float simTime) {
    if (!alive) return;
    sf::Vector2f moveDir(0.f, 0.f);
    const Wolf* danger = nullptr;
    float minDangerSq = 120.f * 120.f; 

    for (const auto& w : wolves) {
        if (!w.alive) continue;
        float dx = pos.x - w.pos.x;
        float dy = pos.y - w.pos.y;
        float distSq = dx*dx + dy*dy;
        if (distSq < minDangerSq) { minDangerSq = distSq; danger = &w; }
    }

    if (danger) {
        sf::Vector2f diff = pos - danger->pos;
        float len = std::sqrt(minDangerSq); 
        float panicMult = (m_level == 1) ? 1.2f : 1.5f;
        if (len > 0.1f) moveDir = (diff / len) * panicMult;
    } 
    else {
        sf::Vector2f targetPos = pos;
        bool found = false;
        float minFoodSq = 300.f * 300.f;
        for (const auto& g : grass) {
            if (!g.alive) continue;
            float dx = pos.x - g.pos.x;
            float dy = pos.y - g.pos.y;
            float distSq = dx*dx + dy*dy;
            if (distSq < minFoodSq) { minFoodSq = distSq; targetPos = g.pos; found = true; }
        }
        if (found) {
            sf::Vector2f diff = targetPos - pos;
            float len = std::sqrt(minFoodSq);
            if (len > 0.1f) moveDir = diff / len;
        } else {
            long long seed = (long long)this % 100;
            float angle = std::sin(simTime * 0.3f + seed) * 6.28f;
            moveDir = {std::cos(angle), std::sin(angle)};
            moveDir *= 0.5f; 
        }
    }
    pos += moveDir * speed * dt;
}

void Sheep::eatGrass() {
    energy += 20.f;
    if (energy > maxEnergy) energy = maxEnergy;

    if (m_level == 1) {
        m_eatenGrass++;
        if (m_eatenGrass >= 5) {
            m_level = 2; 
            speed = 70.f;       
            maxEnergy = 100.f;  
            energy = maxEnergy;
            radius = 9.f;       
            shape.setRadius(radius);
            shape.setOrigin({radius, radius});
            std::cout << "Un AGNEAU devient un MOUTON !" << std::endl;
        }
    }
}

bool Sheep::canReproduce() const {
    return alive && m_level == 2 && energy > 60.f && m_reproCooldown <= 0.f;
}

void Sheep::resetReproduction() {
    energy -= 30.f;      
    m_reproCooldown = 5.f; 
}

void Sheep::draw(sf::RenderWindow& window) {
    shape.setPosition(pos);
    window.draw(shape);
}