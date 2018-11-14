#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "anim.hpp"

Animation::Animation(const sf::Time& frameTime) {
	this->frameTime = frameTime;
}

Animation::~Animation() {

}

const sf::IntRect& Animation::nextFrame() {
	this->currentFrame++;
	size_t index = this->currentFrame % this->frames.size();

	std::cout << "nextFrame(): " << index << std::endl;

	assert(index >= 0);
	assert(index < this->frames.size());

	return this->frames[index];
}

void Animation::addFrame(const sf::IntRect& rect) {
	this->frames.push_back(rect);
}

const sf::Time& Animation::getFrameTime() const {
	return this->frameTime;
}


//==============================================================================

AnimatedSprite::AnimatedSprite(const std::shared_ptr<sf::Texture>& texture) {
	this->texture = texture;

	this->sprite.setPosition({ 0, 0 });
	this->sprite.setTextureRect({ 0, 0, 16, 16});
	this->sprite.setTexture(*this->texture);

	this->animation = new Animation(sf::milliseconds(100));
	this->animation->addFrame( { 0,  0,  16, 16 } );
	this->animation->addFrame( { 16, 0,  16, 16 } );
	this->animation->addFrame( { 32, 0,  16, 16 } );
	this->animation->addFrame( { 48, 0,  16, 16 } );

	this->setScale({ 2.0f, 2.0f });
}

AnimatedSprite::~AnimatedSprite() {
	delete this->animation;
}

void AnimatedSprite::update(const sf::Time& delta) {
	this->frametime += delta;

	if (this->frametime > this->animation->getFrameTime()) {
		const sf::IntRect& rect = this->animation->nextFrame();
		this->sprite.setTextureRect(rect);
		this->frametime = sf::Time::Zero;
	}
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = this->texture.get();
	target.draw(this->sprite, states);
}