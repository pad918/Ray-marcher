#ifndef SPHERE_H
#define SPHERE_H
#include <SFML/Graphics.hpp>
#include "GeometricMath.h"
#include "WorldObject.h"

namespace tsr {

	class SphereObj : public tsr::WorldObject{
	private:
		float radius;
	public:
		void setVals(sf::Vector3f p, float r);
		virtual float getDist(sf::Vector3f rayPos);
	};

}

#endif
