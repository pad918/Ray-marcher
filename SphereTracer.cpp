#include "SphereTracer.h"
#include "SphereObj.h"
#include "PlaneObject.h"

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

float tsr::SphereTracer::tracePointLengthO1(tsr::Ray ray)
{
	/*Optimerad med "MIN EGNA OPTIMISERING! tm" 
		Går ut på att minska antalet dist tester eftersom det är cpu krävande

	*/
	int numObjects = objects.size();
	float * prevDist = new float[numObjects];
	memset(prevDist, 0, numObjects * sizeof(float)); // VET EJ OM DEN SKA ANVÄNDAS SÅ HÄR MEN BORDE TYP FUNKA
	float totalDist = 0.0f;
	sf::Vector3f pos;
	pos = ray.pos;
	float closestObj = 0.0f, prevClosest = 0.0f;;
	for (int i = 0; i < 100; i++) {
		//Uppdatera
		totalDist += closestObj;
		pos += ray.dir * closestObj;
		closestObj = INFINITY;
		for (int j = 0; j < numObjects; j++) {
			if ((prevDist[j] -= prevClosest) < closestObj) {
				float dist = objects[j]->getDist(pos);
				prevDist[j] = dist;
				closestObj = (dist < closestObj) ? dist : closestObj;
			}
		}
		i = (closestObj < 0.05f || closestObj > 100.0f) ? 1000 : i;
		prevClosest = closestObj;
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
	int test = 0;
nextElement: //Har hört att goto anses vara ful kod... 
	while (rays.size()>0) {
		float dist = getClosestDist(rays[0]);
		//ta bort ståle 0 som är nära nog
		if (dist<0.01f || dist>100.0f || test>100) {
			float col = (100.0f - rays[0].travelDist)*2.55f;
			bool hasHit = (dist < 0.01f);
			for (int j = 0; j < 3; j++)
				pixels[rays[0].imgPos * 4 + j] = hasHit ? col : 0;
			pixels[rays[0].imgPos * 4 + 3] = 255;
			rays.erase(rays.begin());
			goto nextElement;
		}
			
		
		for (int i = 1; i < rays.size(); i++) {
			/*Matte från http://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection */
			float r = dist;
			float a, b, c;
			a = math.dotProduct(rays[i].dir, rays[i].dir);
			b = 2.0f * math.dotProduct(rays[i].pos - rays[0].pos, rays[i].dir); 
			c = math.dotProduct(rays[i].pos - rays[0].pos, rays[i].pos - rays[0].pos) - (r*r);
			float disc = (b*b) - (4.0f*a*c);  
			bool isInside = ((b*b) < disc); //uträkning utan kvadratrot. om en t en två rötter och en av dem är negativ så är stålen i sfären
			if (disc <= 0.0f || !isInside) {
				//outside of circle (calculate by itself)
				float l = tracePointLength(rays[i]) + rays[i].travelDist;
				bool hasHit = (l <= 100.0f);
				float col = (100 - rays[i].travelDist)*2.55;
				for(int j = 0; j<3; j++)
					pixels[rays[i].imgPos*4 + j] = hasHit ? col : 0;
				pixels[rays[i].imgPos * 4 + 3] = 255;
				rays.erase(rays.begin() + i); // remove ray since it is calculated
			}
			else {
				//Hitta längden till kollition med sfären:
				float t = (-b + math.fastSqr(disc))/(2.0f*a);
				rays[i].pos += t * rays[i].dir; // flytta stålen till kollitionspunkten
				rays[i].travelDist += t;
			}
			
		}
		rays[0].pos += dist * rays[0].dir;
		rays[0].travelDist += dist;
		test++;
	}
}

tsr::SphereTracer::SphereTracer(sf::Vector2i resolution) // skapa objekt
{
	RESOLUTION = resolution;
	tsr::PlaneObject * tmp = new tsr::PlaneObject;
	tmp->setVals(-1);
	objects.push_back(tmp);
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
		float length = tracePointLengthO1(ray_array[i]); //tracePointLength() <-- orinellt
		bool hasHit = (length <= 100.0f);
		float col = (100.0f - length) *2.55f; //  / 20.0f * 255.0f
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
		//std::cout << "Rendering cluster: " << clusterId << std::endl;
		for (int i = 0; i < clusterSize; i++)
			rayCluster.push_back(ray_array[clusterId*clusterSize+i]); //Långsamt, gör snabbare på ngt sätt (passa inte som reference).
		renderCluster(rayCluster, pixels);
	}
	return pixels;
}


