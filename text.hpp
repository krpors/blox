#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <memory>

class ImageFont {
private:
	std::string filename = "";
	std::string glyphs = "";

	std::map<char, sf::IntRect> glyphMap;

	sf::Image   image;
	sf::Texture texture;
public:
	ImageFont();
	ImageFont(const std::string& filename, const std::string& glyphs);
	~ImageFont();

	void load(const std::string& file, const std::string& glyphs);
	int getFontHeight() const;
	const std::map<char, sf::IntRect> getGlyphMap() const;
	const sf::Texture& getTexture() const;
};

//https://stackoverflow.com/questions/15648844/using-smart-pointers-for-class-members#15649077

/**
 * Text is a class to render text using an ImageFont to the screen.
 */
class Text : public sf::Drawable, public sf::Transformable {
private:
	std::shared_ptr<ImageFont> font;
	std::string text = "";

public:
	Text();
	Text(const std::shared_ptr<ImageFont>& font);
	~Text();

	void setFont(const std::shared_ptr<ImageFont>& font);
	void setText(float x, float y, const std::string& str);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // TEXT_HPP