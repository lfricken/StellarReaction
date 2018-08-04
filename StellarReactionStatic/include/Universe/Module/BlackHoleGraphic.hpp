#pragma once

#include "Module.hpp"
#include "Spinner.hpp"

struct BlackHoleGraphicData;

/// A Module with the specific purpose of a making graphics appear.
class BlackHoleGraphic : public Module
{
public:
	BlackHoleGraphic(const BlackHoleGraphicData& rData);
	virtual ~BlackHoleGraphic();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();

protected:
	List<sptr<GraphicsComponent> > m_decors;
};


/// Initialize BlackHoleGraphic.
struct BlackHoleGraphicData : public ModuleData
{
	BlackHoleGraphicData() :
		innerDecor(),
		middleDecor(),
		outerDecor()
	{
		fixComp.mass = 0.f;
		innerDecor.layer = GraphicsLayer::BackgroundClose;
		middleDecor.layer = GraphicsLayer::BackgroundClose;
		outerDecor.layer = GraphicsLayer::BackgroundClose;
	}

	SpinnerData innerDecor;
	SpinnerData middleDecor;
	SpinnerData outerDecor;

	///Create BlackHoleGraphic object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<BlackHoleGraphic, BlackHoleGraphicData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new BlackHoleGraphicData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
