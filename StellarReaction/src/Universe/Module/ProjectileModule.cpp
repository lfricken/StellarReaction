#include "ProjectileModule.hpp"

ProjectileModule::ProjectileModule(const ProjectileModuleData& rData) : Sensor(rData)
{
	freeSelf = false;
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.baseDecor)));
	m_decors.back()->setPosition(m_fix.getCenter());
	m_decors.back()->setRotation(m_fix.getAngle());
}
ProjectileModule::~ProjectileModule()
{

}
void ProjectileModule::arm(const Message& rPayload)//we will now collide and send our damage Message packet
{
	freeSelf = false;
    //m_io.
}
void ProjectileModule::disarm()//we wont collide with anyone
{

}
void ProjectileModule::postPhysUpdate()
{
	for(int i = 0; i<(signed)m_decors.size(); ++i)
	{
		m_decors[i]->setPosition(m_fix.getCenter());
		m_decors[i]->setRotation(m_fix.getAngle());
	}
}
void ProjectileModule::entered(FixtureComponent* pOther)
{
	//std::cout << "\nEntered";
	//freeSelf = true;
}
void ProjectileModule::exited(FixtureComponent* pOther)
{
	//std::cout << "\nExited";
}
bool ProjectileModule::shouldTerminate() const
{
	return freeSelf;
}