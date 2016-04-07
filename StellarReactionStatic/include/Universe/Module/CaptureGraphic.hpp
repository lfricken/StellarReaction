#pragma once

#include "Module.hpp"
#include "Spinner.hpp"
#include "CaptureArea.hpp"

struct CaptureGraphicData;

class CaptureGraphic : public Module
{
public:
	CaptureGraphic(const CaptureGraphicData& rData);
	virtual ~CaptureGraphic();

	virtual void prePhysUpdate();
	virtual void postPhysUpdate();

protected:
	int m_light1; //default light
	int m_light2; //light corresponding to base owner

	bool m_onlyRed; //only red lights on

	sptr<GraphicsComponent> m_base_decor;
	std::vector<sptr<GraphicsComponent> > m_lights;
	//list of lights as follows
	//0 = white/neutral
	//1 = blue
	//2 = green
	//3 = yellow
	//4 = pink
	//5 = red

	CaptureArea* m_cap;

private:
	void redLights(bool on);
};



struct CaptureGraphicData : public ModuleData
{
	CaptureGraphicData() :
		baseDecor(),
		whiteLights(),
		blueLights(),
		greenLights(),
		yellowLights(),
		pinkLights(),
		redLights()
	{
		fixComp.density = 0.f;
	}
	SpinnerData baseDecor;
	SpinnerData whiteLights;
	SpinnerData blueLights;
	SpinnerData greenLights;
	SpinnerData yellowLights;
	SpinnerData pinkLights;
	SpinnerData redLights;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		CaptureGraphicData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new CaptureGraphic(copy);
	}
	virtual ModuleData* clone() const
	{
		return new CaptureGraphicData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, CaptureGraphicData);
};