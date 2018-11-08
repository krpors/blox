#include <iostream>

#include "player.hpp"

Player::Player() {
	if (!texture.loadFromFile("player.png")) {
		throw "unable to load player texture";
	}

	this->prect = { 0, 0, 16, 16 };
	this->playerSprite.setTexture(this->texture);
	this->playerSprite.setTextureRect(this->prect);
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
		default: break;
		}
	} else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft  = false; break;
		case sf::Keyboard::Right: this->moveRight = false; break;
		default: break;
		}
	}
}

void Player::update(const sf::Time& dt) {
	sf::Vector2f pos = this->playerSprite.getPosition();
	if (this->moveLeft) {
		pos.x -= 0.5f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveRight) {
		pos.x += 0.5f * (dt.asMicroseconds() / 1000.0f);
	}

	if (pos.x > 0 && pos.x < 800.0f - (32)) {
		this->playerSprite.setPosition(pos);
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->playerSprite, states);
}