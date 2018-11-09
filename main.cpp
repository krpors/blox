#include <iomanip>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "level.hpp"
#include "player.hpp"
#include "text.hpp"

int main() {
	Map lolmap;
	lolmap.load("map01.tmx");

	Player player(&lolmap);

	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");


	Text t1(font);
	t1.setText(0, 0, "The quick brown fox jumps over the lazy doggo.");
	Text t2(font);
	t2.setText(0, 10, "Yo dude!");

	sf::RenderWindow window(sf::VideoMode(800, 600), "Blox", sf::Style::Close);

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
				switch (event.key.code) {
				case sf::Keyboard::Escape: window.close(); break;
				default: break;
				}
			}

			player.handleEvent(event);
		}

		std::stringstream ss;
		ss << "The elapsed time is " << std::setfill(' ') << std::setw(5) << elapsed.asMicroseconds() << " microseconds.";
		t2.setText(0, 10, ss.str());

		player.update(elapsed);

		window.clear();
		window.draw(lolmap);
		window.draw(player);
		window.draw(t1);
		window.draw(t2);
		window.display();
	}

	return 0;
}