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
	for(int i = 0; i < RangeList::LAST_VAL; ++i)
	{
		rangeData[i].loadJson(root[RangeList::getFieldName(i)]);
	}
}


void RangeModifierList::loadJson(const Json::Value& root)
{
	for(int i = 0; i < RangeList::LAST_VAL; ++i)
	{
		modifiers[i].loadJson(root[RangeList::getFieldName(i)]);
	}
}

