#include "SphereObj.h"
#include <iostream>

void tsr::SphereObj::setVals(sf::Vector3f p, float r)
{
	pos = p;
	radius = r;
}

float tsr::SphereObj::getDist(sf::Vector3f rayPos)
{
	mth::GeometricMath math;
	return math.distFast(rayPos, pos)-radius;
}


