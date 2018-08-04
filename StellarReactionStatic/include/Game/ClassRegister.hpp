#pragma once
#include "stdafx.hpp"
#include "Debugging.hpp"

template<class... TypeX>
class ClassRegister
{
public:

	template<class B>
	const B* deduce(const String& name)
	{
		std::map<String, sptr<B> > map = mappings.get<std::map<String, sptr<const B> > >();
		return map[name].get();
	}

	template<class B>
	void store(const String& name, const B* instance)
	{
		std::map<String, sptr<B> > map = mappings.get<std::map<String, sptr<const B> > >();
		if(instance != nullptr)
		{
			map[name] = sptr<const B>(instance);
		}
		else
			WARNING;
	}

	std::tuple<std::map<String, sptr<const TypeX> >... > mappings;
};

