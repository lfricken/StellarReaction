#include "GraphicsComponent.hpp"

#include "Convert.hpp"
#include "Universe.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "JSON.hpp"

using namespace sf;

void GraphicsComponentData::loadJson(const Json::Value& root)
{
	GETJSON(dimensions);
	GETJSON(permanentRot);
	GETJSON(center);
	GETJSON(texName);
	if(!root["layer"].isNull())
		layer = ChooseLayer(root["layer"].asString());
	GETJSON(color);
}
void GraphicsComponentData::setCenterTopLeft()
{
	center = sf::Vector2f(-dimensions.x / 2, dimensions.y / 2);
}
GraphicsComponent::GraphicsComponent(const GraphicsComponentData& rData) : m_rUpdater(getGame()->getUniverse().getGfxUpdater()), m_animator(rData.texName)
{
	m_calculatedSize = false;

	m_color = rData.color;
	m_scale = 1;
	m_rUpdater.store(this);
	m_rotation = 0;
	m_offset = sf::Vector2f(0, 0);

	m_dimensions = rData.dimensions;

	m_permanentRot = Convert::degToRad(rData.permanentRot);
	m_center = rData.center;
}
GraphicsComponent::~GraphicsComponent()
{
	m_rUpdater.freeThis(this);

	for(int i = 0; i < m_numVerts; ++i)
		(*m_pVerts)[i + m_startVert].color = sf::Color(0, 0, 0, 0);//make them transparent so they can no longer be seen
}
void GraphicsComponent::setScale(float scale)
{
	m_scale = scale;
}
float GraphicsComponent::getScale() const
{
	return m_scale;
}
void GraphicsComponent::setPosition(const Vec2& rUniverseCoords)
{
	coordinates = rUniverseCoords;
}
void GraphicsComponent::setGuiPosition(const sf::Vector2f& rScreenCoordinates)
{
	coordinates = Convert::screenToUniverse((Vec2)Convert::flipYAxis(rScreenCoordinates));
}
void GraphicsComponent::setRotation(float radCCW)
{
	m_rotation = Convert::normRad(radCCW);
}
void GraphicsComponent::setOffset(const sf::Vector2f pixels)//sets the origin of us
{
	m_offset = pixels;
}
void GraphicsComponent::setColor(sf::Color color)
{
	m_color = color;
	for(int i = 0; i < m_numVerts; ++i)
		(*m_pVerts)[i + m_startVert].color = color;//make them transparent so they can no longer be seen
}
void GraphicsComponent::setAlpha(int alphaValue)
{
	sf::Color curColor = m_color;
	setColor(sf::Color(curColor.r, curColor.g, curColor.b, alphaValue));
}
Animator& GraphicsComponent::getAnimator()
{
	return m_animator;
}
const Vec2& GraphicsComponent::getPosition() const
{
	return coordinates;
}
const sf::Vector2f GraphicsComponent::getGuiPosition() const
{
	return Convert::flipYAxis((sf::Vector2f)Convert::universeToScreen(coordinates));
}
float GraphicsComponent::getRotation() const
{
	return Convert::normRad(m_rotation);
}
const sf::Vector2f& GraphicsComponent::getOffset() const
{
	return m_offset;
}
const sf::Color GraphicsComponent::getColor() const
{
	return m_color;
}
void GraphicsComponent::update()
{
	sf::Transform form = getTransform();
	sf::Vector2f offsetFixed(m_offset.x, -m_offset.y);
	sf::Vector2f centerFixed(m_center.x, -m_center.y);

	for(int i = 0; i < m_numVerts; ++i)
		(*m_pVerts)[i + m_startVert].position = form.transformPoint(m_originPos[i] * m_scale + offsetFixed - centerFixed);

	sf::Vector2i tile = m_animator.getTile();
	sf::Vector2f size = m_animator.getTileSize();

	for(int i = 0; i < m_numVerts; ++i)
		(*m_pVerts)[i + m_startVert].texCoords = sf::Vector2f(size.x*m_originTex[i].x + tile.x*size.x, size.y*m_originTex[i].y + tile.y*size.y);

	postUpdate();
}
sf::Transform GraphicsComponent::getTransform() const
{
	sf::Transform transform;
	transform.translate((sf::Vector2f)Convert::flipYAxis(Convert::universeToScreen(coordinates))).rotate(Convert::radToDeg(-m_rotation - m_permanentRot));
	return transform;
}
Vec2 GraphicsComponent::getUnscaledSize() const
{
	if(!m_calculatedSize)
	{
		Vec2 maxSize(-1, -1);
		for(int i = 0; i < m_numVerts; ++i)
			if(m_originPos[i].x > maxSize.x)
				maxSize.x = m_originPos[i].x;
		for(int i = 0; i < m_numVerts; ++i)
			if(m_originPos[i].y > maxSize.y)
				maxSize.y = m_originPos[i].y;

		Vec2 minSize(-1, -1);
		for(int i = 0; i < m_numVerts; ++i)
			if(m_originPos[i].x < minSize.x)
				minSize.x = m_originPos[i].x;
		for(int i = 0; i < m_numVerts; ++i)
			if(m_originPos[i].y < minSize.y)

				minSize.y = m_originPos[i].y;

		m_size = maxSize - minSize;
		m_size = Convert::screenToUniverse(m_size);

		m_calculatedSize = true;
	}
	return m_size;
}
Vec2 GraphicsComponent::getSize() const
{
	return m_size * m_scale;
}
