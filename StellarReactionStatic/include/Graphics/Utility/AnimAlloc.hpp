#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

struct AnimSet;

/// \brief Stores animations so we don't have to load them from json repeatedly for no reason.
/// 
/// Similar to TextureAllocator.
class AnimAlloc : NonCopyable
{
public:
	AnimAlloc();
	virtual ~AnimAlloc();

	/// \brief Returns a pointer to the AnimSet loaded from the file given. The path is relative to the texture folder.
	const AnimSet* request(const String& rAnimFile);

protected:
private:
	/// Map of full animation name and path to the AnimSet.
	std::map<String, sptr<AnimSet> > m_animationSets;
};

