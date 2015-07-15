#ifndef DECORQUAD_HPP
#define DECORQUAD_HPP

#include "Decoration.hpp"
#include "QuadComponent.hpp"

class GraphicsComponent;
struct DecorQuadData;

class DecorQuad : public Decoration
{
public:
	DecorQuad(const DecorQuadData& rData);
	virtual ~DecorQuad();


	virtual void input(std::string rCommand, sf::Packet rData);
protected:


	QuadComponent m_quad;
private:
};



struct DecorQuadData : public DecorationData
{
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
