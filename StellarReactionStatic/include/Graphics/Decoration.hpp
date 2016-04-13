#ifndef DECORATION_HPP
#define DECORATION_HPP

#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

class GraphicsComponent;
struct DecorationData;

/// <summary>
/// The generic decoration type. Needs to implement a specific type of graphic for GraphicsComponent
/// </summary>
class Decoration final
{
public:
	Decoration(const DecorationData& rData, GraphicsComponent* pGfx);
	virtual ~Decoration();

	void setPosition(const Vec2& rWorld);
	void setRotation(float radiansCCW);
	void setAnimation(const std::string& rAnimName, float duration);
	void setScale(float scale);

	void updateScaledPosition(const Vec2& rCameraCenter, const Vec2& bottomLeft, const Vec2& topRight, float dTime);

	void input(std::string rCommand, sf::Packet rData);
protected:

private:
	bool hasLeftScreen(const sf::Rect<float>& rect) const;

	sptr<GraphicsComponent> m_spGfx;
	IOComponent m_io;

	float m_movementScale;
	
	Vec2 m_velocity;
	Vec2 m_realPosition;//absolute (where it actually would be in a 3d world)
};



struct DecorationData
{
	DecorationData() :
		ioComp(&game.getUniverse().getUniverseIO()),
		realPosition(0, 0),
		minVelocity(0, 0),
		maxVelocity(0, 0),
		tiled(false),
		movementScale(0)
	{

	}

	IOComponentData ioComp;
	float movementScale;
	//Examples imagine you are looking out of a speeding car at another speeding car next to you
	//an object super far      1 (it follows the camera 1:1 and always stays in view) (stars in night sky)
	//an object kind of far  0.5 (it follows the camera, but moves slowly) (grain silo a mile away)
	//an object same plane     0 (it doesnt follow the camera) (the ground under the other car)
	//an object closer      -0.5 (it flies past the camera) (the ground between you and the other car)


	Vec2 realPosition;
	Vec2 minVelocity;
	Vec2 maxVelocity;
	bool tiled;

	virtual void loadJson(const Json::Value& root);
};


#endif // DECORATION_HPP
