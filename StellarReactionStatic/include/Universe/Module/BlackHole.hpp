#pragma once

#include "Module.hpp"
#include "Spinner.hpp"

struct BlackHoleData;

class BlackHole : public Module
{
public:
	BlackHole(const BlackHoleData& rData);
	virtual ~BlackHole();

	virtual void prePhysUpdate();

protected:
	std::vector<sptr<GraphicsComponent> > m_decors;
};



struct BlackHoleData : public ModuleData
{
	BlackHoleData() :
		innerDecor(),
		middleDecor(),
		outerDecor()
	{
		fixComp.density = 0.f;
		innerDecor.layer = GraphicsLayer::BackgroundClose;
		middleDecor.layer = GraphicsLayer::BackgroundClose;
		outerDecor.layer = GraphicsLayer::BackgroundClose;
	}

	SpinnerData innerDecor;
	SpinnerData middleDecor;
	SpinnerData outerDecor;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		BlackHoleData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new BlackHole(copy);
	}
	virtual ModuleData* clone() const
	{
		return new BlackHoleData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, BlackHoleData);
};
