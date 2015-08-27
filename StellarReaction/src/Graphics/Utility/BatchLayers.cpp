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
void BatchLayers::drawBackground(sf::RenderTarget& rTarget)
{
	{
		auto it = m_batches.find(GraphicsLayer::BackgroundUnmoving1);
		if(it != m_batches.end())
			rTarget.draw(it->second);
	}
	{
		auto it = m_batches.find(GraphicsLayer::BackgroundUnmoving2);
		if(it != m_batches.end())
			rTarget.draw(it->second);
	}
}
void BatchLayers::drawWorld(sf::RenderTarget& rTarget)
{
	for(auto it = m_batches.begin(); (it != m_batches.end()) && (it->first != GraphicsLayer::Overlay); ++it)
	{
		if(it->first != GraphicsLayer::BackgroundUnmoving1 && it->first != GraphicsLayer::BackgroundUnmoving2)
			rTarget.draw(it->second);
	}
}
void BatchLayers::drawOverlay(sf::RenderTarget& rTarget)
{
	{
		auto it = m_batches.find(GraphicsLayer::Overlay);
		if(it != m_batches.end())
			rTarget.draw(it->second);
	}
	{
		auto it = m_batches.find(GraphicsLayer::OverlayMiddle);
		if(it != m_batches.end())
			rTarget.draw(it->second);
	}
}