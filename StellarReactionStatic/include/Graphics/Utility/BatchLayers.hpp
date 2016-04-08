#ifndef BATCHLAYERS_HPP
#define BATCHLAYERS_HPP

#include "QuadBatchContainer.hpp"
#include "GraphicsLayer.hpp"
#include "NonCopyable.hpp"

/// <summary>
/// allows more than one layer of drawing. So all the Textures of one layer get drawn
/// then the next layer, etc.
/// To control layering, examine GraphicsLayer.hpp
/// </summary>
class BatchLayers : NonCopyable
{
public:
	BatchLayers();
	virtual ~BatchLayers();

	QuadData request(const std::string& rTexName, GraphicsLayer layer);//gives 4 texture vertexes
	void drawBackground(sf::RenderTarget& rTarget);
	void drawWorld(sf::RenderTarget& rTarget);
	void drawOverlay(sf::RenderTarget& rTarget);


protected:
private:

	std::map<GraphicsLayer, QuadBatchContainer> m_batches;
};

#endif // BATCHLAYERS_HPP
