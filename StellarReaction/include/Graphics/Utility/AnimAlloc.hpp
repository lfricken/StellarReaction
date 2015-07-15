#ifndef ANIMALLOC_HPP
#define ANIMALLOC_HPP

#include "stdafx.hpp"

struct AnimSet;

class AnimAlloc
{
public:
	AnimAlloc();
	virtual ~AnimAlloc();

	const AnimSet* request(const std::string& rAnimFile);//return pointer to it and the size of the tiles

protected:
private:
	std::map<std::string, sptr<AnimSet> > m_animationSets;
};

#endif // ANIMALLOC_HPP
