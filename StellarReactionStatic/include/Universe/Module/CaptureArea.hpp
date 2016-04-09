#pragma once

#include "Sensor.hpp"

struct CaptureAreaData;

class CaptureArea : public Sensor
{
public:
	CaptureArea(const CaptureAreaData& rData);
	~CaptureArea();

	virtual void prePhysUpdate();
	bool isConflicted();
	int getCurrentTeam();
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

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		CaptureAreaData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new CaptureArea(copy);
	}
	virtual ModuleData* clone() const
	{
		return new CaptureAreaData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, CaptureAreaData);
};

