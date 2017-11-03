#pragma once

#include "RangeList.hpp"
#include "BodyComponent.hpp"
#include "Blueprintable.hpp"

struct ModuleParentData;
struct ModuleData;
class Module;
class Chunk;
class ShipModule;

class ModuleParent : public Blueprintable
{
public:
	ModuleParent(const ModuleParentData& data);
	virtual ~ModuleParent();


	/// <summary>
	/// Range of stats.
	/// </summary>
	RangeList& getRanges();
	/// <summary>
	/// Physics body.
	/// </summary>
	BodyComponent& getBodyComponent();


	/// <summary>
	/// Add a module to this.
	/// </summary>
	void add(const ModuleData& rData);
	/// <summary>
	/// Enables or disables stealth on this. Uses stealth pool for effectiveness.
	/// </summary>
	void toggleStealth(bool on);
	/// <summary>
	/// Returns the list of modules on in chunk.
	/// </summary>
	List<sptr<Module>> getModuleList() const;
	/// <summary>
	/// Return a list of module names and positions.
	/// </summary>
	List<Pair<String, sf::Vector2i> > getModuleBPs() const;
	/// <summary>
	/// Return a list of stored module names and positions.
	/// </summary>
	const List<Pair<String, sf::Vector2i> >& getStoredModuleBPs() const;
	/// <summary>
	/// Remove all modules and stored modules from this chunk.
	/// </summary>
	void clear();



	/// <summary>
	/// Returns the title of the module at that position, otherwise returns empty String.
	/// </summary>
	String hasModuleAt(const sf::Vector2i offset) const;
	/// <summary>
	/// Returns nearest module target using taxicab geometry
	/// randomly chooses between equal distances.
	/// </summary>
	ShipModule* getNearestValidTarget(Vec2 target);
	/// <summary>
	/// Returns the radius of this chunk.
	/// </summary>
	float getRadius();




	/// <summary>
	/// Return this dynamic casted to a Chunk*.
	/// </summary>
	Chunk* thisAsChunk();
protected:
	RangeList m_ranges;//List of range values.

	BodyComponent m_body;//Our BodyComponent.

	bool allows(const Vec2& gridPos);
	List<sptr<Module> > m_modules;//List of modules on us.
	List<Pair<String, sf::Vector2i> > m_storedModules; //Stored as Blueprint,position.
	List<Vec2> m_validOffsets;//valid module locations

private:
	float m_radius;//calculated radius of this chunk.
	bool m_recalcRadius;//mark true if the radius of the module should be recalculated
};

struct ModuleParentData : public BlueprintableData
{
	ModuleParentData() : 
		bodyComp()
	{
		rangeData.rangeData[RangeList::Zoom].Min = 1;
		rangeData.rangeData[RangeList::Zoom].Value = 1;
		rangeData.rangeData[RangeList::Zoom].Max = 128;

		for(float i = -5; i < 5; i += 0.5)
			for(float j = -5; j < 5; j += 0.5)
				validPos.push_back(Vec2(i, j));
	}

	RangeListData rangeData;
	BodyComponentData bodyComp;
	List<sptr<const ModuleData> > moduleData;
	List<Vec2> validPos;

	virtual ModuleParentData* clone() const
	{
		return new ModuleParentData(*this);
	}

	virtual void loadJson(const Json::Value& root);
protected:
};



