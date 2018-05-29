#pragma once

#include "stdafx.hpp"
#include "GraphicsLayer.hpp"
#include "Animator.hpp"
#include "NonCopyable.hpp"

class GraphicsComponentUpdater;

/// Blueprint for a GraphicsComponent.
struct GraphicsComponentData
{
	GraphicsComponentData()
	{
		dimensions = sf::Vector2f(256,256);
		permanentRot = 0;//degrees CCW
		center = sf::Vector2f(0,0);
		texName = "default";
		layer = GraphicsLayer::BackgroundFar;
		color = sf::Color::White;
	}

	sf::Vector2f dimensions;//full x,y in pixels
	float permanentRot;//default rotation when told to be 0, degrees CCW
	sf::Vector2f center;//this is the center of us
	String texName;//name of our texture
	GraphicsLayer layer;
	sf::Color color;

	void setCenterTopLeft();

	/// Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};


/// \brief Base type for all game related graphics.
///
/// Does not handle most GUI graphics.
class GraphicsComponent : NonCopyable
{
public:
	GraphicsComponent(const GraphicsComponentData& rData);
	virtual ~GraphicsComponent();

	/// Set the position of this object in world coordinates.
	void setPosition(const Vec2& rUniverseCoords);
	/// <summary> Set the position of the Gui element. </summary>
	void setGuiPosition(const sf::Vector2f& rScreenCoordinates);
	/// Set our rotation in radians counterclockwise.
	void setRotation(float radCCW);
	/// Offset where we are drawn in pixels from our center.
	void setOffset(const sf::Vector2f pixels);
	/// Turns the image this color. 
	void setColor(sf::Color);
	/// Return Animator that animates us.
	Animator& getAnimator();
	/// Control the size of the object. 1 is default. 2 would double the width and height.
	void setScale(float scale);
	/// Changes our transparency.
	/// Explicitly modify only the alpha value of our color.
	void setAlpha(int alphaValue);

	/// Return our scale.
	float getScale() const;
	/// <summary>
	/// Return our dimensions in universe coordinates if we have a scale of 1.
	/// </summary>
	Vec2 getUnscaledSize() const;
	/// <summary>
	/// Get size in universe dimensions
	/// </summary>
	Vec2 getSize() const;
	/// <summary>
	/// Our position in universe coordinates.
	/// </summary>
	const Vec2& getPosition() const;
	/// Return our position in world coordinates.
	const sf::Vector2f getGuiPosition() const;
	/// Return our rotation in radians counterclockwise.
	float getRotation() const;
	/// Return our offset in pixels from our center.
	const sf::Vector2f& getOffset() const;
	/// Return our current color.
	const sf::Color getColor() const;

	/// Update the positions of our vertices in case we have moved.
	void update();
	/// Called after update so this GraphicsComponent can modify itself if needed.
	virtual void postUpdate() = 0;

protected:
	sf::Transform getTransform() const;

	/**ADJUSTMENTS**/
	Vec2 coordinates;//the latest position of us in world coordinates
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
	List<sf::Vector2f> m_originPos;//positions of the vertices relative to 0,0 and no rot
	List<sf::Vector2f> m_originTex;//positinos of vertices in texture relative to 0,0
	GraphicsComponentUpdater& m_rUpdater;

	Animator m_animator;
private:
	mutable bool m_calculatedSize;
	/// <summary>
	/// Unscaled universe size.
	/// </summary>
	mutable Vec2 m_size;
	float m_scale;
};

