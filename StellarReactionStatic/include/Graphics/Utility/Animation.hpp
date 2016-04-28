#pragma once

/// A particular set of animation tiles, like "Run".
struct Animation
{
	/// Name of this state.
	std::string name;
	/// Whether this Animation should repeat when done.
	bool repeats;
	/// Map of animations and the percent of time to spend on that tile.
	std::vector<std::pair<sf::Vector2i, float> > tileSet;
};

/// Stores a list of animations like ["Run","Jump"] etc.
struct AnimSet
{
	/// Name of our file path from textures folder.
	std::string setName;
	/// Dimensions of a single frame of an animation.
	sf::Vector2f tileSize;
	/// Map of animation name and the actual Animation.
	std::map<std::string, Animation> animations;
};

