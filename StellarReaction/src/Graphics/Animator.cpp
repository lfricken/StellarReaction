#include "Animator.hpp"

#include "Globals.hpp"
#include "AnimAlloc.hpp"

#include "Animation.hpp"

using namespace std;


const std::string Animator::m_errorTiles = "Error";
Animator::Animator(const std::string& setAnimationSheet)
{
	m_pSet = game.getAnimAlloc().request(setAnimationSheet);
	m_startTime = m_timer.getTime();
	m_realTime = game.getTime();



	m_name = "Default";
	m_currentDuration = 1.f;
	setAnimation(m_name, 1.f);
}
Animator::~Animator()
{

}
void Animator::setAnimation(const std::string& setAnim, float duration)
{
	m_currentDuration = duration;
	m_name = setAnim;
	m_startTime = m_timer.getTime();

	auto it = m_pSet->animations.find(m_name);
	if(it == m_pSet->animations.end())
	{
		m_name = m_errorTiles;
		auto it = m_pSet->animations.find(m_name);
		if(it == m_pSet->animations.end())
		{
			cout << "\n" << FILELINE;
			///ERROR LOG
		}
	}
}
sf::Vector2i Animator::getTile()//what texture tile should we be on? 0,0, 1,0 ect
{
	float newTime = game.getTime();

	Universe& r = game.getUniverse();

	float timer = m_timer.getTime();
	float elapsed = timer - m_startTime;//elapsed is the time since start of this animation
	float percent = elapsed / m_currentDuration;//tells us how far through we are

	const Animation& rAnim = m_pSet->animations.find(m_name)->second;
	const std::vector<std::pair<sf::Vector2i, float> >& rTiles = rAnim.tileSet;

	if(percent >= 1.f && rAnim.repeats)
	{
		setAnimation(m_name, m_currentDuration);
		percent = 0.f;
	}

	int i = 0;
	for(; i < (signed)rTiles.size() && percent >= 0; ++i)//go through and find what frame we should be on
		percent -= rTiles[i].second;

	return rTiles[i - 1].first;///PROBLEM HERE
}
const sf::Vector2f& Animator::getTileSize() const
{
	return m_pSet->tileSize;
}
