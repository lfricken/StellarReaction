#include "Directory.hpp"

#include <filesystem>

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
std::vector<std::string> Directory::getAllFiles(const std::string& rDir, bool(*evalFunc)(const std::string& name)) const
{
	if(evalFunc == NULL)
		evalFunc = &defaultAllExtensions;

	std::vector<std::string> list;
	for(std::tr2::sys::recursive_directory_iterator i(m_contentDir + rDir), end; i != end; ++i)
		if(!is_directory(i->path()))
			if((*evalFunc)(i->path().extension()))
				list.push_back(i->path().directory_string());


	return list;
}
std::vector<std::string> Directory::getAllFiles(const std::string& rDir, const std::string& extension) const
{
	std::vector<std::string> list;
	for(std::tr2::sys::recursive_directory_iterator i(m_contentDir + rDir), end; i != end; ++i)
		if(!is_directory(i->path()))
			if(i->path().extension() == extension)
				list.push_back(i->path().directory_string());

	return list;
}
const std::string& Directory::getContentDir() const
{
	return m_contentDir;
}
