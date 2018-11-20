#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "anim.hpp"
#include "level.hpp"
#include "text.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
private:
	std::shared_ptr<Map> map;

	sf::FloatRect bounds;

	sf::Texture texture;

	Animation animationRest;
	Animation animationWalk;

	AnimatedSprite playerSprite;

	float dy = 0.0f; //< delta y, used for jumping/falling(gravity)

	bool grounded = false; // are we standing on some solid surface?

	bool moveLeft  = false;
	bool moveRight = false;
	bool moveUp    = false;
	bool moveDown  = false;
public:
	Player(const std::shared_ptr<Map>& map);
	~Player();

	void stop();
	bool isPlayerColliding(const sf::FloatRect& bounds) const;
	void handleEvent(const sf::Event& event);
	void update(const sf::Time& dt);
	const sf::FloatRect& getBounds() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // PLAYER_HPP