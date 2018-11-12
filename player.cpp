#include <cassert>
#include <memory>
#include <iostream>

#include "player.hpp"

Player::Player(const std::shared_ptr<Map>& map) {
	this->map = map;

	if (!texture.loadFromFile("player.png")) {
		throw "unable to load player texture";
	}

	this->bounds = { 37, 68, 16, 16 };

	this->playerSprite.setTexture(this->texture);
	this->playerSprite.setTextureRect({ 0, 0, 16, 16 });
	this->playerSprite.setScale({ 2, 2 });
}

Player::~Player() {
}

void Player::handleEvent(const sf::Event& event) {
	// refactor using sf::Keyboard::is....?
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft  = true; break;
		case sf::Keyboard::Right: this->moveRight = true; break;
		case sf::Keyboard::Down:  this->moveDown  = true; break;
		case sf::Keyboard::Up:    this->moveUp    = true; break;
		default: break;
		}
	} else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft  = false; break;
		case sf::Keyboard::Right: this->moveRight = false; break;
		case sf::Keyboard::Down:  this->moveDown  = false; break;
		case sf::Keyboard::Up:    this->moveUp    = false; break;
		default: break;
		}
	}
}

bool Player::isPlayerColliding(const sf::FloatRect& bounds) const {
	return this->map->isColliding(bounds);
}

void Player::update(const sf::Time& dt) {
	sf::FloatRect bounds = this->bounds;

	if (this->moveLeft) {
		bounds.left -= 0.5f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveRight) {
		bounds.left += 0.5f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveDown) {
		bounds.top += 0.5f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveUp) {
		bounds.top -= 0.5f * (dt.asMicroseconds() / 1000.0f);
	}

	if (!this->map->isColliding(bounds)) {
		this->bounds = bounds;
	}
}

const sf::FloatRect& Player::getBounds() const {
	return this->bounds;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setSize({ this->bounds.width, this->bounds.height});
	rect.setOutlineColor(sf::Color::Red);
	rect.setPosition( { this->bounds.left, this->bounds.top });
	target.draw(rect, states);
}