#ifndef SPHERETRACER_H
#define SPHERETRACER_H
#include <vector>
#include <iostream>
#include <math.h>
#include "Ray.h"
#include "GeometricMath.h"
#include "WorldObject.h"

namespace tsr {
	
	/*
	
	Ray macher i form av en "sphere tracer" som använder sig av minsta avståndet till
	ett föremål för att testa kollition (borde vara mycket långsamt)
	
	*/
	class SphereTracer {
	private:
		sf::Vector2i RESOLUTION;
		std::vector<tsr::WorldObject *> objects;
		int tracePoint(tsr::Ray ray);
		float tracePointLength(tsr::Ray ray);
		mth::GeometricMath math;
	public:
		SphereTracer(sf::Vector2i resolution);
		sf::Uint8 *renderFrame();
		tsr::Ray * ray_array;
	};

}

#endif
