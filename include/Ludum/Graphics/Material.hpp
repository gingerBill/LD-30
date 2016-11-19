#ifndef Ludum_Graphics_Material_hpp
#define Ludum_Graphics_Material_hpp

#include <Ludum/Graphics/Texture.hpp>

namespace gb
{
	struct Material
	{
		Texture* texture;
		sf::Color color;
	};
}

#endif
