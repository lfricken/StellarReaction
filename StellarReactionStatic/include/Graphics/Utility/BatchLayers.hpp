#pragma once

#include "QuadBatchContainer.hpp"
#include "GraphicsLayer.hpp"
#include "NonCopyable.hpp"

/// \brief Stores a Batch for each layer.
/// 
/// Allows more than one layer of drawing.
/// So all the Textures of one layer get drawn
/// then the next layer, etc.
/// To control layering, examine GraphicsLayer.hpp
class BatchLayers : NonCopyable
{
public:
	BatchLayers();
	virtual ~BatchLayers();

	/// Return an array and indexes for drawing a rectangle.
	QuadData request(const String& rTexName, GraphicsLayer layer);

	/// Draw everything that is not the Overlay in order.
	void drawWorld(sf::RenderTarget& rTarget);
	/// Draw the overlay in order.
	void drawOverlay(sf::RenderTarget& rTarget);


protected:
private:
	/// List of each layer, and its batch.
	std::map<GraphicsLayer, QuadBatchContainer> m_batches;
};
