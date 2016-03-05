#pragma once

#define MyType(BASE, TYPE) \
	static Registerer<BASE, TYPE> reg

#define RegisterClass(BASE, TYPE) \
	Registerer<BASE, TYPE> TYPE::reg(#TYPE)

template<class B>
class Deduce
{
public:
	static B* from(const std::string& name, B* instance = NULL)
	{
		static std::map<std::string, B*> mapping;
		if(instance != NULL)
		{
			mapping[name] = instance;
			return NULL;
		}
		else//should check if name is in map first
			return mapping[name];
	}
};

template<class B, class D>
class Registerer
{
public:
	Registerer(const std::string& name)
	{
		Deduce<B>::from(name, new D);//actually add
	}
};
