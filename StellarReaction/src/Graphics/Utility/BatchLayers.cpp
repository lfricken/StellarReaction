#include "BatchLayers.hpp"

BatchLayers::BatchLayers()
{

}
BatchLayers::~BatchLayers()
{

}
QuadData BatchLayers::request(const std::string& rTexName, GraphicsLayer layer)
{
	return m_batches[layer].request(rTexName);
}
void BatchLayers::drawWorld(sf::RenderTarget& rTarget)
{
	for(auto it = m_batches.begin(); (it->first != GraphicsLayer::Overlay)&&(it != m_batches.end()); ++it)
		rTarget.draw(it->second);
}

void BatchLayers::drawOverlay(sf::RenderTarget& rTarget)
{
	for(auto it = m_batches.find(GraphicsLayer::Overlay); (it != m_batches.end()); ++it)
		rTarget.draw(it->second);
}
