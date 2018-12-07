#include "util.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <random>

FpsCounter::FpsCounter() :
		timer(sf::Time::Zero),
		fps(0),
		frameCounter(0) {
}

FpsCounter::~FpsCounter() {
}

void FpsCounter::update(const sf::Time& delta) {
	this->timer += delta;

	if (this->timer > sf::seconds(1)) {
		this->timer = sf::Time::Zero;
		this->fps = frameCounter;
		this->frameCounter = 0;
	} else {
		this->frameCounter++;
	}
}

unsigned int FpsCounter::getFps() const {
	return this->fps;
}

//==============================================================================

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
	dist = std::uniform_real_distribution<>(-10, 10);

	std::uniform_real_distribution<> dist2(100, 8000);

	for (int i = 0; i < 100; i++) {
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
		// std::cout << "curentlife is set to " << p.currentLife << std::endl;
}

void ParticleGenerator::update(const sf::Time& dt) {
	float lol = dt.asSeconds();
	for (auto &p : this->particles) {
		p.pos.x += p.dir.x * lol;
		p.pos.y += p.dir.y * lol;

		p.currentLife -= dt.asSeconds() * 1000;
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

//==============================================================================

ParallaxView::ParallaxView(const std::shared_ptr<Player>& p, const std::shared_ptr<Camera>& c) :
	player(p), camera(c) {
}

ParallaxView::~ParallaxView() {
}

void ParallaxView::update() {
	// f is the factor of resizing the viewport.
	float f = 1.1;
	float w = this->camera->getSize().x / f;
	float h = this->camera->getSize().y / f;

	// get the difference in viewport size, using the original player cam.
	float diffx = w - this->camera->getSize().x;
	float diffy = h - this->camera->getSize().y;

	// Divide the difference to get the adjustments. This is used to properly
	// place the (0, 0) coordinate to the top left based on the player camera.
	float adjustmentx = diffx / 2.0f;
	float adjustmenty = diffy / 2.0f;

	this->setSize(w, h);
	this->setCenter(this->camera->getCenter().x + adjustmentx, this->camera->getCenter().y + adjustmenty);
}

//==============================================================================

Background::Background() {
	this->texture.loadFromFile("background.png");
}

Background::~Background() {
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite s;
	s.setTexture(this->texture);
	target.draw(s, states);
}