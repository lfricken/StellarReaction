#ifndef ControlFACTORY_HPP
#define ControlFACTORY_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class Controller;
class NetworkFactory;

/// <summary>
/// Holds the controllers
/// </summary>
class ControlFactory : NonCopyable
{
public:
	ControlFactory();
	virtual ~ControlFactory();

	void processAllDirectives();//call processDirectives on all intelligences
	void addController(const std::string& slave);//creates a new controller with a specified slave
	void resetControllers(const std::vector<std::string>& slaves);//delete current controllers and just have these
	int getSize();//returns the size of controll list
	void unsetLocal();
	Controller& getController(int index);
	NetworkFactory& getNWFactory();

	sptr<NetworkFactory> m_spNWFactory;//must come before the controllers so that everything is destroyed 
	//in the right order, otherwise m_spControlList controllers Network Components cause a crash

	std::vector<sptr<Controller> > m_spControlList;//list of all controllers


protected:
private:

	sptr<Controller> m_spBackupController;//if we need to return a reference to a controller, but none exist, we give this one
};

#endif // ControlFACTORY_HPP
