#pragma once

#include "stdafx.hpp"
#include "Blueprintable.hpp"


class Hardpoint : public ILoadsJson
{
public:
	sf::Vector2i position;
	bool allowsWeapons;

	virtual void loadJson(const Json::Value& root);
};

/// <summary>
/// Stats about
/// </summary>
class ShipProfile : public ILoadsJson
{
public:
	ShipProfile();
	~ShipProfile();

	List<Hardpoint> hardpoints;

	virtual void loadJson(const Json::Value& root);
};

