#pragma once

#include "stdafx.hpp"
#include "TextureAllocator.hpp"
#include "NonCopyable.hpp"


/// For a particular texture, a pointer to our verts that get drawn.
struct QuadData
{
	/// Marks the first vertex element that we own.
	int firstElement;//
	/// List of all vertices for this texture.
	sf::VertexArray* vertexList;
};



/// \brief Holds a pointer to a texture and a list of vertices.
/// 
/// By having all the vertices for a texture in an array, it becomes much faster to draw them.
struct Batch
{
	/// The texture to draw.
	const sf::Texture* pTexture;
	/// The array that holds all the vertexes.
	sf::VertexArray vertexList;
};



/// Stores a Batch for each Texture.
class QuadBatchContainer : public sf::Drawable, NonCopyable
{
public:
	QuadBatchContainer();
	virtual ~QuadBatchContainer();

	/// Returns 4 vertices for a a particular texture.
	QuadData request(const String& rTexName);
	/// Draws all the vertices with their textures.
	void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const;

protected:
private:
	/// Map of texture names to vertex lists and texture (Batch).
	std::map<String, sptr<Batch> > m_vertexLists;

};


