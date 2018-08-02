#include "QuadBatchContainer.hpp"

#include "Globals.hpp"



QuadBatchContainer::QuadBatchContainer()
{

}
QuadBatchContainer::~QuadBatchContainer()
{

}
QuadData QuadBatchContainer::request(const String& rTexName)///IMPLEMENT VERTEX FREEING AND REUSE FOR DESTROYED THINGS
{
	sf::Vertex vert(sf::Vector2f(0, 0), sf::Color::White);
	QuadData data;
	auto it = m_vertexLists.find(rTexName);

	if(it != m_vertexLists.end())//we found it!
	{
		data.vertexList = &(it->second->vertexList);
		data.firstElement = it->second->vertexList.getVertexCount();

		it->second->vertexList.append(vert);
		it->second->vertexList.append(vert);
		it->second->vertexList.append(vert);
		it->second->vertexList.append(vert);
	}
	else//It's a new texture!
	{
		m_vertexLists[rTexName] = sptr<Batch>(new Batch);
		m_vertexLists[rTexName]->pTexture = getGame()->getTextureAllocator().request(rTexName);
		it = m_vertexLists.find(rTexName);
		it->second->vertexList.setPrimitiveType(sf::Quads);

		it->second->vertexList.append(vert);
		it->second->vertexList.append(vert);
		it->second->vertexList.append(vert);
		it->second->vertexList.append(vert);

		data.firstElement = 0;
		data.vertexList = &(m_vertexLists[rTexName]->vertexList);
	}

	return data;
}
void QuadBatchContainer::draw(sf::RenderTarget& rTarget, sf::RenderStates states) const
{
	for(auto it = m_vertexLists.cbegin(); it != m_vertexLists.cend(); ++it)
	{
		states.texture = it->second->pTexture;
		rTarget.draw(it->second->vertexList, states);
	}
}















