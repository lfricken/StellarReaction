#include "RangeList.hpp"
#include "JSON.hpp"





RangeList::RangeList(const RangeListData& data)
{
	for each (RangeData data in data.rangeData)
	{
		ranges.push_back(sptr<Range>(new Range(data)));
	}
}
RangeList::~RangeList()
{

}

void RangeListData::loadJson(const Json::Value& root)
{
	for(int i = 0; i < rangeData.size(); ++i)
	{
		rangeData[i].loadJson(root[i]);
	}
}


void RangeModifierList::loadJson(const Json::Value& root)
{
	for(int i = 0; i < modifiers.size(); ++i)
	{
		modifiers[i].loadJson(root[i]);
	}
}

