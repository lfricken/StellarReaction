#pragma once

///Create a register for this class with this name.
#define MyType(BASE, TYPE) \
	static Registerer<BASE, TYPE> reg

///Register the class.
#define RegisterClass(BASE, TYPE) \
	Registerer<BASE, TYPE> TYPE::reg(#TYPE)

template<class B>
///Deduce which derived class a String refers to, given a base type.
class Deduce
{
public:
	/// <summary>
	/// Deduce which derived class a String refers to, given a base type.
	/// Pass a pointer to a new object to assign to the mapping.
	/// </summary>
	static const B* from(const String& name, B* instance = nullptr)
	{
		static std::map<String, sptr<B> > mapping;
		if(instance != nullptr)
		{
			mapping[name] = sptr<B>(instance);
			return nullptr;
		}
		else // should check if name is in map first
			return mapping[name].get();
	}
};


template<class B, class D>
///Register a derived class.
class Registerer
{
public:
	///Register a class with this name.
	Registerer(const String& name)
	{
		Deduce<B>::from(name, new D);//actually add
	}
};
