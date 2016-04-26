#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"

class AnimAlloc;
struct AnimSet;
struct Animation;

/// <summary>
/// Controls animation of Graphics Components
/// </summary>
class Animator
{
public:
	Animator(const std::string& setAnimationSheet);
	virtual ~Animator();

	void setAnimation(const std::string& setAnim, float duration);//name of animation, how long should that last? (lower numbers mean the sheet completes faster)
	sf::Vector2i getTile();//what tile should we be on? 0,0, 1,0 ect
	const sf::Vector2f& getTileSize() const;

	const AnimSet* m_pSet;

protected:
private:
	/// To determine how much time has gone by.
	mutable Timer m_timer;
	/// Our current Animation.
	const Animation* m_pAnim;

	/// Current tile index in our Animation.
	int m_currentTileIndex;
	/// How long the animation that we are doing should last.
	float m_currentDuration;
	/// When we started the most recent animation.
	float m_startTime;

	/// Animation to default to.
	static const std::string m_errorTiles;
};
