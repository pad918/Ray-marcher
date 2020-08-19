#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H
#include <SFML/Graphics.hpp>
#include "GeometricMath.h"

namespace tsr {
	/*
	En klass för att hålla många olika föremåls distance function.
	Gör att olika föreemål kan behandlas som om de vore samma.
	*/
	class WorldObject {
	protected:
		sf::Vector3f pos;
	public:
		virtual float getDist(sf::Vector3f rayPos) = 0;
	};

}

#endif
