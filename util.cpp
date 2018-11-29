#include "util.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <random>

Camera::Camera() {
	this->setSize(320, 240);
}

Camera::~Camera() {

}

void Camera::setPlayer(const std::shared_ptr<Player>& player) {
	this->player = player;
}
void Camera::setMap(const std::shared_ptr<Map>& map) {
	this->map = map;
}

void Camera::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseWheelScrolled) {
		int multiplier = 60;
		if (event.mouseWheelScroll.delta > 0) {
			this->setSize(this->getSize().x - multiplier * 1.33f, this->getSize().y - multiplier);
		} else if (event.mouseWheelScroll.delta < 0) {
			this->setSize(this->getSize().x + multiplier * 1.33f, this->getSize().y + multiplier);
		}
	}
}

void Camera::update() {
	const sf::FloatRect& pbounds = this->player->getBounds();
	const sf::FloatRect& mbounds = this->map->getBounds();

	// The minimum x and y camera positions are calculated based on the map's
	// edges. That will prevent the camera from scrolling 'over the edges' of
	// the map. When we aren't clamped to the edge of the map, the player is
	// centered.
	float xmin = this->getSize().x / 2.0f;
	float xmax = mbounds.width - xmin;

	float ymin = this->getSize().y / 2.0f;
	float ymax = mbounds.height - ymin;

	float cx = std::min(xmax, std::max(xmin, pbounds.left));
	float cy = std::min(ymax, std::max(ymin, pbounds.top));

	setCenter(cx, cy);
}

//===
Particle::Particle(float maxLife) :
	maximumLife(maxLife) {
}

Particle::~Particle() {
}
//===


ParticleGenerator::ParticleGenerator() {
	rng.seed(std::random_device()());
	dist = std::uniform_real_distribution<>(-2, 2);

	std::uniform_real_distribution<> dist2(100, 3000);

	for (int i = 0; i < 200; i++) {
		Particle p(dist2(this->rng));
		this->resetParticle(p);
		this->particles.push_back(p);
	}
}

ParticleGenerator::~ParticleGenerator() {
}

void ParticleGenerator::resetParticle(Particle& p) {
		p.pos.x = 200;
		p.pos.y = 200;
		p.dir.x = this->dist(this->rng);
		p.dir.y = this->dist(this->rng);
		p.currentLife = p.maximumLife;
		p.color = sf::Color(255, 255, 255);
}

void ParticleGenerator::update(const sf::Time& dt) {
	for (auto &p : this->particles) {
		p.pos.x += 0.1f * p.dir.x;
		p.pos.y += 0.1f * p.dir.y;
		p.currentLife -= dt.asMilliseconds();
		if (p.currentLife <= 0) {
			this->resetParticle(p);
		}
	}
}

void ParticleGenerator::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::VertexArray arr(sf::Points);
	for (const auto& p : this->particles) {
		sf::Vertex vertex;
		vertex.position = { p.pos.x, p.pos.y };
		vertex.color = p.color;
		arr.append(vertex);
	}

	target.draw(arr);
}