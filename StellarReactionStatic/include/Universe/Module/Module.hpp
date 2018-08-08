#pragma once

#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkComponent.hpp"
#include "FixtureComponent.hpp"
#include "Universe.hpp"
#include "RangeList.hpp"
#include "ClassRegister.hpp"
#include "JSON.hpp"
#include "CommandInfo.hpp"
#include "NonCopyable.hpp"
#include "Debugging.hpp"
#include "Blueprintable.hpp"
#include "ModuleParent.hpp"

struct ModuleData;

/// <summary>
/// Basic building block of Chunks and physical objects.
/// </summary>
class Module : public Blueprintable, NonCopyable
{
public:
	Module(const ModuleData& rData);
	virtual ~Module();
	///Pure Virtual function that processes actions on object before physics updates.
	virtual void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Send a command to a target.
	virtual void directive(const CommandInfo& commands);
	///Set coordinates for current aim.
	virtual void setAim(const Vec2& rTarget);
	/// <summary>
	/// Get center of this module with respect to center of body.
	/// </summary>
	Vec2 getOffset() const;
	///Get name of module.
	const String& getName() const;
	///Set stealth to on or off.
	virtual void toggleStealth(bool toggle);
	///Heal to full health.
	virtual void healToMax();
	///Get a reference to the fixture component of this module.
	const FixtureComponent& getFixtureComponent();

	virtual float functionalCapacity();

	IOComponent m_io;
protected:
	virtual void input(String rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);
	virtual void startContactCB(FixtureComponent* pOther);
	virtual void endContactCB(FixtureComponent* pOther);

	void applyModifierRates();

	Range& energyRange();
	Range& ballisticRange();
	Range& missilesRange();

	Range& zoomRange();
	Range& teleportRange();
	Range& stealthRange();


	FixtureComponent m_fix;

	void applyModifiers(bool apply);
	RangeModifierList rangeModifiers;
	RangeList* ranges;
	Timer m_timer;//timer used to update resource generation

	Timer m_stealthTimer; //timer used to turn stealth mode off;

	String m_name;//what gets displayed to player
	ModuleParent* m_parent;
	int m_collisionDamage;
private:
};

/// <summary>
/// Blueprint for Module
/// </summary>
struct ModuleData : public BlueprintableData
{
	ModuleData() :
		rangeModifiers(),
		name("defaultName"),
		collisionDamage(0),
		ioComp(&getGame()->getUniverse().getUniverseIO()),
		nwComp(),
		fixComp()
	{
		title = "MODULE_DEFAULT_TITLE";
	}

	ModuleParent* parent;
	HardpointRequirements requirements;

	RangeModifierList rangeModifiers;

	String name;//what gets displayed to player
	int collisionDamage;//how much damage we apply to someone who collides with us

	IOComponentData ioComp;
	NetworkComponentData nwComp;
	FixtureComponentData fixComp;

	struct GenerateParams
	{
		ModuleParent* parent;
	};
	/// <summary>
	/// Create Module object from this data object.
	/// </summary>
	virtual Module* generate(GenerateParams params) const
	{
		WARNING;
		return nullptr;
	}
	/// <summary>
	/// Create new copy of this data object.
	/// </summary>
	virtual ModuleData* clone() const
	{
		return new ModuleData(*this);
	}
	/// <summary>
	/// Fill this object with data from a json file.
	/// </summary>
	virtual void loadJson(const Json::Value& root);

protected:
	template<class Type, class TypeData>
	/// <summary>
	/// Generalized code for the generate function.
	/// Called in the add function of ModuleParent.
	/// </summary>
	Type* generateSub(GenerateParams params, const TypeData* const me) const
	{
		TypeData copy(*me);//data copy
		copy.parent = params.parent;
		copy.rangeModifiers.ranges = &params.parent->getRanges();
		copy.fixComp.pBody = params.parent->getBodyComponent().getBodyPtr();
		return new Type(copy);
	}
	/// <summary>
	/// Overwrites data in this object from another ModuleData object.
	/// </summary>
	virtual void inherit(const ModuleData& parent)
	{
		*this = parent;
	}
};
