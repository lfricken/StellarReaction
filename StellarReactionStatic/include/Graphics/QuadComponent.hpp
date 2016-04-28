#pragma once

#include "GraphicsComponent.hpp"

/// Used to initialze a QuadComponent
struct QuadComponentData : public GraphicsComponentData
{
	QuadComponentData() :
		GraphicsComponentData()
	{

	}

	/// Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};


/// \brief Generic game graphics object.
///
/// Contains 4 vertices.
class QuadComponent : public GraphicsComponent
{
public:
	QuadComponent(const QuadComponentData& rData);
	virtual ~QuadComponent();

	/// Forced to override pure virtual function.
	virtual void postUpdate();

protected:
private:
};

