#pragma once

#include <vector>

/// <summary>
/// Automatically accesses content directory.
/// </summary>
class Directory
{
public:
	Directory(const std::string& rContentDir);
	~Directory();
	
	/// <summary>
	/// Gets all files in a relative directory.
	/// </summary>
	/// <param name="rDir">The starting directory.</param>
	/// <param name="evalFunc">Function for extensions.</param>
	/// <returns>List of all files in directory whos extensions satisfy the evalFunc</returns>
	std::vector<std::pair<std::string, std::string> > getAllFiles(const std::string& rDir, bool(*evalFunc)(const std::string& extensionName) = NULL) const;
	/// <summary>
	/// Gets all files in a relative directory.
	/// </summary>
	/// <param name="rDir">The starting directory.</param>
	/// <param name="evalFunc">A particular extension to accept.</param>
	/// <returns>List of all files in directory with given extension.</returns>
	std::vector<std::pair<std::string, std::string> > getAllFiles(const std::string& rDir, const std::string& extension) const;
	/// <summary>
	/// Gives the relative path to the content folder.
	/// </summary>
	/// <returns>Content folder relative path.</returns>
	const std::string& getContentDir() const;	

private:
	std::string m_contentDir;
};


