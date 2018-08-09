#include "Directory.hpp"

#include <filesystem>
#include <iostream>

bool defaultAllExtensions(const String& rName)
{
	return true;
}

Directory::Directory(const String& rContentDir)
{
	m_contentDir = rContentDir;
}
Directory::~Directory()
{

}
List<std::pair<String, String> > Directory::getAllFiles(const String& rDir, bool(*evalFunc)(const String& name)) const
{
	if(evalFunc == nullptr)
		evalFunc = &defaultAllExtensions;

	List<std::pair<String, String> > list;
	for(std::tr2::sys::recursive_directory_iterator it(m_contentDir + rDir), end; it != end; ++it)
		if(!is_directory(it->path()))
			if((*evalFunc)(it->path().extension().string()))
			{
				const String baseFileName = it->path().stem().string();
				const String fullPath = it->path().relative_path().string();
				list.push_back(std::pair<String, String>(baseFileName, fullPath));
			}


	return list;
}
List<std::pair<String, String> > Directory::getAllFiles(const String& rDir, const String& extension) const
{
	List<std::pair<String, String> > list;
	for(std::tr2::sys::recursive_directory_iterator it(m_contentDir + rDir), end; it != end; ++it)
		if(!is_directory(it->path()))
			if(it->path().extension().string() == extension)
			{
				const String baseFileName = it->path().stem().string();
				const String fullPath = it->path().relative_path().string();
				list.push_back(std::pair<String, String>(baseFileName, fullPath));
			}

	return list;
}
const String& Directory::getContentDir() const
{
	return m_contentDir;
}
