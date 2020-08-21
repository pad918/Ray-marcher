#ifndef GEOMETRICMATH_H
#define GEOMETRICMATH_H
#include <SFML/Graphics.hpp>
#include <math.h>

namespace mth {

	class GeometricMath {
	private:
		sf::Vector3f x, y, z; //Enhetsvektorer för x, y, z axlarna
	public:
		GeometricMath();
		sf::Vector3f crossProduct(sf::Vector3f a, sf::Vector3f b);
		sf::Vector3f normalize(sf::Vector3f vec);
		sf::Vector3f rotToVec(sf::Vector3f rot);
		float dist(sf::Vector3f point1, sf::Vector3f point2);
		float distFast(sf::Vector3f point1, sf::Vector3f point2);
		float distSqr(sf::Vector3f point1, sf::Vector3f point2);
		float inverseSqr(float input);
		float fastSqr(float input);
		float dotProduct(sf::Vector3f vect_A, sf::Vector3f vect_B);
		

	};

}

#endif
