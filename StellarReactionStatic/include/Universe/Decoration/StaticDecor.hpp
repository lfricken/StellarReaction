#ifndef STATICDECOR_HPP
#define STATICDECOR_HPP

#include "DecorQuad.hpp"

class GraphicsComponent;
struct DecorQuadData;

class StaticDecor : public DecorQuad
{
public:
	StaticDecor(const StaticDecor& rData);
	virtual ~StaticDecor();

	virtual bool inputHook(std::string rCommand, sf::Packet rData);
protected:
private:
};



struct StaticDecorData : public DecorQuadData
{
	StaticDecorData() :
		DecorQuadData(),
		parallax(1),
		position()
	{

	}

	float parallax;
	b2Vec2 position;

	virtual void loadJson(const Json::Value& root);

	virtual DecorQuad* generate() const
	{
		return new StaticDecor(*this);
	}
	virtual DecorQuadData* clone() const
	{
		return new StaticDecorData(*this);
	}
};

#endif // DECORQUAD_HPP
