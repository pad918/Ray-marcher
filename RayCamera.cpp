#include "RayCamera.h"

tsr::RayCamera::RayCamera(int res_x, int res_y)
{
	RESOLUTION = sf::Vector2i(res_x, res_y); 
	rotation = sf::Vector3f(0.f, 0.f, 0.0f);
	position = sf::Vector3f(0.0f, 0.0f, 0.0f);
	ray_array = new tsr::Ray[res_x * res_y];
}

void tsr::RayCamera::calculateRays()
{
	sf::Vector3f up, side, forward, plainDir;
	mth::GeometricMath math;
	forward = math.rotToVec(sf::Vector3f(rotation.x, rotation.y, 0));
	side = math.normalize(math.crossProduct(sf::Vector3f(0.0f, 1.0f, 0.0f), forward));
	up = math.normalize(math.crossProduct(forward, side));

	float aspectRatio = RESOLUTION.x / (float)RESOLUTION.y;
	for (int y = 0; y < RESOLUTION.y; y++) {
		float partY = (y - (RESOLUTION.y / 2.0f)) / (float)RESOLUTION.y;
		partY *= 2.0f / aspectRatio;
		for (int x = 0; x < RESOLUTION.x; x++) {
			//Ny kod
			sf::Vector3f pixel3dPos;
			pixel3dPos = forward * FOV;
			float partX = (x - (RESOLUTION.x / 2.0f)) / (float)RESOLUTION.x; partX *= 2.0f;
			pixel3dPos -= side * partX;
			pixel3dPos -= up * partY;
			tsr::Ray tmp(position, math.normalize(pixel3dPos));
			ray_array[y*RESOLUTION.x + x] = tmp; //KANSKE FEL HÄR (INTEEE KOLLAT)
		}
	}
}

void tsr::RayCamera::calculateRaysSIMD() //Kan man SIMD:a den här kanske?
{
	sf::Vector3f up, side, forward, plainDir;
	mth::GeometricMath math;
	forward = math.rotToVec(sf::Vector3f(rotation.x, rotation.y, 0));
	side = math.normalize(math.crossProduct(sf::Vector3f(0.0f,1.0f,0.0f),forward));
	up = math.normalize(math.crossProduct(forward, side));
	float aspectRatio = RESOLUTION.y / (float)RESOLUTION.x;
	float halfResX = RESOLUTION.x / 2.0f;
	float halfResY = RESOLUTION.y / 2.0f;
	float inverseResX = 1.0f / (float)RESOLUTION.x;
	float inverseResY = 1.0f / (float)RESOLUTION.y;
	for (int y = 0; y < RESOLUTION.y; y++) {
		float partY = 2.0f * aspectRatio*(y - halfResY) * inverseResY;
		for (int x = 0; x < RESOLUTION.x; x++) {
			//Förenkla följande kod:
			sf::Vector3f pixel3dPos;
			pixel3dPos = forward * FOV;
			float partX = 2.0f * (x - halfResX) * inverseResX;
			pixel3dPos -= side * partX;
			pixel3dPos -= up * partY;
			tsr::Ray tmp(position, math.normalize(pixel3dPos));
			ray_array[y*RESOLUTION.x + x] = tmp;
		}

	}
}

tsr::Ray * tsr::RayCamera::getRays()
{
	return ray_array;
}




void tsr::RayCamera::updatePos(bool buttonsDown[255])
{
	mth::GeometricMath math;
	double xzLen = std::cos(rotation.y);
	sf::Vector3f dirVec = math.rotToVec(sf::Vector3f(rotation.x, rotation.y,0));
	sf::Vector3f sideVec = math.normalize(math.crossProduct(sf::Vector3f(0,1,0), dirVec));
	//sf::Vector3f dirVec(std::cos(rotation.y) * std::cos(rotation.x), std::sin(rotation.y) * std::cos(rotation.x), std::sin(rotation.x));
	if (buttonsDown[22]) {
		position = sf::Vector3f(position.x + dirVec.x * 1.0f, position.y + dirVec.y *1.0f , position.z + dirVec.z * 1.0f);
	} 
	if (buttonsDown[18]) {
		position = sf::Vector3f(position.x - dirVec.x * 1.0f, position.y - dirVec.y *1.0f, position.z - dirVec.z * 1.0f);
	}
	if (buttonsDown[0]) {
		position += sideVec * 1.0f;
	}
	if (buttonsDown[3]) {
		position -= sideVec * 1.0f;
	}
}
