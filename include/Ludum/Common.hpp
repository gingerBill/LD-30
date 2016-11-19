#ifndef Ludum_Common_hpp
#define Ludum_Common_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define TAU   6.28318530718
#define SQRT2 1.41421356237
#define SQRT3 1.73205080757
#define PHI   1.61803398875

namespace gb
{
	template <typename T>
	inline void println(const T& t)
	{
		std::cout << t << std::endl;
	}

	// Read the contents of a file into an array of char
	bool getFileContents(const std::string& filename, std::vector<char>& buffer);

	inline glm::mat4 translate(float x, float y, float z)
	{
		return glm::translate(glm::mat4(), glm::vec3(x, y, z));
	}
	inline glm::mat4 translate(const glm::vec3& position)
	{
		return translate(position.x, position.y, position.z);
	}

	inline glm::mat4 scale(float x, float y, float z)
	{
		return glm::scale(glm::mat4(), glm::vec3(x, y, z));
	}
	inline glm::mat4 scale(const glm::vec3& s)
	{
		return scale(s.x, s.y, s.z);
	}

	inline glm::mat4 rotate(float angle, const glm::vec3& axis)
	{
		return glm::rotate(glm::mat4(), angle, axis);
	}

	inline float randomFloat(float lower, float upper)
	{
		float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

		r *= upper-lower;
		r += lower;

		return r;
	}

	// for OS X Resource Path
	std::string resourcePath();

	// 1 tile = 1/16 x 1/16 of texture, width and height and coefficients for texture size
	glm::vec4 getTextureCoords(int positionNumber, int width = 1, int height = 1);
}

#endif
