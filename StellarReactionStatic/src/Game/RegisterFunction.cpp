#pragma once
#include "Game.hpp"
#include "ClassRegisterType.hpp"


//Modules
#include "Sensor.hpp"
#include "CaptureArea.hpp"

#include "Turret.hpp"
#include "ShipModule.hpp"

#include "Plating.hpp"
#include "Radar.hpp"

#include "Reactor.hpp"
#include "ProjectileModule.hpp"

#include "Thruster.hpp"
#include "Stealth.hpp"
#include "GravityField.hpp"
#include "TriggerSensor.hpp"
#include "BlackHoleGraphic.hpp"
#include "CaptureGraphic.hpp"

#include "Mind.hpp"
#include "Extractor.hpp"
#include "Replicator.hpp"
#include "Relay.hpp"

#include "Capacitor.hpp"
#include "Module.hpp"

#include "Shield.hpp"
#include "DecorationEngine.hpp"
#include "Loot.hpp"
//End Modules

//Particles
#include "Particles.hpp"
//End Particles

//Weapons
#include "MissileWeapon.hpp"
#include "LaserWeapon.hpp"
#include "BallisticWeapon.hpp"
#include "GrappleWeapon.hpp"
#include "Weapon.hpp"
//End Weapons

//Chunks
#include "Chunk.hpp"
//End Chunks

//Projectiles
#include "Projectile.hpp"
#include "Missile.hpp"
//End Projectiles

#define REGISTER(base, type) \
instance->store<base>(#type, new type())

void* Game::getClassRegister()
{
	return classRegister.get();
}
void Game::registerClasses()
{
	auto instance = new ClassRegisterType;
	classRegister.reset(instance);

	//Chunks
	REGISTER(ChunkData, ChunkData);
	//End Chunks

	//Weapons
	REGISTER(WeaponData, ProjectileWeaponBaseData);
	REGISTER(WeaponData, BallisticWeaponData);
	REGISTER(WeaponData, MissileWeaponBlueprint);
	REGISTER(WeaponData, LaserWeaponData);
	REGISTER(WeaponData, GrappleWeaponData);
	//End Weapons

	//Projectile
	REGISTER(ProjectileData, ProjectileData);
	REGISTER(ProjectileData, MissileData);
	//End Projectile

	//Modules
	REGISTER(ModuleData, TurretData);
	REGISTER(ModuleData, CapacitorData);

	REGISTER(ModuleData, ShipModuleData);
	REGISTER(ModuleData, PlatingData);

	REGISTER(ModuleData, RadarData);
	REGISTER(ModuleData, ReactorData);

	REGISTER(ModuleData, ModuleData);
	REGISTER(ModuleData, ProjectileModuleData);

	REGISTER(ModuleData, ThrusterData);
	REGISTER(ModuleData, StealthData);

	REGISTER(ModuleData, CaptureGraphicData);
	REGISTER(ModuleData, BlackHoleGraphicData);
	REGISTER(ModuleData, TriggerSensorData);
	REGISTER(ModuleData, GravityFieldData);
	REGISTER(ModuleData, CaptureAreaData);
	REGISTER(ModuleData, SensorData);

	REGISTER(ModuleData, MindData);
	REGISTER(ModuleData, ExtractorData);
	REGISTER(ModuleData, ReplicatorData);
	REGISTER(ModuleData, RelayData);

	REGISTER(ModuleData, ShieldData);
	REGISTER(ModuleData, ShieldComponentData);

	REGISTER(ModuleData, LootData);
	//End Modules

	//Particles
	REGISTER(Particles, Particles);
	//End Particles
}
