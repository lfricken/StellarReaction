#ifndef BEAM_HPP
#define BEAM_HPP

#include "QuadComponent.hpp"

/// Data needed for a beam.
struct BeamData : public QuadComponentData
{
	BeamData() :
		QuadComponentData(),
		start(),
		end()
	{
		texName = "beam/beam_mid";
		layer = GraphicsLayer::Projectiles;

		start.texName = "beam/beam_start";
		start.layer = GraphicsLayer::Effects;

		end.texName = "beam/beam_end";
		end.layer = GraphicsLayer::Effects;
	}

	QuadComponentData start;
	QuadComponentData end;
};


/// \brief Beam is a special type of GraphicsComponent made for easier drawing of lasers.
class Beam : protected QuadComponent
{
public:
	Beam(const BeamData& rData);
	virtual ~Beam();

	/// Sets the start point of the beam.
	void setStart(const Vec2& rStart);
	/// Sets the end point of the beam.
	void setEnd(const Vec2& rEnd);
	/// Activate the beam for a given amount of time.
	void activate(float secs, int beamWidth, const sf::Color& color);//seconds to be active, and then deactivate
	/// Regardless of what we are doing, turn off so we can't be seen.
	void deactivate();
	/// Used to determine when to turn ourselves off.
	virtual void postUpdate();

protected:
private:
	Timer m_deactivateTimer;

	Vec2 m_startPos;
	QuadComponent m_startSprite;

	Vec2 m_endPos;
	QuadComponent m_endSprite;
};

#endif // BEAM_HPP
