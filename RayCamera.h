#ifndef RAYCAMERA_H
#define RAYCAMERA_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "GeometricMath.h"
#include "Ray.h"

namespace tsr {

	class RayCamera {
	/*
		RatCamera är en klass som skapar stålar med rätt riktning och ursprungsposition 
		för att de ska efterlika riktiga världen. Avståndet mellan strålarna skilljer sig
		och måste räknas ut enskillt.
	
	*/

	private:
		sf::Vector2i RESOLUTION;

		
		float FOV = 2.5f; // 60 grader FOV
		tsr::Ray *ray_array;
	public:
		RayCamera(int res_x, int res_y);
		void calculateRays();
		void calculateRaysSIMD();
		tsr::Ray * getRays();
		sf::Vector3f rotation; // Kamerans rotation runt x och y axlarna.
		sf::Vector3f position;
		void updatePos(bool buttonsDown[255]); // update the position of the camera based on kay and mouse input
	};


}


#endif // !RAYCAMERA_H

