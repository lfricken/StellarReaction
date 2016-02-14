#ifndef GRAPHICSCOMPONENT_HPP
#define GRAPHICSCOMPONENT_HPP

#include "stdafx.hpp"
#include "GraphicsLayer.hpp"
#include "Animator.hpp"
class GraphicsComponentUpdater;

struct GraphicsComponentData
{
	GraphicsComponentData()
	{
		dimensions = sf::Vector2f(256,256);
		permanentRot = 0;//degrees CCW
		center = sf::Vector2f(0,0);
		texName = "default.png";
		animSheetName = "default.acfg";
		layer = GraphicsLayer::BackgroundFar;
		color = sf::Color::White;
	}

	sf::Vector2f dimensions;//full x,y in pixels
	float permanentRot;//default rotation when told to be 0, degrees CCW
	sf::Vector2f center;//this is the center of us
	std::string texName;//name of our texture
	std::string animSheetName;//name of our animation sheet
	GraphicsLayer layer;
	sf::Color color;
};
/// <summary>
/// Base type for all game related graphics
/// </summary>
class GraphicsComponent
{
public:
	GraphicsComponent(const GraphicsComponentData& rData);
	virtual ~GraphicsComponent();

	/**SETTERS**/
	void setPosition(const b2Vec2& rWorldCoords);//set our position in the world (absolute)
	virtual void setRotation(float radiansCCW);//set our rotation in radians CCW (absolute)
	void setOffset(const sf::Vector2f pixels);//offsets us in pixels (from center of us)
	void setColor(sf::Color);
	Animator& getAnimator();
	void setScale(float scale);

	/**GETTERS**/
	const b2Vec2& getPosition() const;
	float getRotation() const;
	const sf::Vector2f& getOffset() const;
	const sf::Color getColor() const;

	/**UPDATE**/
	void update();
	virtual void postUpdate() = 0;//used to update animation, and other things

protected:
	sf::Transform getTransform() const;

	/**ADJUSTMENTS**/
	b2Vec2 coordinates;//the latest position of us in world coordinates
	float m_rotation;//our rotation compared to normal RADIANS CCW
	sf::Vector2f m_offset;//pixels in cartesian coordinates that the image is pushed (in that direction)

	/**PERMANENT**/
	float m_permanentRot;// RADIANS CCW how much we are normally rotated (aka when told to be at 0)
	sf::Vector2f m_center;//true center of us (aka our offset when told to offset by 0) (calculated automatically)

	/**VERTS**/
	int m_numVerts;
	int m_startVert;
	sf::Vector2f m_dimensions;
	sf::VertexArray* m_pVerts;
	sf::Color m_color;
	std::vector<sf::Vector2f> m_originPos;//positions of the vertices relative to 0,0 and no rot
	std::vector<sf::Vector2f> m_originTex;//positinos of vertices in texture relative to 0,0
	GraphicsComponentUpdater& m_rUpdater;

	Animator m_animator;
private:
	float m_scale;
};

#endif // GRAPHICSCOMPONENT_HPP
