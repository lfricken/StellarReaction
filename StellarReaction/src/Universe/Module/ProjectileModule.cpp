#include "ProjectileModule.hpp"

ProjectileModule::ProjectileModule(const ProjectileModuleData& rData) : Sensor(rData), m_decor(rData.baseDecor)
{

}
ProjectileModule::~ProjectileModule()
{

}
void ProjectileModule::postPhysUpdate()
{
    m_decor.setPosition(m_fix.getCenter());
    m_decor.setRotation(m_fix.getAngle());
}
void ProjectileModule::arm(const Message& rPayload)//we will now collide and send our damage Message packet
{
    //m_io.
}
void ProjectileModule::disarm()//we wont collide with anyone
{

}
