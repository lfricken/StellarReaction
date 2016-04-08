#ifndef QUADBATCHCONTAINER_HPP
#define QUADBATCHCONTAINER_HPP

#include "stdafx.hpp"
#include "TextureAllocator.hpp"
#include "NonCopyable.hpp"

/// <summary>
/// For a particular texture, a pointer to our verts
/// </summary>
struct QuadData
{
	int firstElement;//marks the first vertex element that we own
	sf::VertexArray* vertexList;//list of all vertices for this textures
};

/// <summary>
/// Holds a pointer to a texture and a list of vertices
/// By having all the vertices for a texture in an array, it becomes much faster to draw them
/// </summary>
struct Batch
{
	const sf::Texture* pTexture;
	sf::VertexArray vertexList;
};


/// <summary>
/// Stores Batches for each texture requested
/// </summary>
class QuadBatchContainer : public sf::Drawable, NonCopyable
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
