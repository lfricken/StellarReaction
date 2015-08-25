#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "stdafx.hpp"
#include "Timer.hpp"

class AnimAlloc;
struct AnimSet;

class Animator
{
public:
	Animator(const std::string& setAnimationSheet);
	virtual ~Animator();

	void setAnimation(const std::string& setAnim, float duration);//name of animation, how long should that last? (lower numbers mean the sheet completes faster)
	sf::Vector2i getTile();//what tile should we be on? 0,0, 1,0 ect
	const sf::Vector2f& getTileSize() const;

	//Evan - TODO - do something with this
	const AnimSet* m_pSet;

protected:
private:
	mutable Timer m_timer;


	float m_realTime;

	std::string m_name;//the current animation state we are in
	float m_currentDuration;//how long the animation that we are executing should last
	float m_startTime;//when we started the most recent animation

	sf::Vector2f m_tileSize;

	static const std::string m_errorTiles;
};

#endif // ANIMATOR_HPP
