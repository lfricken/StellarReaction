#pragma once

#include "Sensor.hpp"

struct CaptureAreaData;

/*
* CaptureArea Class:
* Extends Sensor Class to implement capture areas
*/
class CaptureArea : public Sensor
{
public:
	CaptureArea(const CaptureAreaData& rData);
	~CaptureArea();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Is this capture area being contested?
	bool isConflicted();
	//Get the team that has captured this area.
	int getCurrentTeam();
	//Get current capture status.
	float getProgress();

protected:
private:
	Money m_value;
	float m_capPercent;
	float m_captureTime;//how long does it take to capture this?

	int m_currentTeam;//team that owns this point currently
	int m_capTeam;//team is currently capturing this point
	bool m_conflicted;
	float m_progress;
	float m_oldProgress;

	bool m_owned;

	Timer m_capTimer;
};

struct CaptureAreaData : public SensorData
{
	CaptureAreaData() :
		value(Money(5)),
		captureTime(10),
		capturePercent(0.75)
	{

	}

	Money value;
	float captureTime;
	float capturePercent;
	///Create CaptureArea object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		CaptureAreaData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new CaptureArea(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new CaptureAreaData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, CaptureAreaData);
};

