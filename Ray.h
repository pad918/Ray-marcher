#ifndef RAY_H
#define RAY_H
#include <SFML/Graphics.hpp>

namespace tsr {
	/*
	En klass som används för att spara den information som skapas i RayCamera
	och föra över den för bearbetning.
	Allt kunde ha skett i samma klass, men en separering gör det enklare att läsa
	koden och att ändra en av alla programmets delar utan att förstöra resten.
	*/
	class Ray {
	public:
		Ray(sf::Vector3f origin, sf::Vector3f normDir);
		Ray();
		sf::Vector3f pos; // Strålens ursprungsposition
		sf::Vector3f dir; // Enhetsvektor för strålens riktning
		int imgPos;
		float travelDist;
	};

}

#endif
