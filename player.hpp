#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "anim.hpp"
#include "level.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
private:
	std::shared_ptr<Map> map;

	sf::FloatRect bounds;

	sf::Texture texture;

	Animation animationRest;
	AnimatedSprite playerSprite;

	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;
public:
	Player(const std::shared_ptr<Map>& map);
	~Player();

	bool isPlayerColliding(const sf::FloatRect& bounds) const;
	void handleEvent(const sf::Event& event);
	void update(const sf::Time& dt);
	const sf::FloatRect& getBounds() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // PLAYER_HPP