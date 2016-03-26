#ifndef POOL_HPP
#define POOL_HPP

#include "JSON.hpp"

typedef int Missiles;
typedef float Energy;
typedef float Ballistic;
typedef float Zoom;


///Generic type for common values
template <typename T>
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
	void loadJson(const Json::Value& root)
	{
		GETJSON(Max);
		GETJSON(Min);
		GETJSON(Value);
	}
};




/**POOLS FOR COMMON DATA**/
template <typename T>
class Pool
{
public:
	Pool(const PoolData<T>& rData)
	{
		m_max = rData.Max;
		m_min = rData.Min;
		m_value = rData.Value;
	}
	virtual ~Pool() {}


	void changeMax(T changeVal)
	{
		setMax(m_max + changeVal);
	}
	void changeMin(T changeVal)
	{
		setMin(m_min + changeVal);
	}
	void changeValue(T changeVal)
	{
		setValue(m_value + changeVal);
	}



	void setMax(T newVal)
	{
		if(newVal >= m_min)
			m_max = newVal;
		setValue(m_value);//update our value
	}
	void setMin(T newVal)
	{
		if(newVal <= m_max)
			m_min = newVal;
		setValue(m_value);//update our value
	}
	void setValue(T newVal)
	{
		if(newVal > m_max)
			newVal = m_max;
		else if(newVal < m_min)
			newVal = m_min;
		m_value = newVal;
	}



	T getMax() const
	{
		return m_max;
	}
	T getMin() const
	{
		return m_min;
	}
	T getValue() const
	{
		return m_value;
	}

protected:
private:

	T m_max;
	T m_min;
	T m_value;//our current value
};

struct PoolCollection
{
	Pool<Ballistic>* ballisticPool;
	Pool<Missiles>* missilePool;
	Pool<Energy>* energyPool;
	Pool<Zoom>* zoomPool;
};

#endif // POOL_HPP
