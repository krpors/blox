#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "anim.hpp"

Animation::Animation() {
}

Animation::~Animation() {

}

const sf::IntRect& Animation::nextFrame() {
	this->currentFrame++;
	size_t index = this->currentFrame % this->frames.size();

	// std::cout << "nextFrame(): " << index << ", frametime is " << this->frameTime.asMilliseconds() << std::endl;

	assert(index >= 0);
	assert(index < this->frames.size());

	return this->frames[index];
}

void Animation::addFrame(const sf::IntRect& rect) {
	this->frames.push_back(rect);
}

void Animation::setFrameTime(const sf::Time& frameTime) {
	std::cout << "frametime is!!! " << frameTime.asMicroseconds() << std::endl;
	this->frameTime = frameTime;
}

const sf::Time& Animation::getFrameTime() const {
	return this->frameTime;
}


//==============================================================================

AnimatedSprite::AnimatedSprite() {
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

	// std::cout << "frametime " << this->animation->getFrameTime().asMilliseconds() << std::endl;

	if (this->frametime > this->animation->getFrameTime()) {
		const sf::IntRect& rect = this->animation->nextFrame();
		this->setTextureRect(rect);
		this->frametime = sf::Time::Zero;
	}
}
