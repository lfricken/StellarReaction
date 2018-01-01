#pragma once

#include "stdafx.hpp"
#include "Blueprintable.hpp"

class HardpointRequirements : public ILoadsJson
{
public:
	HardpointRequirements();

	bool weapon;
	bool engine;
	bool special;

	virtual void loadJson(const Json::Value& root);
};

class Hardpoint : public HardpointRequirements
{
public:
	Vec2 position;

	/// <summary>
	/// Does this hard point provide the specialties?
	/// </summary>
	bool providesRequirements(const HardpointRequirements& other) const;

	virtual void loadJson(const Json::Value& root);
};

/// <summary>
/// Stats about
/// </summary>
class ShipProfile : public ILoadsJson
{
public:
	List<Hardpoint> hardpoints;

	virtual void loadJson(const Json::Value& root);
};

