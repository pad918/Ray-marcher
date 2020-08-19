#include "SphereTracer.h"
#include "SphereObj.h"

int tsr::SphereTracer::tracePoint(tsr::Ray ray)
{
	bool hit = false;
	int tests = 0;
	sf::Vector3f pos;
	pos = ray.pos;
	double closestObj = 0.0f;
	for (int i = 0; i < 1000; i++) {
		//Get closest sphere
		tests++;
		double moveDist= (closestObj);
		pos = sf::Vector3f(pos.x + ray.dir.x * moveDist, pos.y + ray.dir.y * moveDist, pos.z + ray.dir.z * moveDist);
		closestObj = INFINITY;
		for (int j = 0; j < objects.size(); j++) { //bov 1
			float dist = objects[j]->getDist(pos); // STOBOVEN
			if (dist < closestObj) { closestObj = dist; }
		}
		if (closestObj < 0.05f) {
			//Close enough to call it a hit
			return tests;
			break;
		}
		else if (closestObj > 100.0f) { // Renderdistance
			return -1;
			break; // to far away
		}
	}
	return -1;
} //Används int längre

float tsr::SphereTracer::tracePointLength(tsr::Ray ray)
{
	float totalDist = 0.0f;
	sf::Vector3f pos;
	pos = ray.pos;
	float closestObj = 0.0f;
	for (int i = 0; i < 100; i++) { //100
		//Get closest point
		totalDist += closestObj;
		pos += ray.dir * closestObj;
		closestObj = INFINITY;
		for (int j = 0; j < objects.size(); j++) { 
			float dist = objects[j]->getDist(pos); 
			closestObj = (dist < closestObj) ? dist : closestObj;
		}
		i = (closestObj < 0.05f || closestObj > 100.0f) ? 1000 : i;
	}
	return totalDist;
}

tsr::SphereTracer::SphereTracer(sf::Vector2i resolution)
{
	RESOLUTION = resolution;
	for (int i = 0; i < 6; i++) {
		tsr::SphereObj * tmp = new tsr::SphereObj;
		tmp->setVals(sf::Vector3f(i*10, 0, 0), (i+1)/2.0f);
		objects.push_back(tmp);
	}
	ray_array = new tsr::Ray[resolution.x * resolution.y];
}

sf::Uint8 * tsr::SphereTracer::renderFrame()
{
	sf::Uint8 *pixels = new sf::Uint8[RESOLUTION.x * RESOLUTION.y * 4]; //RGBA pixlar
	int totalRays = RESOLUTION.x*RESOLUTION.y;
	for (int i = 0; i < totalRays; i++) {
		int pixPos = i * 4;
		float length = tracePointLength(ray_array[i]);
		bool hasHit = (length <= 100.0f);
		float col = (20.0f - length) *12.75; //  / 20.0f * 255.0f
		pixels[pixPos + 0] = hasHit ? col : 0;
		pixels[pixPos + 1] = hasHit ? col : 0;
		pixels[pixPos + 2] = hasHit ? col : 0;
		pixels[pixPos + 3] = 255;
	}
	return pixels;
}

