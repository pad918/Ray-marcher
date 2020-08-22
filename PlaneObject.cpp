#include "PlaneObject.h"

void tsr::PlaneObject::setVals(float h)
{
	height = h;
}

float tsr::PlaneObject::getDist(sf::Vector3f rayPos)
{
	return (rayPos.y-height);
}
