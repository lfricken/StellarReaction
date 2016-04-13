#include "DecorationEngine.hpp"

#include "QuadComponent.hpp"
#include "Decoration.hpp"
#include "Camera.hpp"
#include "Convert.hpp"

DecorationEngine::DecorationEngine() : m_deltaTime(0)
{

}
DecorationEngine::~DecorationEngine()
{

}
void DecorationEngine::update(const Camera& camera)
{
	Vec2 cameraPos = camera.getPosition();

	sf::Vector2f center = camera.getView().getCenter();
	sf::Vector2f halfSize(camera.getView().getSize().x / 2, camera.getView().getSize().y / 2);

	halfSize = leon::screenToCart(halfSize);//for easier subtraction and addition below
	Vec2 bottomLeft = leon::sfTob2(center - halfSize);
	Vec2 topRight = leon::sfTob2(center + halfSize);

	float dTime = m_deltaTime.getTimeElapsed();;
	for(auto it = m_static.begin(); it != m_static.end(); ++it)
		(**it).updateScaledPosition(cameraPos, bottomLeft, topRight, dTime);
}
void DecorationEngine::loadJson(const Json::Value& root)
{
	m_deltaTime.getTimeElapsed();

	Decoration* pDecor = NULL;
	QuadComponent* pQuad = NULL;
	QuadComponentData quadData;
	DecorationData decorData;

	quadData.layer = GraphicsLayer::Effects;
	decorData.movementScale = 0.9f;


	decorData.realPosition = Vec2(0, 0);

	pQuad = new QuadComponent(quadData);
	pDecor = new Decoration(decorData, pQuad);
	m_static.push_back(sptr<Decoration>(pDecor));


	//decorData.realPosition = Vec2(1, 0);

	//pQuad = new QuadComponent(quadData);
	//pDecor = new Decoration(decorData, pQuad);
	//m_static.push_back(sptr<Decoration>(pDecor));
}


