#pragma once

#include "Range.hpp"

struct RangeListData;


struct RangeDataModifierList : RangeData
{
	RangeDataModifierList();

	RangeDataModifier& operator[](int index)
	{
		return modifiers[index];
	}
	const RangeDataModifier& operator[](int index) const
	{
		return modifiers[index];
	}

	List<RangeDataModifier> modifiers;
};

class RangeList
{
public:

	enum
	{
		Energy = 0,
		Ballistic,
		Missiles,

		Zoom,
		Teleport,
		Stealth,

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
	void applyModifiers(const RangeDataModifierList& mods)
	{
		for(int i = 0; i < RangeList::LAST_VAL; ++i)
			ranges[i]->modify(mods[i]);
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

