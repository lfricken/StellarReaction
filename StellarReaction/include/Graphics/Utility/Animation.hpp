#ifndef ANIMATION_HPP
#define ANIMATION_HPP


struct Animation
{
	std::string name;//name of this state
	bool repeats;//should we repeat
	std::vector<std::pair<sf::Vector2i, float> > tileSet;//texture tiles, %time to be on each one
};

struct AnimSet
{
	std::string setName;//name of our file including directory MINUS textures/
	sf::Vector2f tileSize;//size of texture tiles
	std::map<std::string, Animation> animations;//name of <Animation, the Animation>
};

#endif // ANIMATION_HPP
