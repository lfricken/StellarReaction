#include "AnimAlloc.hpp"
#include "Animation.hpp"

#include "Globals.hpp"

using namespace std;

AnimAlloc::AnimAlloc()
{

}
AnimAlloc::~AnimAlloc()
{

}
const AnimSet* AnimAlloc::request(const std::string& rAnimFile)
{
	const AnimSet* pSet = NULL;
	auto it_find = m_animationSets.find(rAnimFile);


	if(it_find != m_animationSets.end())//if it exists
	{
		pSet = &*(it_find->second);
	}
	else//if it doesn't exist
	{
		sptr<AnimSet> spAnimSet(new AnimSet);
		Animation animation;


		const std::string fullPath = contentDir() + "textures/" + rAnimFile;
		Json::Value root;
		Json::Reader reader;
		std::ifstream inputFStream(fullPath, std::ifstream::binary);
		bool parsedSuccess = reader.parse(inputFStream, root, false);

		/**== SUCCESSFUL PARSE ==**/
		if(parsedSuccess)
		{

			spAnimSet->setName = rAnimFile;
			spAnimSet->tileSize = sf::Vector2f(root["texTileSize"][0].asInt(), root["texTileSize"][1].asInt());
			const Json::Value stateList = root["stateList"];

			int counter = 0;
			for(auto it = stateList.begin(); it != stateList.end(); ++it)//get all the state settings
			{

				Animation animation;

				animation.name = (*it)["state"].asString();
				animation.repeats = (*it)["repeats"].asBool();
				const Json::Value tileList = (*it)["tileList"];

				for(unsigned int i = 0; i<tileList.size(); ++i)
				{
					animation.tileSet.push_back(std::pair<sf::Vector2i, float>(sf::Vector2i(tileList[i]["x"].asInt(), tileList[i]["y"].asInt()), tileList[i]["t"].asFloat()));
				}

				spAnimSet->animations[animation.name] = animation;

			}

			m_animationSets[spAnimSet->setName] = spAnimSet;
			pSet = m_animationSets[rAnimFile].get();
		}
		else//we failed to parse successfully
		{
			std::cout << "\nParse Failed [" << fullPath << "].\n" << FILELINE;
			///ERROR LOG

			pSet = this->request("default.acfg");
		}
	}

	return pSet;
}
