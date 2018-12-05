#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "anim.hpp"
#include "level.hpp"
#include "player.hpp"
#include "text.hpp"
#include "util.hpp"

int main() {
	// std::mt19937 rng;
	// rng.seed(std::random_device()());
	// std::uniform_real_distribution<> dist(-1, 1);
	// float f = dist(rng);

	// return 0;

	std::shared_ptr<Map> lolmap = std::make_shared<Map>();
	lolmap->load("map01.tmx");

	std::shared_ptr<Player> player = std::make_shared<Player>();
	player->setMap(lolmap);

	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");

	Text t2(font);

	float w = 320;
	float h = 240;

	Camera camera;
	camera.setMap(lolmap);
	camera.setPlayer(player);

	ParticleGenerator pgen;


	sf::View derp({ w/2, h/2}, { w, h });

	sf::RenderWindow window(sf::VideoMode(800, 600), "Blox", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	sf::View bgView;
	Background bg;

	FpsCounter fps;

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

			camera.handleEvent(event);
			player->handleEvent(event);
		}

		bgView.setSize(camera.getSize().x * 2, camera.getSize().y * 2);
		bgView.setCenter(camera.getCenter().x, camera.getCenter().y);

		std::stringstream ss;
		ss
			<< "Player at ("
				<< static_cast<int>(player->getBounds().left) << ", "
				<< static_cast<int>(player->getBounds().top) << ")"
				<< std::endl
			<< "FPS: " << fps.getFps() << std::endl
			<< "Camera at (" << camera.getCenter().x << ", " << camera.getCenter().y << std::endl;
		t2.setText(0, 0, ss.str());

		player->update(elapsed);
		camera.update();
		pgen.update(elapsed);
		fps.update(elapsed);

		window.clear();
		window.setView(bgView);
		window.draw(bg);
		window.setView(camera);
		lolmap->drawBackgrounds(window);
		window.draw(*player);
		lolmap->drawForegrounds(window);
		window.draw(pgen);
		window.setView(window.getDefaultView());
		window.draw(t2);
		window.display();
	}

	return 0;
}