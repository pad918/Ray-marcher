#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H
#include "WorldObject.h"

namespace tsr {

	class PlaneObject : public tsr::WorldObject {
		float height;

	public:

		void setVals(float h);
		virtual float getDist(sf::Vector3f rayPos);
	};


}


#endif;