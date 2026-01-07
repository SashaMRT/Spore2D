/**
 * @file Wolf.cpp
 * @brief Implémentation de la croissance du louveteau.
 */

// AUCUN INCLUDE ICI

Wolf::Wolf(sf::Vector2f position) 
    : Entity(position, 70.f, sf::Color(200, 50, 50), 6.f) 
{
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(100, 0, 0)); 
    speed = 85.f;     
    energy = 50.f;    
    m_reproCooldown = 5.0f;
    m_eatenSheep = 0;
    m_level = 1; 
}

void Wolf::update(float dt) {
    if (!alive) return;
    float loss = (m_level == 1) ? 2.5f : 3.5f;
    energy -= loss * dt;
    if (energy <= 0) alive = false;
    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Wolf::moveAI(float dt, const std::vector<Sheep>& sheeps, float simTime) {
    if (!alive) return;
    const Sheep* target = nullptr;
    float minDist = 400.f; 
    for (const auto& s : sheeps) {
        if (!s.alive) continue; 
        float d = dist(s.pos);
        if (d < minDist) { minDist = d; target = &s; }
    }
    sf::Vector2f moveDir(0.f, 0.f);
    if (target) {
        sf::Vector2f diff = target->pos - pos;
        if (minDist > 0.1f) moveDir = diff / minDist;
    } else {
        long long seed = (long long)this % 100; 
        float angle = std::sin(simTime * 0.5f + seed) * 6.28f; 
        moveDir = {std::cos(angle), std::sin(angle)};
        speed *= 0.6f; 
    }
    pos += moveDir * speed * dt;
    if (!target) speed /= 0.6f; 
}

void Wolf::eat(std::vector<Sheep>& sheeps) {
    if (!alive) return;
    for (auto& s : sheeps) {
        float killDist = radius + s.radius + 5.f;
        if (s.alive && dist(s.pos) < killDist) { 
            s.alive = false;       
            energy += 50.f;        
            if (energy > maxEnergy) energy = maxEnergy;
            if (m_level == 1) {
                m_eatenSheep++;
                if (m_eatenSheep >= 5) {
                    m_level = 2; 
                    speed = 110.f;      
                    maxEnergy = 120.f;  
                    energy = maxEnergy;
                    radius = 11.f;      
                    shape.setRadius(radius);
                    shape.setOrigin({radius, radius});
                    shape.setFillColor(sf::Color(160, 0, 0)); 
                    std::cout << "Un LOUVETEAU devient un LOUP Alpha !" << std::endl;
                }
            }
            break; 
        }
    }
}

bool Wolf::canReproduce() const {
    return alive && m_level == 2 && energy > 80.f && m_reproCooldown <= 0.f;
}

void Wolf::resetReproduction() {
    energy -= 40.f;         
    m_reproCooldown = 10.f; 
}

void Wolf::draw(sf::RenderWindow& window) {
    shape.setPosition(pos); 
    window.draw(shape);     
}