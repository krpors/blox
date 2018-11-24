#include <iomanip>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "anim.hpp"
#include "level.hpp"
#include "player.hpp"
#include "text.hpp"
#include "util.hpp"

int main() {
	std::shared_ptr<Map> lolmap = std::make_shared<Map>();
	lolmap->load("map01.tmx");

	std::shared_ptr<Player> player = std::make_shared<Player>();
	player->setMap(lolmap);

	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");

	Text t2(font);
	Text t3(font);

	float w = 320;
	float h = 240;

	Camera camera;
	camera.setMap(lolmap);
	camera.setPlayer(player);


	sf::View derp({ w/2, h/2}, { w, h });

	sf::RenderWindow window(sf::VideoMode(800, 600), "Blox", sf::Style::Close);

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();

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
			if (event.type == sf::Event::MouseWheelScrolled) {
				int multiplier = 60;
				if (event.mouseWheelScroll.delta > 0) {
					derp.setSize(derp.getSize().x - multiplier * 1.33f, derp.getSize().y - multiplier);
				} else if (event.mouseWheelScroll.delta < 0) {
					derp.setSize(derp.getSize().x + multiplier * 1.33f, derp.getSize().y + multiplier);
				}
			}

			player->handleEvent(event);
		}

		std::stringstream ss;
		ss << "The elapsed time is " << std::setfill(' ') << std::setw(5) << elapsed.asMicroseconds() << " microseconds.";
		t2.setText(0, 0, ss.str());

		ss.str("");
		ss
			<< "Player at ("
			<< static_cast<int>(player->getBounds().left) << ", "
			<< static_cast<int>(player->getBounds().top) << ")";
		t3.setText(0, 12, ss.str());

		player->update(elapsed);

		camera.update();

		window.clear();
		window.setView(camera);
		lolmap->drawBackgrounds(window);
		window.draw(*player);
		lolmap->drawForegrounds(window);
		window.setView(window.getDefaultView());
		window.draw(t2);
		window.draw(t3);
		window.display();
	}

	return 0;
}