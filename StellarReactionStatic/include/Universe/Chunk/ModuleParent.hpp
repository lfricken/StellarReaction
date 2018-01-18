#pragma once

#include "RangeList.hpp"
#include "BodyComponent.hpp"
#include "Blueprintable.hpp"
#include "ShipProfile.hpp"

struct ModuleParentData;
struct ModuleData;
class Module;
class Chunk;
class ShipModule;
namespace leon
{
	class Grid;
}

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
	void add(ModuleData& data);
	/// <summary>
	/// Add a module to this.
	/// </summary>
	void add(const String bpName, const Vec2& pos);
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


	void resetStatusBoard(wptr<leon::Grid> grid);
	wptr<leon::Grid> getStatusBoard();


	/// <summary>
	/// Return this dynamic casted to a Chunk*.
	/// </summary>
	Chunk* thisAsChunk();
protected:
	RangeList m_ranges;//List of range values.
	BodyComponent m_body;//Our BodyComponent.

	/// <summary>
	/// Checks hardpoints and their requirements.
	/// </summary>
	bool allows(const ModuleData& data);
	List<sptr<Module> > m_modules;//List of modules on us.
	List<Pair<String, sf::Vector2i> > m_storedModules; //Stored as Blueprint,position.

private:
	float m_radius;//calculated radius of this chunk.
	bool m_recalcRadius;//mark true if the radius of the module should be recalculated
	wptr<leon::Grid> m_statusBoard; // if someone is targeting us, this is the board we should update
	ShipProfile m_profile;
};

struct ModuleParentData : public BlueprintableData
{
	ModuleParentData() : 
		bodyComp()
	{
		rangeData.rangeData[RangeList::Zoom].Min = 1;
		rangeData.rangeData[RangeList::Zoom].Value = 1;
		rangeData.rangeData[RangeList::Zoom].Max = 128;
	}

	ShipProfile profile;
	RangeListData rangeData;
	BodyComponentData bodyComp;
	List<Pair<String, Vec2> > moduleData;

	virtual ModuleParentData* clone() const
	{
		return new ModuleParentData(*this);
	}

	virtual void loadJson(const Json::Value& root);
protected:
};



