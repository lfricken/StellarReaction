#include "DebugDraw.hpp"
#include "globals.hpp"
#include "stdafx.hpp"
#include "Convert.hpp"



DebugDraw::DebugDraw()
{
	sf::Color outline(255,0,60,255);
	sf::Color fillC(0,0,255,64);
	float thickness = -2.f;

	m_convex.setFillColor(fillC);
	m_convex.setOutlineColor(outline);
	m_convex.setOutlineThickness(thickness);
	m_convex.setPointCount(4);

	m_convex2.setFillColor(fillC);
	m_convex2.setOutlineColor(outline);
	m_convex2.setOutlineThickness(thickness);

	m_circle.setFillColor(fillC);
	m_circle.setOutlineColor(outline);
	m_circle.setOutlineThickness(thickness);

	m_line.append(sf::Vertex(sf::Vector2f(0,0), outline));
	m_line.append(sf::Vertex(sf::Vector2f(0,0), outline));
	m_line[0].texCoords = sf::Vector2f(0,0);
	m_line[1].texCoords = sf::Vector2f(0,0);
	m_line.setPrimitiveType(sf::Lines);
}

DebugDraw::~DebugDraw()
{

}
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	(void)color;
	m_convex2.setPointCount(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		m_convex2.setPoint(i, leon::b2Tosf<float>(vertices[i]));
	}
	game.getWindow().draw(m_convex2);
}
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	(void)color;

	for (int i = 0; i < 4; i++)
	{
		m_convex.setPoint(i, leon::b2Tosf<float>(vertices[i]));
	}
	game.getWindow().draw(m_convex);
}
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	(void)color;
	m_circle.setRadius(radius*scale);
	m_circle.setPosition(leon::b2Tosf<float>(center));
	game.getWindow().draw(m_circle);
}
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{

	(void)color;
	(void)axis;
	m_circle.setOrigin(radius*scale, radius*scale);
	m_circle.setRadius(radius*scale);
	m_circle.setPosition(leon::b2Tosf<float>(center));
	game.getWindow().draw(m_circle);
}
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	(void)color;
	m_line[0].position = leon::b2Tosf<float>(p1);
	m_line[1].position = leon::b2Tosf<float>(p2);

	game.getWindow().draw(m_line);
}
void DebugDraw::DrawTransform(const b2Transform& xf)
{
	(void)xf;

	m_line[0].position = leon::b2Tosf<float>(Vec2(0,0));
	m_line[1].position = leon::b2Tosf<float>(xf.p);

	game.getWindow().draw(m_line);
}
void SetWindow(const sf::RenderWindow& rWindow)
{
	(void)rWindow;//shutup the compiler about unused
}
