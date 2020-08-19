#include <SFML/Graphics.hpp>
#include "GeometricMath.h"
#include "RayCamera.h"
#include "SphereTracer.h"
#include <iostream>
#include <omp.h>

int main() {
	int RESOLUTION_X = 640; int RESOLUTION_Y = 360; // GLOBAL upplösnings variabel 640 360
	//TEST START
	mth::GeometricMath math;
	sf::Vector3f pointer = math.rotToVec(sf::Vector3f(1, 0.5f, 0));
	sf::Vector3f pointer1 = math.rotToVec(sf::Vector3f(1 + 3.14159265358979f/2.0f, 0.0f, 0));
	std::cout << " "  << std::endl;
	// TEST END

	sf::Texture pixelsTexture;
	pixelsTexture.create(RESOLUTION_X, RESOLUTION_Y);
	sf::Sprite frameSprite;
	tsr::SphereTracer ST(sf::Vector2i(RESOLUTION_X, RESOLUTION_Y));
	tsr::RayCamera camera(RESOLUTION_X, RESOLUTION_Y);
	camera.calculateRaysSIMD();
	ST.ray_array = camera.getRays();
	pixelsTexture.update(ST.renderFrame());
	frameSprite.setTexture(pixelsTexture);
	//knapphantering
	bool buttonsDown[255];
	for (bool &btn : buttonsDown)
		btn = false;
	
	sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "Ray_tracing");
	window.setMouseCursorGrabbed(true);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);
	sf::Clock frameClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					//std::cout << "Key down = " << event.key.code << std::endl;
					if (event.key.code < 255 && event.key.code >= 0)
						buttonsDown[event.key.code] = true;
					break;
				case sf::Event::KeyReleased:
					if (event.key.code < 255 && event.key.code >= 0)
						buttonsDown[event.key.code] = false;
					break;
				case sf::Event::MouseMoved:
					camera.rotation.x += (RESOLUTION_X / 2 - event.mouseMove.x) / 1000.0f;
					camera.rotation.y += (RESOLUTION_Y / 2 - event.mouseMove.y) / 1000.0f;
					//camera.rotation.y = (camera.rotation.y > 1) ? 1 : camera.rotation.y;
					//camera.rotation.y = (camera.rotation.y < -1) ? -1 : camera.rotation.y;
					//std::cout << camera.rotation.x << " " << camera.rotation.y << std::endl;
					sf::Mouse::setPosition(sf::Vector2i(RESOLUTION_X / 2, RESOLUTION_Y / 2), window);
					
					break;
				default:
					break;
			}
		}

		//RÄKNA
		camera.updatePos(buttonsDown);
		camera.calculateRaysSIMD();
		ST.ray_array = camera.getRays();
		sf::Uint8 * pixels = ST.renderFrame();
		pixelsTexture.update(pixels); delete(pixels);
		frameSprite.setTexture(pixelsTexture);
		//Rita
		window.clear();
		window.draw(frameSprite);
		window.display();
		sf::Time elapsed1 = frameClock.getElapsedTime();
		std::cout << "renderTime = " << elapsed1.asMilliseconds() << "ms" << std::endl; //325ms från början 160*90 px | 30ms för kameran att räkna rays
		//std::cout << "cam pos, rot= " << camera.position.x << " " << camera.position.y << " " << camera.position.z <<
		//	" | rotation = " << camera.rotation.x << " " << camera.rotation.y << std::endl;
		frameClock.restart();
		
	}
	
	return 0;
}