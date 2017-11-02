#include "Teleporter.hpp"
#include "Controller.hpp"
#include "Player.hpp"
#include "JSON.hpp"
#include "RangeList.hpp"



void TeleporterData::loadJson(const Json::Value& root)
{
	GETJSON(energyConsumptionPerDist);
	GETJSON(cooldown);
}
Teleporter::Teleporter(const TeleporterData& data)
{
	energyConsumptionPerDist = data.energyConsumptionPerDist;
	cooldown = data.cooldown;
}
Teleporter::~Teleporter()
{

}
void Teleporter::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;

	if(rIssues[Directive::Teleport])
	{
		//get mouse position and use that to decide how far we are teleporting
		b2Body* bod = parent->getBodyComponent().getBodyPtr();

		const Vec2 mousePos = game.getLocalPlayer().getMouseInWorld();
		const Vec2 origPos = bod->GetPosition();

		Vec2 direction = mousePos - origPos;
		float actualDist = 0;

		const float attemptDistance = direction.len();
		const float maxDist = parent->getRanges()[RangeList::TeleportRange].getValue();

		Vec2 target;
		if(attemptDistance <= maxDist)
		{
			target = mousePos;
			actualDist = attemptDistance;
		}
		else
		{
			direction = direction.unit();
			direction *= maxDist;

			target = direction + origPos;
			actualDist = maxDist;
		}

		const float consumption = actualDist * energyConsumptionPerDist;

		if(m_cooldownTimer.isTimeUp())
		{
			if(parent->getRanges()[RangeList::Energy].tryChange(-consumption))
			{
				//check if the destination is clear
				if(game.getUniverse().isClear(target, parent->getRadius(), parent))
				{
					//restart timer, perfrom teleport
					m_cooldownTimer.restartCountDown();
					bod->SetTransform(target, bod->GetAngle());
				}
			}
		}
	}
}

