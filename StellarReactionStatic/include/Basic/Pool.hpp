#ifndef POOL_HPP
#define POOL_HPP

#include "JSON.hpp"

typedef int Missiles;
typedef float Energy;
typedef float Ballistic;
typedef float Zoom;



template <typename T>
///Constructor data for Pool
struct PoolData
{
	PoolData() :
		Max(0),
		Min(0),
		Value(0)
	{

	}

	T Max;
	T Min;
	T Value;

	template <typename T>
	///Fill this object with data from a json file.
	void loadJson(const Json::Value& root)
	{
		GETJSON(Max);
		GETJSON(Min);
		GETJSON(Value);
	}
};




template <typename T>
/// \brief Generic container to limit value ranges.
///
/// Good for any attribute that has a min value, current value, max value.
/// The functions prevent the current value from escaping min-max, as well as prevent
/// min-max from being reversed, such as min=3, max=2
class Pool
{
public:
	Pool(const PoolData<T>& rData)
	{
		m_min = rData.Min;
		setMax(rData.Max);
		setValue(rData.Value);
	}
	virtual ~Pool() {}

	///Change max by amount. Defaults to addition rather than subtraction.
	void changeMax(T changeVal)
	{
		setMax(m_max + changeVal);
	}
	///Change min by amount. Defaults to addition rather than subtraction.
	void changeMin(T changeVal)
	{
		setMin(m_min + changeVal);
	}
	///Change value by amount. Defaults to addition rather than subtraction.
	void changeValue(T changeVal)
	{
		setValue(m_value + changeVal);
	}


	///Set max value. Will be equal to min if passed value is less than min.
	///Will update current value to cap at max.
	void setMax(T newVal)
	{
		if(newVal >= m_min)
			m_max = newVal;
		setValue(m_value);//update our value
	}
	///Set min value. Will be equal to max if passed value is greater than min.
	///Will update current value to cap at min.
	void setMin(T newVal)
	{
		if(newVal <= m_max)
			m_min = newVal;
		setValue(m_value);//update our value
	}
	///Set current value. Will be capped by max and min.
	void setValue(T newVal)
	{
		if(newVal > m_max)
			newVal = m_max;
		else if(newVal < m_min)
			newVal = m_min;
		m_value = newVal;
	}


	///Returns max value.
	T getMax() const
	{
		return m_max;
	}
	///Returns min value.
	T getMin() const
	{
		return m_min;
	}
	///Returns current value.
	T getValue() const
	{
		return m_value;
	}

protected:
private:

	T m_max;///Max value.
	T m_min;///Min value.
	T m_value;///Current value.
};

///The set of pools a Chunk has.
struct PoolCollection
{
	///Ballistic Ammunition
	Pool<Ballistic>* ballisticPool;
	///Missile Ammunition
	Pool<Missiles>* missilePool;
	///Energy Reservoir
	Pool<Energy>* energyPool;
	///Limits on camera zoom.
	Pool<Zoom>* zoomPool;
};

#endif // POOL_HPP
