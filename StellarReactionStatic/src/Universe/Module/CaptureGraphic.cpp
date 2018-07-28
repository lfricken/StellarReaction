#include "CaptureGraphic.hpp"



void CaptureGraphicData::loadJson(const Json::Value& root)
{
	LOADJSON(baseDecor);
	LOADJSON(whiteLights);
	LOADJSON(blueLights);
	LOADJSON(greenLights);
	LOADJSON(yellowLights);
	LOADJSON(pinkLights);
	LOADJSON(redLights);

	ModuleData::loadJson(root);
}

CaptureGraphic::CaptureGraphic(const CaptureGraphicData& rData) : Module(rData)
{
	m_light1 = 0;
	m_light2 = 0;
	m_onlyRed = false;
	m_cap = nullptr;
	m_base_decor = sptr<GraphicsComponent>(new Spinner(rData.baseDecor));
	m_lights.push_back(sptr<GraphicsComponent>(new Spinner(rData.whiteLights)));
	m_lights.push_back(sptr<GraphicsComponent>(new Spinner(rData.blueLights)));
	m_lights.push_back(sptr<GraphicsComponent>(new Spinner(rData.greenLights)));
	m_lights.push_back(sptr<GraphicsComponent>(new Spinner(rData.yellowLights)));
	m_lights.push_back(sptr<GraphicsComponent>(new Spinner(rData.pinkLights)));
	m_lights.push_back(sptr<GraphicsComponent>(new Spinner(rData.redLights)));

	Vec2 parentPos = m_parent->getBodyComponent().getPosition();
	m_base_decor->setPosition(parentPos);
	for (auto light = m_lights.begin(); light != m_lights.end(); ++light)
	{
		(*light)->setPosition(parentPos);
		(*light)->setAlpha(0);
	}
	m_lights[m_light1]->setAlpha(255);
}
CaptureGraphic::~CaptureGraphic()
{

}
void CaptureGraphic::prePhysUpdate()
{
	if(m_cap == nullptr)
	{
		m_cap = dynamic_cast<CaptureArea*>((&*m_parent->getModuleList()[0]));
		m_parent->getBodyComponent().setTeam(Team::Capturable);
	}
}
void CaptureGraphic::postPhysUpdate()
{
	if(m_cap == nullptr)
		return;

	Team cur_team = m_cap->getCurrentTeam();
	float prog = m_cap->getProgress();

	//check if the red lights should be turned on or off
	redLights(m_cap->isConflicted());
	if (!m_cap->isConflicted() && cur_team != Team::Invalid){
		//adjust light alpha based on capture progress
		//set the current light
		m_light2 = (int)cur_team;
		//m_light1 is the default light
		int newAlpha = (int)(255 * (1.f - prog));
		if (newAlpha < 0)
			newAlpha = 0;
		m_lights[m_light1]->setAlpha(newAlpha);
		//m_light2 is the current owner of the base
		newAlpha = (int)(255 * prog);
		if (newAlpha < 0)
			newAlpha = 0;
		m_lights[m_light2]->setAlpha(newAlpha);
	}
}

void CaptureGraphic::redLights(bool conflict)
{
	//turn off red if it was previously on and area is not in conflict
	if (m_onlyRed && !conflict)
	{
		m_lights[5]->setAlpha(0);
		m_onlyRed = false;
	}
	//turn on red if it was previously off and area is in conflict
	if (!m_onlyRed && conflict)
	{
		m_lights[m_light1]->setAlpha(0);
		m_lights[m_light2]->setAlpha(0);
		m_lights[5]->setAlpha(255);
		m_onlyRed = true;
	}
}
