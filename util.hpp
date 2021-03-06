#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
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

/**
 * A parallax view is basically just a view which can be used to support
 * parallax background or foreground scrolling.
 */
class ParallaxView : public sf::View {
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<Camera> camera;
public:
	ParallaxView(const std::shared_ptr<Player>& p, const std::shared_ptr<Camera>& c);
	~ParallaxView();

	void update();
};

class Background : public sf::Drawable {
private:
	float left = 0;
	float top = 0;
	int width = 0;
	int height = 0;
	sf::Texture texture;
public:
	Background();
	~Background();

	void update(const Camera& cam);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // UTIL_HPP
