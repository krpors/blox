#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "anim.hpp"

Animation::Animation() :
	frameTime(sf::milliseconds(100)) {
}

Animation::~Animation() {

}

const sf::IntRect& Animation::nextFrame() {
	this->currentFrame++;
	size_t index = this->currentFrame % this->frames.size();

	assert(index >= 0);
	assert(index < this->frames.size());

	return this->frames[index];
}

void Animation::reset() {
	this->currentFrame = 0;
}

void Animation::addFrame(const sf::IntRect& rect) {
	this->frames.push_back(rect);
}

const std::vector<sf::IntRect>& Animation::getFrames() const {
	return this->frames;
}

void Animation::setFrameTime(const sf::Time& frameTime) {
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

void AnimatedSprite::flipTexture() {
	const sf::Vector2f& scaling = this->getScale();
	this->setScale(-scaling.x, scaling.y);
}

void AnimatedSprite::setAnimation(Animation& animation) {
	this->animation = &animation;
}

void AnimatedSprite::setPlaying(bool playing) {
	// dont change states if its unnecessary
	if (this->playing == playing) {
		return;
	}

	this->playing = playing;

	// When we are starting to play, set the frametime to zero and reset the
	// animation frame index so we can start 'over'.
	if (this->playing) {
		this->frametime = sf::Time::Zero;
	}
}

void AnimatedSprite::update(const sf::Time& delta) {
	if (this->animation == nullptr) {
		return;
	}

	if (!this->playing) {
		return;
	}

	this->frametime += delta;

	if (this->frametime > this->animation->getFrameTime()) {
		const sf::IntRect& rect = this->animation->nextFrame();
		this->setTextureRect(rect);
		this->frametime = sf::Time::Zero;
	}
}
