#ifndef QUADCOMPONENT_HPP
#define QUADCOMPONENT_HPP

#include "GraphicsComponent.hpp"


struct QuadComponentData : public GraphicsComponentData
{
	QuadComponentData() :
		GraphicsComponentData()
	{

	}
	virtual void loadJson(const Json::Value& root);
};

/// <summary>
/// Generic display type
/// </summary>
class QuadComponent : public GraphicsComponent
{
public:
	QuadComponent(const QuadComponentData& rData);
	virtual ~QuadComponent();

	virtual void postUpdate();

protected:
private:
};

#endif // QUADCOMPONENT_HPP
