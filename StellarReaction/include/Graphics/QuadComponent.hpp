#ifndef QUADCOMPONENT_HPP
#define QUADCOMPONENT_HPP

#include "GraphicsComponent.hpp"


struct QuadComponentData : public GraphicsComponentData
{
	QuadComponentData() :
		GraphicsComponentData()
	{

	}

};


class QuadComponent : public GraphicsComponent
{
public:
	QuadComponent(const QuadComponentData& rData);
	virtual ~QuadComponent();

	///void setAnimation();
	virtual void postUpdate();

protected:
private:
};

#endif // QUADCOMPONENT_HPP
