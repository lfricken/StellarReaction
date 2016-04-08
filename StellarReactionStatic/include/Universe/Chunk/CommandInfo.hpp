#pragma once

#include "stdafx.hpp"
#include "Directive.hpp"


struct CommandInfo
{
	Map<Directive, bool> directives;//things to do
	Map<int, bool> weaponGroups;//which weapon groups are active
	bool isLocal;//is this command coming from a controller that is controlled by the local player?
};

