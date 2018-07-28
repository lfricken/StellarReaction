#pragma once

#include "stdafx.hpp"

/// \brief Automatically accesses content directory.
class Directory
{
public:
	Directory(const String& r);
	~Directory();


	/// \brief Gets all files in a relative directory.
	///
	/// If passed an evaluator function, the function will be used to determine whether a particular extension is accepted.
	/// Otherwise it will accept any extension, including none.
	List<std::pair<String, String> > getAllFiles(const String& rDir, bool(*evalFunc)(const String& extensionName) = nullptr) const;
	/// Gets all files in a relative directory whos extension matches the given.
	List<std::pair<String, String> > getAllFiles(const String& rDir, const String& extension) const;
	/// Gives the relative path to the content folder.
	const String& getContentDir() const;

private:
	/// The relative path to the content folder.
	String m_contentDir;
};


