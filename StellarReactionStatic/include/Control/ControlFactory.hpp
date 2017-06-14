#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"
#include "Factory.hpp"

class Controller;
class NetworkFactory;


/// Holds a list of all the Controllers.
class ControlFactory : public NonCopyable
{
public:
	ControlFactory();
	virtual ~ControlFactory();

	///Have all controllers send their commands to their targets.
	void processAllDirectives();
	///Creates a new controller with a specified target. Returns the index of the new controller.
	int addController(const String& target);
	///Delete current controllers, and make a new set of controllers with these targets.
	void resetControllers(const List<String>& targets);
	///How many controllers are there?
	int getSize();
	///Set all controllers to be not local.
	void setAllNonLocallyControlled();
	///Return a particular controller, given it's index.
	Controller* getController(int index);
	///Return reference to the network factory for these controllers.
	NetworkFactory& getNWFactory();
	///Network factory for these controllers.
	sptr<NetworkFactory> m_spNWFactory;
	//must come before the controllers so that everything is destroyed 
	//in the right order, otherwise m_spControlList controllers Network Components cause a crash


	Factory<Controller> m_list;


protected:
private:
};

