#pragma once

#include "Range.hpp"
#include "Debugging.hpp"

struct RangeListData;
class RangeList;

#define ENUMTOSTRING(key, ENUMNAME) \
	 if(key == ENUMNAME) return #ENUMNAME

class RangeList
{
public:

	static String getFieldName(int key)
	{
		ENUMTOSTRING(key, Energy);
		ENUMTOSTRING(key, Ballistic);
		ENUMTOSTRING(key, Missiles);

		ENUMTOSTRING(key, Zoom);
		ENUMTOSTRING(key, TeleportRange);
		ENUMTOSTRING(key, Stealth);

		ENUMTOSTRING(key, Shield);

		WARNING;
		return "ERROR_FIELDNAME";
	}

	enum
	{
		Energy = 0,
		Ballistic,
		Missiles,

		Zoom,
		TeleportRange,
		Stealth,
		
		Shield,

		LAST_VAL,
	};

	RangeList(const RangeListData& data);
	~RangeList();

	Range& operator[](int index)
	{
		return *ranges[index];
	}
	const Range& operator[](int index) const
	{
		return *ranges[index];
	}

	List<sptr<Range> > ranges;
};




struct RangeListData
{
	RangeListData()
	{
		for(int i = 0; i < RangeList::LAST_VAL; ++i)
			rangeData.push_back(RangeData());
	}

	List<RangeData> rangeData;

	virtual void loadJson(const Json::Value& root);
};


/// <summary>
/// Stores, and can apply, modifiers to a chunk, such as energy and storage.
/// </summary>
class RangeModifierList
{
public:
	RangeModifierList()
	{
		hasAppliedModifiers = false;

		for(int i = 0; i < RangeList::LAST_VAL; ++i)
			modifiers.push_back(RangeModifier());
	}
	RangeModifierList(const RangeModifierList& other)
	{
		hasAppliedModifiers = false;

		for(int i = 0; i < RangeList::LAST_VAL; ++i)
			modifiers.push_back(RangeModifier());

		*this = other;
	}
	~RangeModifierList()
	{
		if(ranges != nullptr)
			tryRemove();
	}
	RangeModifier& operator[](int index)
	{
		return modifiers[index];
	}
	const RangeModifier& operator[](int index) const
	{
		return modifiers[index];
	}
	/// <summary>
	/// Increase values that increase over time.
	/// </summary>
	void applyRateModifiers(float deltaTime)
	{
		for(int i = 0; i < RangeList::LAST_VAL; ++i)
			(*ranges)[i].changeValue(deltaTime * modifiers[i].ModifierPerSecond);
	}
	/// <summary>
	/// Applies these changes to the ranges.
	/// </summary>
	void tryApply()
	{
		if(hasAppliedModifiers == false)
		{
			for(int i = 0; i < RangeList::LAST_VAL; ++i)
				(*ranges)[i].modify(modifiers[i]);
		}

		hasAppliedModifiers = true;
	}
	/// <summary>
	/// Removes these changes to the ranges.
	/// </summary>
	void tryRemove()
	{
		if(hasAppliedModifiers)
		{
			for(int i = 0; i < RangeList::LAST_VAL; ++i)
				(*ranges)[i].modify(modifiers[i].negate());
		}

		hasAppliedModifiers = false;
	}

	bool hasAppliedModifiers; // True if this instance has already applied modifiers.
	List<RangeModifier> modifiers; // List of modifying values.
	RangeList* ranges; // The ranges to apply these modifiers to.

	virtual void loadJson(const Json::Value& root);
};
