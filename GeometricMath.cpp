#include "GeometricMath.h"
#include <immintrin.h>

mth::GeometricMath::GeometricMath()
{
	x = sf::Vector3f(1, 0, 0);
	y = sf::Vector3f(0, 1, 0);
	z = sf::Vector3f(0, 0, 1);
}

sf::Vector3f mth::GeometricMath::crossProduct(sf::Vector3f a, sf::Vector3f b)
{
	sf::Vector3f output(0, 0, 0);
	output.x = (a.y*b.z) - (a.z*b.y);
	output.y = (a.z*b.x) - (a.x*b.z);
	output.z = (a.x*b.y) - (a.y*b.x);
	return output;
}

sf::Vector3f mth::GeometricMath::normalize(sf::Vector3f vec)
{
	float mag = std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	return sf::Vector3f(vec.x/mag, vec.y/mag, vec.z/mag);
}

sf::Vector3f mth::GeometricMath::rotToVec(sf::Vector3f rot)
{
	double xzLen = std::cos(rot.y);

	return sf::Vector3f(xzLen * std::cos(rot.x), std::sin(rot.y), xzLen* std::sin(-rot.x));
}

float mth::GeometricMath::dist(sf::Vector3f point1, sf::Vector3f point2) //__declspec(noinline) <-- för debugging
{		
	return std::sqrt(
		std::pow(point1.x - point2.x, 2) +
		std::pow(point1.y - point2.y, 2) +
		std::pow(point1.z - point2.z, 2)
	);
}

float mth::GeometricMath::distFast(sf::Vector3f point1, sf::Vector3f point2)
{
	//Använder fast invers square root iställer för std::sqrt. Ger en avrundning som är ok.
	float dx = (point1.x - point2.x), dy= (point1.y - point2.y), dz = (point1.z - point2.z);
	float sum = dx*dx + dy*dy + dz*dz;//(point1.x - point2.x, 2) + std::pow(point1.y - point2.y, 2) + std::pow(point1.z - point2.z, 2);
	return (inverseSqr(sum)*sum);
}

float mth::GeometricMath::distSqr(sf::Vector3f point1, sf::Vector3f point2)
{
	return (
			std::pow(point1.x - point2.x, 2) +
			std::pow(point1.y - point2.y, 2) +
			std::pow(point1.z - point2.z, 2)
			);
}

float mth::GeometricMath::inverseSqr(float input)
{
	float x = input;
	#ifndef _MSC_VER //Optimal
		float result;
		asm( //Note AT&T order
			#ifdef __AVX__
			"vrsqrtss %1, %1, %0"
			#else
			"rsqrtss %1, %0"
			#endif
			: "=x"(result)
			: "x"(x)
		);
		return result;
		#else //TODO: not optimal when in AVX mode or when not inlined
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ps1(x)));
	#endif
}
