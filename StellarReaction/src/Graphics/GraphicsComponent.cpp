#include "GraphicsComponent.hpp"

#include "convert.hpp"
#include "Universe.hpp"
#include "GraphicsComponentUpdater.hpp"

using namespace std;
using namespace sf;

GraphicsComponent::GraphicsComponent(const GraphicsComponentData& rData) : m_rUpdater(game.getUniverse().getGfxUpdater()), m_animator(rData.animSheetName)
{
	m_color = rData.color;
	m_rUpdater.store(this);
	m_rotation = 0;
	m_offset = sf::Vector2f(0,0);

	m_dimensions = rData.dimensions;

	m_permanentRot = leon::degToRad(rData.permanentRot);
	m_center = rData.center;
}
GraphicsComponent::~GraphicsComponent()
{

	m_rUpdater.free(this);

	for(int i=0; i<m_numVerts; ++i)
		(*m_pVerts)[i+m_startVert].color = sf::Color(0,0,0,0);//make them transparent so they can no longer be seen
}


void GraphicsComponent::setPosition(const b2Vec2& rWorldCoords)
{
	coordinates = rWorldCoords;
}
void GraphicsComponent::setRotation(float radiansCCW)
{
	m_rotation = radiansCCW;
}
void GraphicsComponent::setOffset(const sf::Vector2f pixels)//sets the origin of us
{
	m_offset = pixels;
}
void GraphicsComponent::setColor(sf::Color color)
{
	for(int i=0; i<m_numVerts; ++i)
		(*m_pVerts)[i+m_startVert].color = color;//make them transparent so they can no longer be seen
}


Animator& GraphicsComponent::getAnimator()
{
	return m_animator;
}


const b2Vec2& GraphicsComponent::getPosition() const
{
	return coordinates;
}
float GraphicsComponent::getRotation() const
{
	return m_rotation;
}
const sf::Vector2f& GraphicsComponent::getOffset() const
{
	return m_offset;
}




void GraphicsComponent::update()
{
	sf::Transform form = getTransform();
	sf::Vector2f offsetFixed(m_offset.x, -m_offset.y);
	sf::Vector2f centerFixed(m_center.x, -m_center.y);

	for(int i=0; i<m_numVerts; ++i)
		(*m_pVerts)[i+m_startVert].position = form.transformPoint(m_originPos[i]+offsetFixed-centerFixed);

	sf::Vector2i tile = m_animator.getTile();
	sf::Vector2f size = m_animator.getTileSize();

	for(int i=0; i<m_numVerts; ++i)
		(*m_pVerts)[i+m_startVert].texCoords = sf::Vector2f(size.x*m_originTex[i].x+tile.x*size.x, size.y*m_originTex[i].y+tile.y*size.y);

	postUpdate();
}
sf::Transform GraphicsComponent::getTransform() const
{
	sf::Transform transform;
	transform.translate(leon::b2Tosf<float>(coordinates)).rotate(leon::radToDeg(-m_rotation-m_permanentRot));
	return transform;
}






