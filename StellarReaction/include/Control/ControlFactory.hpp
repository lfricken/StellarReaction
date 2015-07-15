#ifndef ControlFACTORY_HPP
#define ControlFACTORY_HPP

#include "stdafx.hpp"

class Controller;
class NetworkFactory;

/**Creates Controllers and holds them**/
class ControlFactory
{
public:
	ControlFactory();
	virtual ~ControlFactory();

	void processAllDirectives();//call processDirectives on all intelligences
	void addController(const std::string& slave);//creates a new controller with a specified slave
	void resetControllers(const std::vector<std::string>& slaves);//delete current controllers and just have these
	void unsetLocal();
	Controller& getController(int index);
	NetworkFactory& getNWFactory();

protected:
private:
	sptr<NetworkFactory> m_spNWFactory;
	std::vector<sptr<Controller> > m_spControlList;
	sptr<Controller> m_spBackupController;
};

#endif // ControlFACTORY_HPP
