#ifndef DECORQUAD_HPP
#define DECORQUAD_HPP

#include "Decoration.hpp"
#include "QuadComponent.hpp"

class GraphicsComponent;
struct DecorQuadData;

/// <summary>
/// Child decoration type. Implements a specific type of graphic for GraphicsComponent (a quad component)
/// </summary>
class DecorQuad : public Decoration
{
public:
	DecorQuad(const DecorQuadData& rData);
	virtual ~DecorQuad();

	virtual bool inputHook(std::string rCommand, sf::Packet rData);
protected:

	QuadComponent m_quad;
private:
};



struct DecorQuadData : public DecorationData
{
	std::string title;
	DecorQuadData() :
		DecorationData()
	{

	}

	QuadComponentData quadComp;

	virtual Decoration* generate() const
	{
		return new DecorQuad(*this);
	}
	virtual DecorationData* clone() const
	{
		return new DecorQuadData(*this);
	}
};

#endif // DECORQUAD_HPP
