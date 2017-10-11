#include "RangeList.hpp"



RangeDataModifierList::RangeDataModifierList()
{
	for(int i = 0; i < RangeList::LAST_VAL; ++i)
		modifiers.push_back(RangeDataModifier());
}



void RangeListData::loadJson(const Json::Value& root)
{
	for(int i = 0; i < rangeData.size(); ++i)
	{
		rangeData[i].loadJson(root[i]);
	}
}
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
