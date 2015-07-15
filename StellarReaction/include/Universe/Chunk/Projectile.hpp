#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "BodyComponent.hpp"
#include "ProjectileModule.hpp"

struct ProjectileData
{
	ProjectileData() :
		body(),
		module()
	{
		body.angularDampening = 0;
		body.coords = b2Vec2(25,25);
		body.isBullet = true;
		body.isDynamic = true;
		body.linearDampening = 0;
		body.rotation = 0;
		body.startAwake = true;
	}

	BodyComponentData body;
	ProjectileModuleData module;
};

class Projectile
{
public:
	Projectile(const ProjectileData& rData);
	virtual ~Projectile();

	void launch(const b2Vec2& rStart, const b2Vec2& rVel, float radCCW, float radCCWps, const Message& rPayload);// coord, linVel, rot, rotVel
	void reset();//this projectile will go back into projectile pool

protected:
	BodyComponent m_body;
	ProjectileModule m_module;
private:

};


#endif // PROJECTILE_HPP
