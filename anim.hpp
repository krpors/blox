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
	Animation(const sf::Time& frameTime);
	~Animation();

	const sf::Time& getFrameTime() const;
	const sf::IntRect& nextFrame();
	void addFrame(const sf::IntRect& rect);
};

class AnimatedSprite : public sf::Sprite {
private:
	sf::Time frametime;
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;

	size_t currFrame = 0;
	std::vector<sf::IntRect> frames;

	Animation* animation;
public:
	AnimatedSprite(const std::shared_ptr<sf::Texture>& texture);
	~AnimatedSprite();

	void update(const sf::Time& delta);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif // ANIMATION_HPP