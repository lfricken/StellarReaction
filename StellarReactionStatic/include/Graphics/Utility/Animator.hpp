#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"

class AnimAlloc;
struct AnimSet;
struct Animation;

/// Controls Animation of GraphicsComponents
class Animator
{
public:
	Animator(const String& setAnimationSheet);
	virtual ~Animator();

	/// <summary>
	/// Set name of animation and how long it should last in seconds.
	/// </summary>
	void setAnimation(const String& setAnim, float duration);
	/// Returns which tile should we be on right now.
	sf::Vector2i getTile();
	/// How large the texture tiles are.
	const sf::Vector2f& getTileSize() const;



protected:
private:
	/// The pointer to the AnimSet we use.
	const AnimSet* m_pSet;
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
	static const String m_errorTiles;
};
