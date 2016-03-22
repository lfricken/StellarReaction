#include "Directory.hpp"

#include <filesystem>
#include <iostream>

bool defaultAllExtensions(const std::string& rName)
{
	return true;
}

Directory::Directory(const std::string& rContentDir)
{
	m_contentDir = rContentDir;
}
Directory::~Directory()
{

}
std::vector<std::pair<std::string, std::string> > Directory::getAllFiles(const std::string& rDir, bool(*evalFunc)(const std::string& name)) const
{
	if(evalFunc == NULL)
		evalFunc = &defaultAllExtensions;

	std::vector<std::pair<std::string, std::string> > list;
	for(std::tr2::sys::recursive_directory_iterator it(m_contentDir + rDir), end; it != end; ++it)
		if(!is_directory(it->path()))
			if((*evalFunc)(it->path().extension()))
			{
				std::string fileName = it->path().basename();
				std::string fullPath = it->path().directory_string();
				list.push_back(std::pair<std::string, std::string>(fileName, fullPath));
			}


	return list;
}
std::vector<std::pair<std::string, std::string> > Directory::getAllFiles(const std::string& rDir, const std::string& extension) const
{
	std::vector<std::pair<std::string, std::string> > list;
	for(std::tr2::sys::recursive_directory_iterator it(m_contentDir + rDir), end; it != end; ++it)
		if(!is_directory(it->path()))
			if(it->path().extension() == extension)
			{
				std::string fileName = it->path().basename();
				std::string fullPath = it->path().directory_string();
				list.push_back(std::pair<std::string, std::string>(fileName, fullPath));
			}

	return list;
}
const std::string& Directory::getContentDir() const
{
	return m_contentDir;
}
