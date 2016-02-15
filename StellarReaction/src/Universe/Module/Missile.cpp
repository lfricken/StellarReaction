#include "Missile.hpp"

using namespace std;

Missile::Missile(const MissileData& rData) : Projectile(rData)
{

}
Missile::~Missile()
{

}
void Missile::missileLaunch()
{
	cout << "missile launched";
}