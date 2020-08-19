#include "Ray.h"

tsr::Ray::Ray(sf::Vector3f origin, sf::Vector3f normDir)
{
	dir = normDir;
	pos = origin;
}

tsr::Ray::Ray()
{
	//Gör inget
}
