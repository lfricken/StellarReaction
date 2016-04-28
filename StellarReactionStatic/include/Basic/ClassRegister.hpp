#pragma once

///Create a register for this class with this name.
#define MyType(BASE, TYPE) \
	static Registerer<BASE, TYPE> reg

///Register the class.
#define RegisterClass(BASE, TYPE) \
	Registerer<BASE, TYPE> TYPE::reg(#TYPE)

template<class B>
///Deduce which derived class a string refers to, given a base type.
class Deduce
{
public:
	///Deduce which derived class a string refers to, given a base type.
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
///Register a derived class.
class Registerer
{
public:
	///Register a class with this name.
	Registerer(const std::string& name)
	{
		Deduce<B>::from(name, new D);//actually add
	}
};
