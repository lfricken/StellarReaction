#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "Module.hpp"

struct SensorData;

class Sensor : public Module
{
public:
	Sensor(const SensorData& rData);
	virtual ~Sensor();

	virtual void prePhysUpdate();

	virtual void entered(FixtureComponent* pOther);//called when a
	virtual void exited(FixtureComponent* pOther);

protected:
	void startContactCB(FixtureComponent* pOther) final;
	void endContactCB(FixtureComponent* pOther) final;

	std::vector<FixtureComponent*> m_guests;
private:

};



struct SensorData : public ModuleData
{
	SensorData()
	{
		fixComp.isSensor = true;
		fixComp.density = 0.f;
	}

	virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
	{
		SensorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		return new Sensor(copy);
	}
	virtual ModuleData* clone() const
	{
		return new SensorData(*this);
	}
};



#endif // SENSOR_HPP
