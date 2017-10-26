#pragma once

#include "Range.hpp"

struct RangeListData;
class RangeList;


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

class RangeModifierList : RangeData
{
public:
	RangeModifierList(int null)
	{
		assert(null == 0);
	}
	RangeModifierList(const RangeModifier& other)
	{
		*this = other;

		if(ranges != nullptr)
			tryApply();
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

	void tryApply()
	{
		if(hasAppliedModifiers == false)
		{
			for(int i = 0; i < RangeList::LAST_VAL; ++i)
				(*ranges)[i].modify(modifiers[i]);
		}

		hasAppliedModifiers = true;
	}
	void tryRemove()
	{
		if(hasAppliedModifiers)
		{
			for(int i = 0; i < RangeList::LAST_VAL; ++i)
				(*ranges)[i].modify(modifiers[i].negate());
		}

		hasAppliedModifiers = false;
	}

	bool hasAppliedModifiers;
	List<RangeModifier> modifiers;
	RangeList* ranges;
};
