#pragma once

#include "stdafx.hpp"

struct RangeData
{
	RangeData() :
		Max(0),
		Min(0),
		Value(0)
	{
	
	}

	float Max;
	float Min;
	float Value;

	///Fill this object with data from a json file.
	void loadJson(const Json::Value& root);
};


struct RangeModifier : RangeData
{
	RangeModifier() :
		ModifierPerSecond(0)
	{

	}

	RangeModifier negate() const
	{
		RangeModifier copy = *this;
		copy.Min = -Min;
		copy.Max = -Max;
		copy.Value = -Value;
		copy.ModifierPerSecond = -ModifierPerSecond;
		return copy;
	}

	float ModifierPerSecond;

	void loadJson(const Json::Value& root);

};


/// <summary>
/// Generic container to limit value ranges.
/// </summary>
class Range
{
public:

	Range(const RangeData& rData)
	{
		m_min = rData.Min;
		setMax(rData.Max);
		setValue(rData.Value);
	}
	virtual ~Range() 
	{
	
	}

	/// <summary>
	/// Consumes the value without capping if possible. Returns true if it does. False means it did not consume.
	/// </summary>
	bool tryChange(float val)
	{
		float oldValue = getValue();

		if(changeValue(val))
		{
			return true;
		}
		else
		{
			setValue(oldValue);
			return false;
		}
	}
	///Change max by amount. Defaults to addition rather than subtraction.
	void changeMax(float changeVal)
	{
		setMax(m_max + changeVal);
	}
	///Change min by amount. Defaults to addition rather than subtraction.
	void changeMin(float changeVal)
	{
		setMin(m_min + changeVal);
	}
	/// <summary>
	/// Change value by amount. Defaults to addition rather than subtraction.
	/// Return true if it changed the entire value without getting capped.
	/// </summary>
	bool changeValue(float changeVal)
	{
		return setValue(m_value + changeVal);
	}


	///Set max value. Will be equal to min if passed value is less than min.
	///Will update current value to cap at max.
	void setMax(float newVal)
	{
		if(newVal >= m_min)
			m_max = newVal;

		reevaluate();
	}
	///Set min value. Will be equal to max if passed value is greater than min.
	///Will update current value to cap at min.
	void setMin(float newVal)
	{
		if(newVal <= m_max)
			m_min = newVal;

		reevaluate();
	}
	/// <summary>
	/// Set value. Will be capped. Return true if it wasn't capped.
	/// </summary>
	bool setValue(float newVal)
	{
		bool wasNotCapped = true;
		if(newVal > m_max)
		{
			newVal = m_max;
			wasNotCapped = false;
		}
		else if(newVal < m_min)
		{
			newVal = m_min;
			wasNotCapped = false;
		}

		m_value = newVal;
		return wasNotCapped;
	}


	///Returns max value.
	float getMax() const
	{
		return m_max;
	}
	/// <summary>
	/// Returns the percentage between min and max 0 - 1.
	/// </summary>
	float getPercent() const
	{
		float max = m_max - m_min;
		float cur = m_value - m_min;

		return cur / max;
	}
	///Returns min value.
	float getMin() const
	{
		return m_min;
	}
	///Returns current value.
	float getValue() const
	{
		return m_value;
	}

	void modify(const RangeModifier& other)
	{
		m_min += other.Min;
		m_max += other.Max;
		m_value += other.Value;

		reevaluate();
	}

protected:
private:

	void reevaluate()
	{
		assert(m_max >= m_min);
		if(m_value > m_max)
			m_value = m_max;
		else if(m_value < m_min)
			m_value = m_min;
	}

	float m_max;///Max value.
	float m_min;///Min value.
	float m_value;///Current value.
};


