#pragma once

#include <vector>

/// \brief Automatically accesses content directory.
class Directory
{
public:
	Directory(const std::string& r
		);
	~Directory();
	

	/// \brief Gets all files in a relative directory.
	///
	/// If passed an evaluator function, the function will be used to determine whether a particular extension is accepted.
	/// Otherwise it will accept any extension, including none.
	std::vector<std::pair<std::string, std::string> > getAllFiles(const std::string& rDir, bool(*evalFunc)(const std::string& extensionName) = NULL) const;
	/// Gets all files in a relative directory whos extension matches the given.
	std::vector<std::pair<std::string, std::string> > getAllFiles(const std::string& rDir, const std::string& extension) const;
	/// Gives the relative path to the content folder.
	const std::string& getContentDir() const;	

private:
	/// The relative path to the content folder.
	std::string m_contentDir;
};


