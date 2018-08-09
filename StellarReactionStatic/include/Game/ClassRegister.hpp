#pragma once
#include "stdafx.hpp"
#include "Debugging.hpp"


//template<int Index, class Search, class First, class... Types>
//struct get_internal
//{
//	typedef typename get_internal<Index + 1, Search, Types...>::type type;
//	static constexpr int index = Index;
//};
//
//template<int Index, class Search, class... Types>
//struct get_internal<Index, Search, Search, Types...>
//{
//	typedef get_internal type;
//	static constexpr int index = Index;
//};
//
//template<class T, class... Types>
//T get_custom(std::tuple<Types...> tuple)
//{
//	return std::get<get_internal<0, T, Types...>::type::index>(tuple);
//}

template<class... TypeX>
class ClassRegister
{
public:

	template<class B>
	const B* deduce(const String& name)
	{
		std::map<String, sptr<const B> >& map = std::get<std::map<String, sptr<const B> > >(mappings);
		return map[name].get();
	}

	template<class B>
	void store(const String& name, const B* instance)
	{
		std::map<String, sptr<const B> >& map = std::get<std::map<String, sptr<const B> > >(mappings);
		if(instance != nullptr)
		{
			map[name] = sptr<const B>(instance);
		}
		else
			WARNING;
	}

	std::tuple<std::map<String, sptr<const TypeX> >... > mappings;

private:
};

