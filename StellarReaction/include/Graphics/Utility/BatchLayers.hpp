#ifndef BATCHLAYERS_HPP
#define BATCHLAYERS_HPP

#include "QuadBatchContainer.hpp"
#include "GraphicsLayer.hpp"

class BatchLayers
{
public:
	BatchLayers();
	virtual ~BatchLayers();

	QuadData request(const std::string& rTexName, GraphicsLayer layer);//gives 4 texture vertexes
	void drawWorld(sf::RenderTarget& rTarget);
	void drawOverlay(sf::RenderTarget& rTarget);


protected:
private:

	std::map<GraphicsLayer, QuadBatchContainer> m_batches;
};

#endif // BATCHLAYERS_HPP
