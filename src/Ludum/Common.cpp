#include <Ludum/Common.hpp>

#ifdef __APPLE__
	#include <CoreFoundation/CoreFoundation.h>
#endif

namespace gb
{
	std::string resourcePath()
	{
	#ifdef __APPLE__
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef resourcesURL  = CFBundleCopyResourcesDirectoryURL(mainBundle);
		char path[PATH_MAX];
		if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
		{
			// error!
		}
		CFRelease(resourcesURL);

		return std::string(path) + "/";
	#elif
		return "";
	#endif
	}

	bool getFileContents(const std::string& filename, std::vector<char>& buffer)
	{
		std::ifstream file(filename.c_str(), std::ios_base::binary);
		if (file)
		{
			file.seekg(0, std::ios_base::end);
			std::streamsize size = file.tellg();
			if (size > 0)
			{
				file.seekg(0, std::ios_base::beg);
				buffer.resize(static_cast<std::size_t>(size));
				file.read(&buffer[0], size);
			}
			buffer.push_back('\0');
			return true;
		}
		else
		{
			return false;
		}
	}

	glm::vec4 getTextureCoords(int positionNumber, int width, int height)
	{
		// hex
		//  0  1  2  3 ..  F
		// 10 11 12 13 .. 1F
		// ..
		// F0 F1 F2 F3 .. FF

		// 16 x 16 tiles in texture

		glm::vec4 uv(0, 0, 0, 0);

		uv[0] = (positionNumber % 16) / 16.0f;
		uv[1] = 1.0f - (positionNumber / 16) / 16.0f;
		uv[2] = uv[0] + width*(1.0f / 16.0f);
		uv[3] = uv[1] - height*(1.0f / 16.0f);

		return glm::vec4(uv[0], uv[3], uv[2], uv[1]);
	}
}
