#ifndef TEXTUREALLOCATOR_H
#define TEXTUREALLOCATOR_H

#include "stdafx.hpp"

/// <summary>
/// Loads textures once and keeps the only copy
/// so we don't have to waste memory with 1 texture copied repeatedly
/// (this works since nothing modifies the texture, just how it is drawn)
/// </summary>
class TextureAllocator
{
public:
	TextureAllocator(bool shouldSmoothTextures);
	~TextureAllocator();

	void smoothTextures(bool smooth);//loops through our textures and either smooths them or not, and sets m_smoothTextures apropriatly
	sf::Texture* request(const std::string& rFilePath);
protected:
private:
	std::string defaultTex;
	bool m_smoothTextures;
	std::map<std::string, sptr<sf::Texture> > m_textures;
};

#endif // TEXTUREALLOCATOR_H
