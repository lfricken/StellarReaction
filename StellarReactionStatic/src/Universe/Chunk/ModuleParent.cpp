#include "ModuleParent.hpp"
#include "Module.hpp"
#include "JSON.hpp"
#include "BlueprintLoader.hpp"
#include "ShipModule.hpp"
#include "OddballFunctions.hpp"
#include "Chunk.hpp"
#include "Grid.hpp"
#include "Turret.hpp"

void ModuleParentData::loadJson(const Json::Value& root)
{
	BlueprintableData::loadJson(root);

	GETJSON(isPlayer);
	LOADJSON(profile);
	LOADJSON(rangeData);
	LOADJSON(bodyComp);

	if(!root[NAMEOF(moduleData)].isNull())
	{
		sptr<ModuleData> spMod;
		for(auto it = root[NAMEOF(moduleData)].begin(); it != root[NAMEOF(moduleData)].end(); ++it)
		{
			Pair<String, Vec2> data;
			if(!(*it)["title"].isNull())
			{
				data.first = (*it)["title"].asString();

				Vec2 position;
				position.x = (*it)["Position"][0].asFloat();
				position.y = (*it)["Position"][1].asFloat();
				data.second = position;
			}
			else
			{
				WARNING;
			}

			moduleData.push_back(data);
		}
	}
}
ModuleParent::ModuleParent(const ModuleParentData& data) : Blueprintable(data), m_body(data.bodyComp), m_ranges(data.rangeData)
{
	m_isPlayer = data.isPlayer;
	m_profile = data.profile;

	m_radius = 0.f;
	m_recalcRadius = true;

	// add modules
	for(auto it = data.moduleData.cbegin(); it != data.moduleData.cend(); ++it)
	{
		add(it->first, it->second);
	}
}
ModuleParent::~ModuleParent()
{

}
bool ModuleParent::isPlayer()
{
	return m_isPlayer;
}
float ModuleParent::maxWeaponRange()
{
	float range = 0.f;
	for each (sptr<Module> module in m_modules)
	{
		Turret* turret = dynamic_cast<Turret*>(module.get());
		if(turret != nullptr)
		{
			String title = turret->getWeapon()->getTitle();
			const float thisWeaponRange = getGame()->getUniverse().getBlueprints().getWeaponSPtr(title)->range;
			if(thisWeaponRange > range)
				range = thisWeaponRange;
		}
	}
	return range;
}
Chunk* ModuleParent::thisAsChunk()
{
	return dynamic_cast<Chunk*>(this);
}
bool ModuleParent::allows(const ModuleData& data)
{
	if(m_profile.hardpoints.size() == 0)//if they didn't define anything, allow anything.
		return true;

	for(auto it = m_profile.hardpoints.begin(); it != m_profile.hardpoints.end(); ++it)
	{
		if(it->position == data.fixComp.offset)//if we are talking about the same position
		{
			if(it->providesRequirements(data.requirements))//if the hardpoint requirements are met
				return true;

			break;//otherwise exit
		}
	}

	return false;
}
void ModuleParent::add(String bpName, const Vec2& pos)
{
	if(!isdigit(bpName.back())) // necessary for when our ship is created from the base blueprint
	{
		int team = (int)m_body.getTeam();
		if(team > 0)
		{
			bpName += String(team);
		}
	}

	auto dataPtr = sptr<ModuleData>(getGame()->getUniverse().getBlueprints().getModuleSPtr(bpName)->clone());
	auto& data = *dataPtr;

	data.fixComp.offset = pos;
	add(data);
}
void ModuleParent::add(ModuleData& data)
{
	m_recalcRadius = true;

	if(data.fixComp.offset.x == 3) // defines the storage TODO: (storage should be defined better)
	{
		m_storedModules.push_back(Pair<String, sf::Vector2i>(data.title, (sf::Vector2i)data.fixComp.offset));
	}
	else if(this->allows(data))
	{
		sptr<ModuleData> moduleDataCopy(data.clone());
		moduleDataCopy->ioComp.pMyManager = &getGame()->getUniverse().getUniverseIO();
		ModuleData::GenerateParams params;
		params.parent = this;
		auto module = sptr<Module>(moduleDataCopy->generate(params));
		m_modules.push_back(module);
	}
	else
		WARNING;
}
RangeList& ModuleParent::getRanges()
{
	return m_ranges;
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
			String title = (*it)->getTitle();
			list.push_back(std::pair<String, sf::Vector2i>(title, sf::Vector2i((int)pos.x, (int)pos.y)));
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
void ModuleParent::resetStatusBoard(wptr<leon::Grid> grid)
{
	m_statusBoard = grid;
	if(auto board = m_statusBoard.lock())
	{
		board->reset(getModuleBPs());
		auto modules = getModuleList();
		for(auto it = modules.begin(); it != modules.end(); ++it)
		{
			auto module = dynamic_cast<ShipModule*>(it->get());
			if(module != nullptr)
			{
				board->damageModule(module->getOffset(), module->getHealthState(), module->getHealth().getHealthPercent(), false);
			}
		}
	}//else we set it to null
}
wptr<leon::Grid> ModuleParent::getStatusBoard()
{
	return m_statusBoard;
}
