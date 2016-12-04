#include "TextureAllocator.hpp"
#include "Globals.hpp"

TextureAllocator::TextureAllocator(bool shouldSmoothTextures)
{
	defaultTex = "textures/default.png";

	m_smoothTextures = shouldSmoothTextures;

	sptr<sf::Texture> spTempTex(new sf::Texture);
	if(!spTempTex->loadFromFile(contentDir() + defaultTex))/**cant be loaded**/
	{
		///ERROR LOG
		Print << "\nThere was an error loading the texture [" << defaultTex << "].";
	}
	spTempTex->setSmooth(m_smoothTextures);
	m_textures[defaultTex] = spTempTex;
}
TextureAllocator::~TextureAllocator()
{
	Print << "\nTexAlloc Destroying...";
}
void TextureAllocator::smoothTextures(bool smooth)
{
	m_smoothTextures = smooth;
	for(auto it = m_textures.begin(); it != m_textures.end(); ++it)//loop through all our textures to make them smooth
		it->second->setSmooth(m_smoothTextures);
}
sf::Texture* TextureAllocator::request(const String& rFilePath)
{
	Map<String, sptr<sf::Texture> >::iterator it = m_textures.find(rFilePath);

	if(it != m_textures.end())/**we already have it**/
		return &(*(it->second));
	else/**we dont have it loaded**/
	{
		sptr<sf::Texture> spTempTex(new sf::Texture);
		if(!spTempTex->loadFromFile(contentDir() + "textures/" + rFilePath))/**cant be loaded**/
		{
			///ERROR LOG
			Print << "\nThere was an error loading the texture [" << contentDir() + rFilePath << "].";
			return &*m_textures[defaultTex];
		}
		m_textures[rFilePath] = spTempTex;
		spTempTex->setSmooth(m_smoothTextures);
		return &*spTempTex;
	}
}
