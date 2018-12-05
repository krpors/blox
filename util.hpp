#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "level.hpp"
#include "player.hpp"

class FpsCounter {
private:
	sf::Time timer;
	unsigned int fps;
	unsigned int frameCounter;
public:
	FpsCounter();
	~FpsCounter();

	void update(const sf::Time& delta);
	unsigned int getFps() const;
};

//==============================================================================

class Camera : public sf::View {
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<Map> map;
public:
	Camera();
	~Camera();

	void setPlayer(const std::shared_ptr<Player>& player);
	void setMap(const std::shared_ptr<Map>& map);
	void handleEvent(const sf::Event& event);
	void update();
};

//==============================================================================

class Particle {
public:
	sf::Vector2f pos;
	sf::Vector2f dir;
	sf::Color    color;
	float        maximumLife; //< max life
	float        currentLife; //< life in millisecs

	Particle(float maxLife);
	~Particle();
};

//==============================================================================

class ParticleGenerator : public sf::Drawable {
private:
	std::mt19937 rng;
	std::uniform_real_distribution<> dist;

	std::vector<Particle> particles;
public:
	ParticleGenerator();
	~ParticleGenerator();

	void resetParticle(Particle& p);
	void update(const sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

//==============================================================================

class Background : public sf::Drawable {
private:
	sf::Texture texture;
public:
	Background();
	~Background();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // UTIL_HPP
