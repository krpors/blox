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
	Animation animationJump;
	Animation animationFalling;

	AnimatedSprite playerSprite;

	float dy = 0.0f; //< delta y, used for jumping/falling(gravity)

	bool grounded = false; // are we standing on some solid surface?

	bool moveLeft  = false;
	bool moveRight = false;
	bool moveDown  = false;

	bool jump      = false; //< True if we pressed the jump button.
	bool jumping   = false; //< True if we are still jumping.
public:
	Player();
	~Player();

	void setMap(const std::shared_ptr<Map>& map);
	void stop();
	bool isPlayerColliding(const sf::FloatRect& bounds) const;
	bool isStatic() const;
	void handleEvent(const sf::Event& event);
	void update(const sf::Time& dt);
	const sf::FloatRect& getBounds() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // PLAYER_HPP