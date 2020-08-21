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

float tsr::SphereTracer::getClosestDist(tsr::Ray & ray)
{
	float closestObj = INFINITY;
	for (int j = 0; j < objects.size(); j++) {
		float dist = objects[j]->getDist(ray.pos);
		closestObj = (dist < closestObj) ? dist : closestObj;
	}
	return closestObj;
}

void tsr::SphereTracer::renderCluster(std::vector<tsr::Ray>& rays, sf::Uint8 * pixels)
{
nextElement:
	while (rays.size()>0) {
		float dist = getClosestDist(rays[0]);
		//ta bort ståle 0 som är nära nog
		if (dist<0.01f || dist>100.0f) {
			rays.erase(rays.begin());
			goto nextElement;
		}
			
		
		for (int i = 1; i < rays.size(); i++) {
			/*Matte från http://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection */
			float r = dist;//
			float a, b, c;
			a = math.dotProduct(rays[i].dir, rays[i].dir);
			b = 2.0f * math.dotProduct(rays[i].pos - rays[0].pos, rays[i].dir); // ska vara noll i början
			c = math.dotProduct(rays[i].pos - rays[0].pos, rays[i].pos - rays[0].pos) - (r*r);//FEL HÄR TYP?
			float disc = (b*b) - (4.0f*a*c); // discriminant 
			bool isInside = ((b*b) < disc); //uträkning utan kvadratrot. om en t är negativ och en positiv så är den i sfären
			if (disc <= 0.0f || !isInside) {
				//outside of circle (calculate by itself)
				float l = tracePointLength(rays[i]) + rays[i].travelDist;
				bool hasHit = (l <= 100.0f);
				for(int j = 0; j<3; j++)
					pixels[rays[i].imgPos*4 + j] = hasHit ? 200 : 0;
				pixels[rays[i].imgPos * 4 + 3] = 255;
				rays.erase(rays.begin() + i); // remove ray since it is calculated
			}
			else {
				//Hitta längden till kollition med sfären:
				float t = (-b + math.fastSqr(disc))/(2.0f*a);
				rays[i].pos += t * rays[i].dir; // flytta stålen till kollitionspunkten
				rays[i].travelDist += t;
			}
			rays[0].pos += dist * rays[0].dir;
		}
	}
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

sf::Uint8 * tsr::SphereTracer::renderFrameOpt()
{
	sf::Uint8 *pixels = new sf::Uint8[RESOLUTION.x * RESOLUTION.y * 4]; //RGBA pixlar
	int totalRays = RESOLUTION.x*RESOLUTION.y;
	int clusterSize = 8;
	std::vector<tsr::Ray> rayCluster;
	int totalClusters = totalRays / clusterSize;
	for (int clusterId = 0; clusterId < totalClusters; clusterId++) {
		std::cout << "Rendering cluster: " << clusterId << std::endl; //Fastnar på kluster id = 1 av ngn anledning
		for (int i = 0; i < clusterSize; i++)
			rayCluster.push_back(ray_array[clusterId*clusterSize+i]); //Långsamt, gör snabbare på ngt sätt (passa inte som reference).
		renderCluster(rayCluster, pixels);
	}
	return pixels;
}


