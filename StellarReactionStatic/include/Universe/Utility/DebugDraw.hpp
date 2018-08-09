#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "stdafx.hpp"
#include "NonCopyable.hpp"

/// Manually draws lines around Box2D objects to help with debugging.
class DebugDraw : public b2Draw, NonCopyable
{
public:
	DebugDraw();
	virtual ~DebugDraw();
	///Draw a empty polygon with a given vertices and color.
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	///Draw a solid polygon with a given vertices and color.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	///Draw a empty circle with a given vertices and color.
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	///Draw a empty circle with a given vertices and color.
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	///Draw a line segment with given points and color.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	///Draw a transformation.
	void DrawTransform(const b2Transform& xf);

	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
protected:

	sf::ConvexShape m_convex;//necessary to hold here to prevent major lag from creating and destroying
	sf::ConvexShape m_convex2;//necessary to hold here to prevent major lag from creating and destroying
	sf::CircleShape m_circle;
	sf::VertexArray m_line;
private:
};

#endif // DEBUGDRAW_H
