#include "TextureAllocator.hpp"
#include "Globals.hpp"
#include "Debugging.hpp"

bool hasEnding2(const String& fullString, const String& ending)
{
	if(fullString.length() >= ending.length())
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	else
		return false;
}

TextureAllocator::TextureAllocator(bool shouldSmoothTextures)
{
	defaultTex = "default";

	m_smoothTextures = shouldSmoothTextures;
	request(defaultTex);
}
TextureAllocator::~TextureAllocator()
{

}
void TextureAllocator::smoothTextures(bool smooth)
{
	m_smoothTextures = smooth;
	for(auto it = m_textures.begin(); it != m_textures.end(); ++it)//loop through all our textures to make them smooth
		it->second->setSmooth(m_smoothTextures);
}
sf::Texture* TextureAllocator::request(const String& rFilePath)
{
	if(hasEnding2(rFilePath, ".acfg"))
	{
		dout << FILELINE;
	}


	String fullPath = rFilePath + ".png";
	Map<String, sptr<sf::Texture> >::iterator it = m_textures.find(fullPath);

	if(it != m_textures.end())/**we already have it**/
		return &(*(it->second));
	else/**we dont have it loaded**/
	{
		sptr<sf::Texture> spTempTex(new sf::Texture);
		if(!spTempTex->loadFromFile(contentDir() + "textures/" + fullPath))/**cant be loaded**/
		{
			///ERROR LOG
			Print << "\nThere was an error loading the texture [" << contentDir() + fullPath << "].";
			return &*m_textures[defaultTex + ".png"];
		}
		m_textures[fullPath] = spTempTex;
		spTempTex->setSmooth(m_smoothTextures);
		return &*spTempTex;
	}
}
