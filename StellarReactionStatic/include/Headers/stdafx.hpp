#ifndef STDAFX_H
#define STDAFX_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <functional>
#include <random>
#include <chrono>

#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <assert.h>
#include <intrin.h>
#include <stdint.h>


#include <SFML/System.hpp>/**SFML**/
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <TGUI/TGUI.hpp>/**TGUI**/

#include <Box2D/Box2D.h>/**BOX2D**/

#include <JSON/json.h>/**JSON**/


template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T>
using Sp = std::shared_ptr<T>;

template<typename T>
using Wp = std::weak_ptr<T>;

template<typename T>
using List = std::vector<T>;

template<typename T, typename R>
using Map = std::map<T, R>;

using Vec2 = b2Vec2;

#endif // STFAFX_H



/**I would have written less code, but I don't have the time.**/

/** Coding Nomenclature and Standard Practices


DOCUMENTING:

//minor addition
/ ** significant information ** /


INCLUDING:
Always have an include guard. (actually do #pragma once)
Never include .cpp files!
use #include "filename" for files we created
use #include <filename> for files we didn't create
#define constants etc go in stdafx.h

COMMENTING:
If there is something that needs to be created or edited in the future, put ///TODO
If you are writing something for the purpose of debugging, put //debug after it!

GENERAL STUFF:
Use auto for iterators and names that are stupidly long.
Only make functions virtual if you actually plan to override them.
Only make stuff protected if the class will actually have a child.
Always inherit publicly.
Never use any "using namespace" in header files.
Comment things that aren't obvious, but don't comment things that are obvious, like getVariableName and setVariableName
Most struct and POD objects should have a constructor that creates a really generic version of itself with some properties for testing

Pass stuff around by value if its an individual STL data type, like in or string, const reference when its more complex data
Use pointers if the lifetime of an object is not known.
Use const when possible


POINTERS:
Things that cause pointers to go wrong are:
Objects getting destroyed when you think they aren't going to be.
std::vectors destroy and realloc when you add or remove
Any STL containers for that matter.


SFML:
Use Vector2f for dealing with screen coordinates only.
Try to use b2Vec2 for everything else. They are the standard unit of measurement



COMMON ABREVIATIONS:
position    pos
texture	    tex
coordinates coords
pixels      pix
vertices    vert


PREFIX: ControlTUATION:              EXAMPLE:
m_  private member variable     m_test;
p   variable is a raw pointer   pBadBear;
it_ variable is an iterator     it_data; //if its the only iterator in the loop, just use "it" for the name

**/
