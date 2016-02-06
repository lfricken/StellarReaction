#ifndef ANIMATION_HPP
#define ANIMATION_HPP

/// <summary>
/// A particular set of animation tiles, like "Run"
/// </summary>
struct Animation
{
	std::string name;//name of this state
	bool repeats;//should we repeat
	std::vector<std::pair<sf::Vector2i, float> > tileSet;//texture tiles, %time to be on each one
};

/// <summary>
/// Stores animations like ["Run","Jump"] etc.
/// </summary>
struct AnimSet
{
	std::string setName;//name of our file including directory MINUS textures/
	sf::Vector2f tileSize;//size of texture tiles
	std::map<std::string, Animation> animations;//name of <Animation, the Animation>
};

#endif // ANIMATION_HPP
