#pragma once

#include "Pool.hpp"

/// <summary>
/// Object that change pools on the chunk.
/// </summary>
class PoolChanger
{
public:
	template<typename T>
	/// <summary>
	/// Adds to the pools value if we haven't already.
	/// </summary>
	void tryContributeValue(Pool<T>* targetPool, T amount)
	{
		if(!m_hasContributed)
		{
			targetPool->changeValue(amount);
			m_hasContributed = true;
		}
	}
	template<typename T>
	/// <summary>
	/// Removes contribution from the pools value if we haven't already.
	/// </summary>
	void tryRemoveValue(Pool<T>* targetPool, T amount)
	{
		if(m_hasContributed)
		{
			targetPool->changeValue(-amount);
			m_hasContributed = false;
		}
	}
	template<typename T>
	/// <summary>
	/// Adds to the pools max if we haven't already.
	/// </summary>
	void tryContributeMax(Pool<T>* targetPool, T amount)
	{
		if(!m_hasContributed)
		{
			targetPool->changeMax(amount);
			m_hasContributed = true;
		}
	}
	template<typename T>
	/// <summary>
	/// Removes contribution from the pools max if we haven't already.
	/// </summary>
	void tryRemoveMax(Pool<T>* targetPool, T amount)
	{
		if(m_hasContributed)
		{
			targetPool->changeMax(-amount);
			m_hasContributed = false;
		}
	}

private:
	bool m_hasContributed;
};













