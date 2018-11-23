#include <cassert>
#include <memory>
#include <iostream>
#include <sstream>

#include "player.hpp"
#include "text.hpp"

Player::Player(const std::shared_ptr<Map>& map) {
	this->map = map;

	if (!texture.loadFromFile("player.png")) {
		throw "unable to load player texture";
	}

	this->bounds = { 37, 33, 16, 24 };
	this->grounded = true;

	this->animationRest.setFrameTime(sf::milliseconds(100));
	this->animationRest.addFrame({ 0,  0, 16, 16});
	this->animationRest.addFrame({ 16, 0, 16, 16});
	this->animationRest.addFrame({ 32, 0, 16, 16});
	this->animationRest.addFrame({ 48, 0, 16, 16});

	this->animationWalk.setFrameTime(sf::milliseconds(50));
	this->animationWalk.addFrame({ 0,  16, 16, 16 });
	this->animationWalk.addFrame({ 16, 16, 16, 16 });
	this->animationWalk.addFrame({ 32, 16, 16, 16 });
	this->animationWalk.addFrame({ 48, 16, 16, 16 });
	this->animationWalk.addFrame({ 64, 16, 16, 16 });
	this->animationWalk.addFrame({ 80, 16, 16, 16 });

	this->playerSprite.setAnimation(animationWalk);
	this->playerSprite.setTexture(this->texture);
	this->playerSprite.setOrigin({ 8, 8 });
	this->playerSprite.setScale({ 2, 2 });
	this->playerSprite.setPlaying(true);
}

Player::~Player() {
}

void Player::handleEvent(const sf::Event& event) {
	// refactor using sf::Keyboard::is....?
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft  = true; this->playerSprite.setFlipped(true);  break;
		case sf::Keyboard::Right: this->moveRight = true; this->playerSprite.setFlipped(false); break;
		case sf::Keyboard::Down:  this->moveDown  = true; break;
		case sf::Keyboard::Up:    this->jump      = true; break;

		default: break;
		}
	} else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft  = false; break;
		case sf::Keyboard::Right: this->moveRight = false; break;
		case sf::Keyboard::Down:  this->moveDown  = false; break;
		case sf::Keyboard::Up:    this->jump      = false; break;
		default: break;
		}
	}
}

bool Player::isPlayerColliding(const sf::FloatRect& bounds) const {
	return this->map->isColliding(bounds);
}

void Player::update(const sf::Time& dt) {
	sf::FloatRect newBounds = this->bounds;

	float timeStep = dt.asMicroseconds() / 100000.0f;

	if (this->moveLeft) {
		newBounds.left -= 18.0f * timeStep;
	}
	if (this->moveRight) {
		newBounds.left += 18.0f * timeStep;
	}

	if (this->moveLeft || this->moveRight) {
		this->playerSprite.setAnimation(this->animationWalk);
		// If we are colliding over the x-axis, reset the newBounds.
		if (this->map->isColliding(newBounds)) {
			newBounds = this->bounds;
		}

		// check a tile below. If nothing, unground ourselves so we can fall freely.
		sf::FloatRect temp = newBounds;
		temp.top += 1;
		if (!this->map->isColliding(temp)) {
			this->grounded = false;
		}
	} else {
		this->playerSprite.setAnimation(this->animationRest);
	}

	if (this->jump && this->grounded) {
		this->dy = -0.5f;
		this->grounded = false;
		this->jumping = true;
	}

	// Are we in the jumping state (jumping), but did we release the jump button?
	// In that case, stop the jumping to control the player height finer grained.
	if (this->jumping && !this->jump) {
		// Stop the jumping state and reset the y acceleration.
		this->jumping = false;
		this->dy = 0.0f;
	}

	// 1. if not grounded. fall.
	// 2. if collision, set grounded to true.
	// 3. set player y position to top of tile
	if (!this->grounded) {
		this->dy += 0.1f * timeStep;
		newBounds.top += this->dy * (dt.asMicroseconds() / 1000.0f);
		if (this->isPlayerColliding(newBounds)) {
			if (this->dy < 0.0f) {
				std::cout << "I was jumping newbounds: " << newBounds.top << std::endl;
				int newy = static_cast<int>(newBounds.top / 32.0) * 32 + 1 + 32;
				newBounds.top = newy;
				std::cout << "NEWBOUNDS " << newBounds.top << std::endl;
				this->dy = 0.0f;
			} else {
				int newy = static_cast<int>(newBounds.top / 32.0) * 32 - 1 + 32;
				std::cout
					<< "Newbounds top: " << newBounds.top
					<< ", newy = " << newy
					<< std::endl;
				newBounds.top = newy - newBounds.height;
				this->grounded = true;
				this->dy = 0.0f;
			}

			this->jumping = false;
		}
	}

	this->playerSprite.update(dt);
	this->bounds = newBounds;
	this->playerSprite.setPosition({ bounds.left + 8, bounds.top + 10 });
}

const sf::FloatRect& Player::getBounds() const {
	return this->bounds;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape collisionBounds;
	collisionBounds.setSize({ this->bounds.width, this->bounds.height});
	collisionBounds.setFillColor(sf::Color::Transparent);
	collisionBounds.setOutlineColor(sf::Color::Red);
	collisionBounds.setOutlineThickness(1.0f);
	collisionBounds.setPosition( { this->bounds.left, this->bounds.top });

	sf::RectangleShape globBoundsRect;
	globBoundsRect.setSize({ this->playerSprite.getGlobalBounds().width, this->playerSprite.getGlobalBounds().height});
	globBoundsRect.setFillColor(sf::Color::Transparent);
	globBoundsRect.setOutlineColor(sf::Color::Magenta);
	globBoundsRect.setOutlineThickness(1.0f);
	globBoundsRect.setPosition({ this->playerSprite.getGlobalBounds().left, this->playerSprite.getGlobalBounds().top});

	target.draw(this->playerSprite, states);
	target.draw(collisionBounds, states);
	target.draw(globBoundsRect, states);
}