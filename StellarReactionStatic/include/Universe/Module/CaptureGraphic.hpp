#pragma once

#include "Module.hpp"
#include "Spinner.hpp"
#include "CaptureArea.hpp"

struct CaptureGraphicData;



/// CaptureArea graphical components are used to show which team has captured a capture area.
class CaptureGraphic : public Module
{
public:
	CaptureGraphic(const CaptureGraphicData& rData);
	virtual ~CaptureGraphic();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();

protected:
	int m_light1; //default light
	int m_light2; //light corresponding to base owner

	bool m_onlyRed; //only red lights on

	sptr<GraphicsComponent> m_base_decor;
	List<sptr<GraphicsComponent> > m_lights;
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


/// Initialize CaptureGraphic
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
		fixComp.mass = 0.f;
	}
	SpinnerData baseDecor;
	SpinnerData whiteLights;
	SpinnerData blueLights;
	SpinnerData greenLights;
	SpinnerData yellowLights;
	SpinnerData pinkLights;
	SpinnerData redLights;
	///Create CaptureGraphics object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<CaptureGraphic, CaptureGraphicData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new CaptureGraphicData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
