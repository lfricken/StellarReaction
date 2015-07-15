#ifndef QUADBATCHCONTAINER_HPP
#define QUADBATCHCONTAINER_HPP

#include "stdafx.hpp"
#include "TextureAllocator.hpp"

struct QuadData
{
	int firstElement;//marks the first vertex element that we own
	sf::VertexArray* vertexList;//list of all vertices for this textures
};


struct Batch
{
	const sf::Texture* pTexture;
	sf::VertexArray vertexList;
};


//stores all quads for drawing of all graphics objects
class QuadBatchContainer : public sf::Drawable
{
public:
	QuadBatchContainer();
	virtual ~QuadBatchContainer();

	QuadData request(const std::string& rTexName);//gives 4 texture vertexes
	void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const;//draws all the quads with their textures!

protected:
private:

	std::map<std::string, sptr<Batch> > m_vertexLists;//contains all QUAD vertexes for all
};

#endif // QUADBATCHCONTAINER_HPP
