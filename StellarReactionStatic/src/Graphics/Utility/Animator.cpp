#include "Animator.hpp"
#include "Globals.hpp"
#include "AnimAlloc.hpp"
#include "Debugging.hpp"
#include "Animation.hpp"


const String Animator::m_errorTiles = "Error";
Animator::Animator(const String& setAnimationSheet)
{
	m_pSet = getGame()->getAnimAlloc().request(setAnimationSheet);
	m_startTime = m_timer.getTime();

	m_currentDuration = 1.f;
	setAnimation("Default", 1.f);
}
Animator::~Animator()
{

}
void Animator::setAnimation(const String& animationName, float duration)
{
	m_currentTileIndex = 0;
	m_currentDuration = duration;
	m_startTime = m_timer.getTime();

	auto it = m_pSet->animations.find(animationName);

	if(it == m_pSet->animations.end())
	{
		auto it = m_pSet->animations.find(m_errorTiles);// Failed to find our animation.

		if(it == m_pSet->animations.end())
			WARNING;// Failed to find error animation!
		else
			m_pAnim = &(it->second);
	}
	else
		m_pAnim = &(it->second);// Found our animation.
}
sf::Vector2i Animator::getTile()//what texture tile should we be on? 0,0, 1,0 ect
{
	float elapsed = m_timer.getTime() - m_startTime;
	float percent = elapsed / m_currentDuration;
	const List<std::pair<sf::Vector2i, float> >& tiles = m_pAnim->tileSet;

	if(percent >= 1.f)// We are done with this animation.
	{ 
		if(!m_pAnim->repeats)
			return tiles.back().first;// Return last tile.
		else
		{
			m_startTime = m_timer.getTime();// Restart animation.
			percent = 0.f;
			m_currentTileIndex = 0;
		}
	}

	while(tiles[m_currentTileIndex].second < percent)
		++m_currentTileIndex;

	return tiles[m_currentTileIndex].first;///PROBLEM HERE
}
const sf::Vector2f& Animator::getTileSize() const
{
	return m_pSet->tileSize;
}
