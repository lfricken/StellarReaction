#pragma once

#include "stdafx.hpp"

/// <summary>
/// Provides a way of adding an item, then destroying it and freeing the location for future use
/// </summary>
template <class T>
class Factory
{
public:
	Factory()
	{

	}
	~Factory()
	{

	}

	/// <summary>
	/// returns handle to object
	/// </summary>
	int insert(sptr<T> object)
	{
		int position = -1;
		if(m_freePositions.size() > 0)
		{
			position = m_freePositions.back();
			m_freePositions.pop_back();
			m_list[position] = object;
		}
		else
		{
			position = m_list.size();
			m_list.push_back(object);
		}

		Print << "\nFactory Into" << position;
		return position;
	}
	/// <summary>
	/// Returns object at position, or null if not exists
	/// </summary>
	sptr<T> get(int position)
	{
		if(position < static_cast<signed int>(m_list.size()))
			return m_list[position];
		else
			return sptr<T>(nullptr);
	}
	/// <summary>
	/// removes the element from the position and potentially reuses it
	/// </summary>
	bool free(int position)
	{
		Print << "\nFactory Free " << position;
		if(position < m_list.size())
			if(m_list[position].get() != nullptr)
			{
				m_list[position].reset();
				m_freePositions.push_back(position);
			}

		return false;
	}
	/// <summary>
	/// Destroy all objects and reset this to defaults
	/// </summary>
	void clear()
	{
		m_list.clear();
		m_freePositions.clear();
	}

	typename List<sptr<T> >::iterator begin()
	{
		return m_list.begin();
	}
	typename List<sptr<T> >::iterator end()
	{
		return m_list.end();
	}

	int size() const
	{
		return static_cast<signed>(m_list.size());
	}

	typename sptr<T> operator [](int i)
	{
		return m_list[i];
	}
protected:
	List<sptr<T> > m_list;
	List<int> m_freePositions;

};

