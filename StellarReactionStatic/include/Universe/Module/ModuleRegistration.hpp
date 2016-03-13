#pragma once
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
#include "Teleport.hpp"
#include "GravityField.hpp"
#include "TriggerSensor.hpp"

#include "Capacitor.hpp"
#include "Module.hpp"



RegisterClass(ModuleData, TurretData);
RegisterClass(ModuleData, CapacitorData);

RegisterClass(ModuleData, ShipModuleData);
RegisterClass(ModuleData, PlatingData);

RegisterClass(ModuleData, RadarData);
RegisterClass(ModuleData, ReactorData);

RegisterClass(ModuleData, ModuleData);
RegisterClass(ModuleData, ProjectileModuleData);

RegisterClass(ModuleData, ThrusterData);
RegisterClass(ModuleData, StealthData);
RegisterClass(ModuleData, TeleportData);

RegisterClass(ModuleData, TriggerSensorData);
RegisterClass(ModuleData, GravityFieldData);
RegisterClass(ModuleData, CaptureAreaData);
RegisterClass(ModuleData, SensorData);






