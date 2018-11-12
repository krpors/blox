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
	// t1.setText(0, 0, "The quick brown fox jumps over the lazy doggo.");
	t1.setText(12, 0, "abcdefghijklmnopqrstuvwxyz");
	Text t2(font);
	t2.setText(0, 10, "Yo dude!");

	float w = 320;
	float h = 240;
	sf::View derp({ w/2, h/2}, { w, h });

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
				case sf::Keyboard::Add: derp.setSize(derp.getSize().x + 10, derp.getSize().y + 10); break;
				case sf::Keyboard::Subtract: derp.setSize(derp.getSize().x - 10, derp.getSize().y - 10); break;
				default: break;
				}
			}

			player.handleEvent(event);
		}

		std::stringstream ss;
		ss << "The elapsed time is " << std::setfill(' ') << std::setw(5) << elapsed.asMicroseconds() << " microseconds.";
		t2.setText(0, 10, ss.str());

		player.update(elapsed);
		derp.setCenter({ player.getBounds().left, player.getBounds().top });

		window.clear();
		window.setView(derp);
		window.draw(lolmap);
		lolmap.drawBackgrounds(window);
		window.draw(player);
		lolmap.drawForegrounds(window);
		window.setView(window.getDefaultView());
		window.draw(t1);
		window.draw(t2);
		window.display();
	}

	return 0;
}