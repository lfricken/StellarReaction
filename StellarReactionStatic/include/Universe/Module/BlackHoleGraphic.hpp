#pragma once

#include "Module.hpp"
#include "Spinner.hpp"

struct BlackHoleGraphicData;

class BlackHoleGraphic : public Module
{
public:
	BlackHoleGraphic(const BlackHoleGraphicData& rData);
	virtual ~BlackHoleGraphic();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();

protected:
	std::vector<sptr<GraphicsComponent> > m_decors;
};



struct BlackHoleGraphicData : public ModuleData
{
	BlackHoleGraphicData() :
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

	///Create BlackHoleGraphic object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		BlackHoleGraphicData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new BlackHoleGraphic(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new BlackHoleGraphicData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, BlackHoleGraphicData);
};
