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
		texName = "beam/beam_mid.png";
		animSheetName = "beam/beam_mid.acfg";
		layer = GraphicsLayer::Projectiles;

		start.texName = "beam/beam_start.png";
		start.animSheetName = "beam/beam_start.acfg";
		start.layer = GraphicsLayer::Effects;

		end.texName = "beam/beam_end.png";
		end.animSheetName = "beam/beam_end.acfg";
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
	void setStart(const b2Vec2& rStart);
	/// Sets the end point of the beam.
	void setEnd(const b2Vec2& rEnd);
	/// Activate the beam for a given amount of time.
	void activate(float secs, int beamWidth, const sf::Color& color);//seconds to be active, and then deactivate
	/// Regardless of what we are doing, turn off so we can't be seen.
	void deactivate();
	/// Used to determine when to turn ourselves off.
	virtual void postUpdate();

protected:
private:
	Timer m_deactivateTimer;

	b2Vec2 m_startPos;
	QuadComponent m_start;

	b2Vec2 m_endPos;
	QuadComponent m_end;
};

#endif // BEAM_HPP
