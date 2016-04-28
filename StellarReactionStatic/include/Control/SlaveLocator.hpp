#ifndef SLAVELOCATOR_HPP
#define SLAVELOCATOR_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class Chunk;

/// \brief This class holds pointers to all the chunks.
///
/// When a controller wants to control a chunk, it goes through us to find it.
class SlaveLocator : NonCopyable
{
public:
	SlaveLocator();
	virtual ~SlaveLocator();

	/// A Chunk is allocated, and we store a pointer to it in a list, and remember its name and position (return position).
	int give(Chunk* pSlave);
	/// A Chunk is freed, so we no longer can set it as a target. Remove it from our list.
	void free(int position);
	/// Return index of a Chunk, given its name. -1 if it can't be found.
	int findPos(const std::string& slaveTarget);
	/// Return a pointer to a Chunk given its position. NULL if it can't be found.
	Chunk* find(int position);
	/// Return a pointer to a Chunk given its name. NULL if it can't be found.
	Chunk* find(const std::string& rName);

protected:
private:
	/// List of Chunk pointers.
	std::vector<Chunk*> m_slavePtrs;
	/// Spots in the Chunk list where the Chunk was destroyed.
	std::vector<int> m_freeIndexes;
	/// Lookup table to find index, given a name.
	std::map<std::string, int> m_nameLookup;
};

#endif // SLAVELOCATOR_HPP
