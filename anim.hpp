#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

class Animation {
private:
	sf::Time frameTime; ///< The frame time between each frame.

	size_t currentFrame;
	std::vector<sf::IntRect> frames; ///< Texture rectangles.
public:
	Animation();
	~Animation();

	void setFrameTime(const sf::Time& frametime);
	const sf::Time& getFrameTime() const;
	const sf::IntRect& nextFrame();
	void addFrame(const sf::IntRect& rect);
};

//==============================================================================

class AnimatedSprite : public sf::Sprite {
private:
	sf::Time frametime;

	Animation* animation = nullptr;
public:
	AnimatedSprite();
	~AnimatedSprite();

	void setAnimation(Animation& animation);
	void update(const sf::Time& delta);
	// void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif // ANIMATION_HPP