#pragma once

#include "Sensor.hpp"

struct CaptureAreaData;



/// Capture areas are regions that provide money to the team that captures them and are a victory condition in certain game modes.
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
	Team getCurrentTeam();
	//Get current capture status.
	float getProgress();

protected:
private:
	Money m_value;
	float m_capPercent;
	float m_captureTime;//how long does it take to capture this?

	Team m_currentTeam;//team that owns this point currently
	Team m_capTeam;//team is currently capturing this point
	bool m_conflicted;
	float m_progress;
	float m_oldProgress;

	bool m_owned;

	Timer m_capTimer;
};

/// Initialize CaptureArea.
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
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<CaptureArea, CaptureAreaData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new CaptureAreaData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

