#include <iostream>
#include <map>
#include <memory>
#include <sstream>

#include "text.hpp"

ImageFont::ImageFont() {
}

ImageFont::ImageFont(const std::string& file, const std::string& glyphs) {
	this->load(file, glyphs);
}

ImageFont::~ImageFont() {
	std::clog << "Imagefont is destroyed" << std::endl;
}

void ImageFont::load(const std::string& file, const std::string& glyphs) {
	this->filename = file;
	this->glyphs = glyphs;

	if (!this->image.loadFromFile(file)) {
		throw "Unable to open file '" + file + "'";
	}

	if (!this->texture.loadFromImage(this->image)) {
		throw "Unable to load texture from image object";
	}

	texture.setSmooth(false);

	std::clog << "ImageFont created with image file " << file << std::endl;

	// Get the separator color of the image, which is the topleft pixel of the image.
	sf::Color sepColor = image.getPixel(0, 0);

	// The vector containing rectangle for all the glyphs.
	std::vector<sf::IntRect> glyphRects;

	bool readGlyph = false;
	unsigned int glyphWidth = 0;
	unsigned glyphCount = 0;

	// Start iterating of the x-axis of the image to find the individual glyphs.
	const sf::Vector2u imageSize = image.getSize();
	for (unsigned int x = 0; x < imageSize.x; x++) {
		sf::Color currentColor = image.getPixel(x, 0);
		if (currentColor == sepColor) {
			if (readGlyph) {
				// We found a (next) separator, so we can read the glyph fully
				// by using the glyph width.
				glyphRects.emplace_back(x - glyphWidth, 0, glyphWidth, imageSize.y);
				glyphCount++;
			}

			readGlyph = false;
			glyphWidth = 0;
		} else {
			// We are not reading a separator, so increase the glyph width,
			// until we hit the next separator color.
			readGlyph = true;
			glyphWidth++;
		}
	}

	if (glyphCount != this->glyphs.size()) {
		std::stringstream ss;
		ss << "Glyph count " << glyphCount << " does not equal " << this->glyphs.size();
		throw ss.str();
	}

	int counter = 0;
	for (auto& rect : glyphRects) {
		// map the current character in the 'glyphs' string to the rectangle.
		this->glyphMap[this->glyphs[counter++]] = rect;
	}

	std::clog << "Initialized ImageFont with " << glyphCount << " glyphs" << std::endl;
}

const std::map<char, sf::IntRect> ImageFont::getGlyphMap() const {
	return this->glyphMap;
}

const sf::Texture& ImageFont::getTexture() const {
	return this->texture;
}

// =============================================================================
// Text class.
// =============================================================================

Text::Text() {
}

Text::Text(const std::shared_ptr<ImageFont>& font) {
	this->setFont(font);
}

Text::~Text() {
	std::clog << "Text is destroyed" << std::endl;
}

void Text::setFont(const std::shared_ptr<ImageFont>& font) {
	this->font = font;
}

void Text::setText(float x, float y, const std::string& str) {
	setPosition(x, y);
	this->text = str;
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// TODO: this transform will do it relative to a parent. figure it out properly.
	// states.transform = this->getTransform();
	sf::Sprite sprite(font->getTexture());
	sf::Vector2f position = getPosition();
	const std::map<char, sf::IntRect>& map = this->font->getGlyphMap();
	for (const char& c : this->text) {
		sf::Text t;
		auto search = map.find(c);
		if (search != map.end()) {
			sf::IntRect texrect = search->second;
			// TODO: when the View is zoomed in, we get yellow borders.
			// The texture rect is probably bugged.

			sprite.setTextureRect(texrect);
			sprite.setPosition(position);
			position.x += search->second.width;

			target.draw(sprite, states);
 		}
	}
}