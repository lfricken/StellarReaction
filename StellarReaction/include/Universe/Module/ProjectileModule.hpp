#ifndef PROJECTILEMODULE_HPP
#define PROJECTILEMODULE_HPP

#include "Sensor.hpp"
#include "QuadComponent.hpp"

struct ProjectileModuleData;

class ProjectileModule : public Sensor
{
public:
	ProjectileModule(const ProjectileModuleData& rData);
	virtual ~ProjectileModule();

	void postPhysUpdate();
	void arm(const Message& rPayload);//we will now collide and send our damage Message packet
	void disarm();//we wont collide with anyone

protected:
private:
	QuadComponent m_decor;
};


struct ProjectileModuleData : public SensorData
{
	ProjectileModuleData() :
		SensorData(),
		baseDecor()
	{
		fixComp.isSensor = true;
		fixComp.shape = Shape::Circle;
		fixComp.colCategory = Category::Projectile;
		fixComp.colMask = Mask::Projectile;
		baseDecor.animSheetName = "projectile/ballistic1.acfg";
		baseDecor.texName = "projectile/ballistic1.png";
		baseDecor.layer = GraphicsLayer::Projectiles;
	}

	QuadComponentData baseDecor;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
	{
		ProjectileModuleData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		return new ProjectileModule(copy);
	}
	virtual ModuleData* clone() const
	{
		return new ProjectileModuleData(*this);
	}
};

#endif // PROJECTILEMODULE_HPP
