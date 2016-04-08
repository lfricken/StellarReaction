#ifndef DYNAMICDECOR_HPP
#define DYNAMICDECOR_HPP

#include "DecorQuad.hpp"

class GraphicsComponent;
struct DynamicDecorData;

class DynamicDecor : public DecorQuad
{
public:
	DynamicDecor(const DynamicDecorData& rData);
	virtual ~DynamicDecor();

	virtual bool inputHook(std::string rCommand, sf::Packet rData);
protected:
private:
	float m_parallax;
	float m_frequency;
	float m_minX;
	float m_minY;
	float m_maxX;
	float m_maxY;
};



struct DynamicDecorData : public DecorQuadData
{
	DynamicDecorData() :
		DecorQuadData(),
		parallax(1),
		frequency(1),
		minX(1),
		minY(1),
		maxX(10),
		maxY(10)
	{

	}

	float parallax;
	float frequency;
	float minX;
	float minY;
	float maxX;
	float maxY;

	virtual void loadJson(const Json::Value& root);

	virtual DecorQuad* generate() const
	{
	return new DynamicDecor(*this);
	}
	virtual DynamicDecorData* clone() const
	{
		return new DynamicDecorData(*this);
	}
	MyType(DynamicDecorData, DynamicDecorData);
};

#endif // DYNAMICDECOR_HPP
