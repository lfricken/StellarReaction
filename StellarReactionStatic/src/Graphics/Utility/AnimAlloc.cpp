#include "AnimAlloc.hpp"
#include "Animation.hpp"
#include "Debugging.hpp"
#include "Globals.hpp"


bool hasEnding(const String& fullString, const String& ending)
{
	if(fullString.length() >= ending.length())
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	else
		return false;
}

AnimAlloc::AnimAlloc()
{

}
AnimAlloc::~AnimAlloc()
{

}
const AnimSet* AnimAlloc::request(const String& rAnimFile)
{
	auto fileName = rAnimFile;
	if(hasEnding(rAnimFile, ".acfg"))
	{
		dout << FILELINE;
	}
	else
	{
		fileName += ".acfg";
	}

	const AnimSet* pSet = nullptr;
	auto it_find = m_animationSets.find(fileName);


	if(it_find != m_animationSets.end())//if it exists
	{
		pSet = &*(it_find->second);
	}
	else//if it doesn't exist
	{
		sptr<AnimSet> spAnimSet(new AnimSet);
		Animation animation;


		const String fullPath = contentDir() + "textures/" + fileName;
		Json::Value root;
		Json::Reader reader;
		std::ifstream inputFStream(fullPath, std::ifstream::binary);
		bool parsedSuccess = reader.parse(inputFStream, root, false);

		/**== SUCCESSFUL PARSE ==**/
		if(parsedSuccess)
		{

			spAnimSet->setName = fileName;
			spAnimSet->tileSize = sf::Vector2f((float)root["texTileSize"][0].asInt(), (float)root["texTileSize"][1].asInt());
			const Json::Value stateList = root["stateList"];

			int counter = 0;
			for(auto it = stateList.begin(); it != stateList.end(); ++it)//get all the state settings
			{
				Animation animation;

				animation.name = (*it)["state"].asString();
				animation.repeats = (*it)["repeats"].asBool();
				const Json::Value tileList = (*it)["tileList"];

				float totalSum = 0;
				for(unsigned int i = 0; i < tileList.size(); ++i)
					totalSum += tileList[i]["t"].asFloat();

				float localSum = 0;
				for(unsigned int i = 0; i < tileList.size(); ++i)
				{
					localSum += tileList[i]["t"].asFloat();
					animation.tileSet.push_back(std::pair<sf::Vector2i, float>(sf::Vector2i(tileList[i]["x"].asInt(), tileList[i]["y"].asInt()), localSum/totalSum));
				}

				spAnimSet->animations[animation.name] = animation;

			}

			m_animationSets[spAnimSet->setName] = spAnimSet;
			pSet = m_animationSets[fileName].get();
		}
		else//we failed to parse successfully
		{
			Print << "\nParse Failed [" << fullPath << "].\n" << FILELINE;
			///ERROR LOG

			pSet = this->request("default");
		}
	}

	return pSet;
}
