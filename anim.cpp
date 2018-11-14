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

AnimatedSprite::AnimatedSprite() {
	this->setScale({ 2.0f, 2.0f });
}

AnimatedSprite::~AnimatedSprite() {
}

void AnimatedSprite::setAnimation(Animation& animation) {
	this->animation = &animation;
	// TODO: select first frame cus this advances to the next immediately
	this->setTextureRect(this->animation->nextFrame());
}

void AnimatedSprite::update(const sf::Time& delta) {
	if (this->animation == nullptr) {
		return;
	}

	this->frametime += delta;

	if (this->frametime > this->animation->getFrameTime()) {
		const sf::IntRect& rect = this->animation->nextFrame();
		this->setTextureRect(rect);
		this->frametime = sf::Time::Zero;
	}
}
