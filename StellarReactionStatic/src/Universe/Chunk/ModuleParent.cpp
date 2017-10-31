#include "ModuleParent.hpp"
#include "Module.hpp"
#include "JSON.hpp"
#include "BlueprintLoader.hpp"
#include "ShipModule.hpp"
#include "OddballFunctions.hpp"

void ModuleParentData::loadJson(const Json::Value& root)
{
	BlueprintableData::loadJson(root);
	
	LOADJSON(rangeData);
	LOADJSON(bodyComp);

	if(!root["moduleData"].isNull())
	{
		sptr<ModuleData> spMod;
		for(auto it = root["moduleData"].begin(); it != root["moduleData"].end(); ++it)
		{
			if(!(*it)["title"].isNull())
			{
				String bpName = (*it)["title"].asString();
				auto clone = game.getUniverse().getBlueprints().getModuleSPtr(bpName)->clone();
				spMod.reset(clone);

				spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
				spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
			}
			else
			{
				WARNING;
			}

			moduleData.push_back(spMod);
		}
	}
}
ModuleParent::ModuleParent(const ModuleParentData& data) : Blueprintable(data), m_body(data.bodyComp), m_ranges(data.rangeData)
{
	m_body.parent = this;
	m_validOffsets = data.validPos;

	m_radius = 0.f;
	m_recalcRadius = true;

	//Add Modules.
	for(auto it = data.moduleData.cbegin(); it != data.moduleData.cend(); ++it)
	{
		add(**it);
	}
}
ModuleParent::~ModuleParent()
{

}
bool ModuleParent::allows(const Vec2& gridPos)
{
	return (std::find(m_validOffsets.begin(), m_validOffsets.end(), gridPos) != m_validOffsets.end());
}
void ModuleParent::add(const ModuleData& data)
{
	m_recalcRadius = true;

	if(data.fixComp.offset.x == 3)
	{
		m_storedModules.push_back(Pair<String, sf::Vector2i>(data.title, (sf::Vector2i)data.fixComp.offset));
	}
	else if(this->allows(data.fixComp.offset))
	{
		sptr<ModuleData> moduleDataCopy(data.clone());
		moduleDataCopy->ioComp.pMyManager = &game.getUniverse().getUniverseIO();
		ModuleData::GenerateParams params;
		params.parent = this;
		auto module = sptr<Module>(moduleDataCopy->generate(params));
		m_modules.push_back(module);
	}
	else
		WARNING;
}
RangeList* ModuleParent::getRanges()
{
	return &m_ranges;
}
BodyComponent& ModuleParent::getBodyComponent()
{
	return m_body;
}
List<sptr<Module>> ModuleParent::getModuleList() const
{
	return m_modules;
}
String ModuleParent::hasModuleAt(const sf::Vector2i offset) const
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		Vec2 pos = (*it)->getOffset();
		if(offset == sf::Vector2i((int)pos.x, (int)pos.y))
			return (*it)->getTitle();

	}
	return "";
}
/// <summary>
/// List of module BP names and 
/// </summary>
List<std::pair<String, sf::Vector2i> > ModuleParent::getModuleBPs() const
{
	List<std::pair<String, sf::Vector2i> > list;
	for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
	{
		if(dynamic_cast<ShipModule*>(it->get()) != nullptr)//make sure it's not a strange item, like a ShieldComponent
		{
			Vec2 pos = (*it)->getOffset();
			list.push_back(std::pair<String, sf::Vector2i>((*it)->getTitle(), sf::Vector2i((int)pos.x, (int)pos.y)));
		}
	}
	return list;
}
const List<Pair<String, sf::Vector2i> >& ModuleParent::getStoredModuleBPs() const
{
	return m_storedModules;
}
void ModuleParent::clear()
{
	m_modules.clear();
	m_storedModules.clear();
}
Chunk* ModuleParent::thisAsChunk()
{
	return dynamic_cast<Chunk*>(this);
}
float ModuleParent::getRadius()
{
	if(m_recalcRadius)
	{
		Vec2 max(0, 0);
		for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
			if(max.len() < Vec2((*it)->getOffset()).len())
				max = Vec2((*it)->getOffset());

		m_radius = max.len();
	}

	return m_radius;
}
ShipModule* ModuleParent::getNearestValidTarget(Vec2 target)
{
	Map<Vec2, ShipModule*> moduleCoordinates;
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		ShipModule* shipModule = dynamic_cast<ShipModule*>(it->get());
		if(shipModule != nullptr && shipModule->getHealth().getHealthPercent() > 0.f)
		{
			Vec2 pos = shipModule->getFixtureComponent().getOffset() - target;
			moduleCoordinates[pos] = shipModule; // -target because we need to offset damage wave origin
		}
	}


	List<ShipModule*> availableTargets;
	bool hits = false;
	for(int i = 1; hits == false && i < 4; ++i)
	{
		List<Vec2> circle = taxicabCircle(i);

		for(auto cirPos = circle.cbegin(); cirPos != circle.cend(); ++cirPos)
		{
			for(auto modulePos = moduleCoordinates.cbegin(); modulePos != moduleCoordinates.cend(); ++modulePos)
			{
				if((*cirPos) == (modulePos->first))
				{
					hits = true;
					availableTargets.push_back(modulePos->second);
				}
			}
		}
	}

	if(availableTargets.size() == 0)
		return nullptr;
	else
	{
		int choice = Rand::get(0, availableTargets.size() - 1);//it's inclusive for ints!
		return availableTargets[choice];
	}
}


