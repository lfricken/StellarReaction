#include "Teleporter.hpp"
#include "Controller.hpp"
#include "Player.hpp"
#include "JSON.hpp"
#include "RangeList.hpp"
#include "Chunk.hpp"



void TeleporterData::loadJson(const Json::Value& root)
{
	GETJSON(energyConsumptionPerDist);
	GETJSON(cooldown);
}
Teleporter::Teleporter(const TeleporterData& data, ModuleParent* parent)
{
	m_parent = parent;

	m_energyConsumptionPerDist = data.energyConsumptionPerDist;
	m_cooldown = data.cooldown;

	m_cooldownTimer.setCountDown(m_cooldown);
}
Teleporter::~Teleporter()
{

}
void Teleporter::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;

	if(rIssues[Directive::Teleport])
	{
		Chunk* chunk = m_parent->thisAsChunk();

		BodyComponent& body = chunk->getBodyComponent();

		const Vec2 mousePos = chunk->getAim();
		const Vec2 origPos = body.getPosition();

		Vec2 direction = mousePos - origPos;
		float actualDist = 0;

		const float attemptDistance = direction.len();
		const float maxDist = chunk->getRanges()[RangeList::TeleportRange].getValue();

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

		const float consumption = actualDist * m_energyConsumptionPerDist;

		if(m_cooldownTimer.isTimeUp())
		{
			if(getGame()->getUniverse().isClear(target, chunk->getRadius(), chunk))//check if the destination is clear
			{
				if(chunk->getRanges()[RangeList::Energy].tryChange(-consumption))
				{
					//restart timer, perform teleport
					m_cooldownTimer.restartCountDown();
					body.setTransform(target, body.getAngle());
				}
			}
		}
	}
}

