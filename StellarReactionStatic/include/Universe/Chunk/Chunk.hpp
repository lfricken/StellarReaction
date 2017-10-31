#pragma once

#include "Chunk.hpp"
#include "BodyComponent.hpp"
#include "RangeList.hpp"
#include "ClassRegister.hpp"
#include "QuadComponent.hpp"
#include "BlueprintableData.hpp"
#include "NonCopyable.hpp"
#include "Team.hpp"
#include "Resources.hpp"
#include "ModuleParent.hpp"
#include "Universe.hpp"

struct ChunkData;
class Module;
struct ModuleData;
class ShipModule;
struct CommandInfo;
enum class Directive;
enum class Request;
namespace leon
{
	class Grid;
}

/// \brief A players ship.
class Chunk : public ModuleParent
{
public:

	virtual ~Chunk();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	////Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();


	///Set coordinates for current aim.
	void setAim(const Vec2& world);
	///Send a command to this target.
	void directive(const CommandInfo& commands);
	///Get a requested value.
	float get(Request value) const;
	///Returns the graphical component for the hull.
	sptr<GraphicsComponent> getHull() const;
	///Increments the number of deaths by this chunk.
	int incDeaths();
	///Get the spawn location in the world.
	Vec2 getSpawn();
	///Set the spawn location in the world.
	void setSpawn(float x, float y);
	///Returns safe location to spawn chunk near spawn point.
	Vec2 getClearSpawn();
	///Set stealth to on or off.
	void setStealth(bool stealthToggle);
	///Get stealth toggle.
	bool isStealth();

	void resetStatusBoard(wptr<leon::Grid> grid);
	wptr<leon::Grid> getStatusBoard();
	IOComponent m_io;

	bool m_inDeathProcess;//true if we have already sent a message to destroy ourselves
	bool m_canDie;
	int m_controller;
	int m_shipAI;
	sptr<Resources> m_resources;
	int universePosition;
protected:
	/// <summary>
	/// Causes explosion effects and calls removeFromUniverse.
	/// </summary>
	void explode();
	/// <summary>
	/// Deletes us.
	/// </summary>
	void removeFromUniverse(int targetChunkUniversePos, bool shake, float delay);

	wptr<leon::Grid> m_statusBoard;
	virtual void input(String rCommand, sf::Packet rData);
	Chunk(const ChunkData& rData);
	friend struct ChunkData;
private:

	Universe& m_universe;

	Timer m_timer;
	int m_slavePosition;


	Vec2 m_spawnPoint;

	sptr<GraphicsComponent> hull;
	bool m_wasThrusting;
	List<sptr<GraphicsComponent> > afterburners;
	bool m_wasBoosting;
	List<sptr<GraphicsComponent> > afterburners_boost;

	/// <summary>
	/// Controls how frequently you can toggle the shields.
	/// </summary>
	Timer m_shieldToggleTimer;
	/// <summary>
	/// Minimum energy required to sustain shields.
	/// </summary>
	float m_minShieldPower;
	bool m_areShieldsOn;
	bool m_stealth;

	int m_thrustNoiseIndex;
	int m_boostNoiseIndex;
	int m_deaths;
	float m_radius;
	Vec2 m_lastAim;
};

/// Initialize a Chunk.
struct ChunkData : public ModuleParentData
{
	ChunkData() :
		minShieldPower(0.25),
		ioComp(&game.getUniverse().getUniverseIO()),
		universeParent(nullptr)
	{
		title = "CHUNK_DEFAULT_TITLE";
	}

	Universe* universeParent;

	float minShieldPower;
	IOComponentData ioComp;

	QuadComponentData hullSpriteData;
	List<QuadComponentData> afterburnerSpriteData;
	List<QuadComponentData> afterburnerThrustSpriteData;

	///Create Chunk object from this data object.
	virtual Chunk* generate(Universe* universeParent) const
	{
		ChunkData copy(*this);
		copy.universeParent = universeParent;
		copy.ioComp.pMyManager = &universeParent->getUniverseIO();
		return new Chunk(copy);
	}
	///Create new copy of this data object.
	virtual ChunkData* clone() const
	{
		return new ChunkData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
private:
	void loadModules(const Json::Value& root);
	MyType(ChunkData, ChunkData);
};

/// <summary>
/// Used to create ALL chunks in the game.
/// </summary>
struct ChunkDataMessage
{
	ChunkDataMessage()
	{
		blueprintName = "default";
		coordinates = Vec2(0, 0);
		rotation = 0;
		team = -1;
		slaveName = "NOSLAVE";
		needsController = false;
		aiControlled = false;
	}
	String blueprintName;
	Vec2 coordinates;
	/// <summary>
	/// Rotation in rads CCW
	/// </summary>
	float rotation;
	int team;
	/// <summary>
	/// Leave this blank if you want an auto assignment.
	/// </summary>
	String slaveName;
	bool needsController;
	bool aiControlled;

	void pack(sf::Packet* data) const;
	void unpack(const sf::Packet& data);

	void loadJson(const Json::Value& root);
};
