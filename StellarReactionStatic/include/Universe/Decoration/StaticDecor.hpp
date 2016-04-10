#ifndef STATICDECOR_HPP
#define STATICDECOR_HPP

#include "DecorQuad.hpp"
#include "ClassRegister.hpp"

class GraphicsComponent;
struct StaticDecorData;

class StaticDecor : public DecorQuad
{
public:
	StaticDecor(const StaticDecorData& rData);
	virtual ~StaticDecor();

	virtual bool inputHook(std::string rCommand, sf::Packet rData);
protected:
private:
};



struct StaticDecorData : public DecorQuadData
{
	StaticDecorData() :
		DecorQuadData()
	{

	}

	void loadJson(const Json::Value& root);

	virtual DecorQuad* generate() const
	{
		return new StaticDecor(*this);
	}
	virtual StaticDecorData* clone() const
	{
		return new StaticDecorData(*this);
	}
	MyType(StaticDecorData, StaticDecorData);
};

#endif // STATICDECOR_HPP
