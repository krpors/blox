#include <iomanip>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "level.hpp"
#include "text.hpp"

int main() {
	Map lolmap;
	lolmap.load("map01.tmx");
	// return 0;

	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");

	Text t1(font);
	t1.setText(0, 0, "The quick brown fox jumps over the lazy doggo.");
	Text t2(font);
	t2.setText(0, 10, "Yo dude!");

	sf::RenderWindow window(sf::VideoMode(800, 600), "Blox");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();
		(void)elapsed;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		std::stringstream ss;
		ss << "The elapsed time is " << std::setfill(' ') << std::setw(5) << elapsed.asMicroseconds() << " microseconds.";
		t2.setText(0, 10, ss.str());

		window.clear();
		// window.draw(shape);
		// window.draw(t1);
		// window.draw(t2);
		window.draw(lolmap);
		window.display();
	}

	return 0;
}