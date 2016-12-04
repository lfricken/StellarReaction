#pragma once

#include "stdafx.hpp"

/// \brief Loads textures once and keeps the only copy
/// 
/// so we don't have to waste memory with 1 texture copied repeatedly
/// (this works since nothing modifies the texture, just how it is drawn)
class TextureAllocator
{
public:
	TextureAllocator(bool shouldSmoothTextures);
	~TextureAllocator();

	/// Set whether all textures should be smoothed. An SFML feature.
	void smoothTextures(bool smooth);
	/// Get a texture pointer.
	/// Will load the texture if necessary.
	sf::Texture* request(const String& rFilePath);
protected:
private:
	/// Name of default texture to return
	/// if the requested one cannot be found.
	String defaultTex;
	bool m_smoothTextures;
	/// Map of texture name to the texture.
	std::map<String, sptr<sf::Texture> > m_textures;
};
