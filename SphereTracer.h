#ifndef SPHERETRACER_H
#define SPHERETRACER_H
#include <vector>
#include <iostream>
#include <math.h>
#include "Ray.h"
#include "GeometricMath.h"
#include "WorldObject.h"
#include "RayCamera.h"

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
		//Olka metoder för att få avståndet till een pixel
		float tracePointLength(tsr::Ray ray);
		float tracePointLengthO1(tsr::Ray ray);

		float getClosestDist(tsr::Ray &ray);
		void renderCluster(std::vector<tsr::Ray> &rays, sf::Uint8 * pixels);
		mth::GeometricMath math;
	public:
		SphereTracer(sf::Vector2i resolution);
		//Olika renderingstekniker
		sf::Uint8 *renderFrame();
		sf::Uint8 *renderFrameOpt();
		//Publika variabler 
		tsr::Ray * ray_array;
	};

}

#endif
