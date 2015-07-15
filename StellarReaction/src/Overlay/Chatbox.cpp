#include "Chatbox.hpp"

#include "NetworkBoss.hpp"

using namespace leon;
using namespace std;

Chatbox::Chatbox(tgui::Gui& gui, const ChatboxData& rData) : WidgetBase(rData), m_pChatBox(gui), m_nw(rData.nwCompData, &Chatbox::pack, &Chatbox::unpack, this, game.getNwBoss().getNWFactoryTcp())
{
	f_initialize(rData);
}
Chatbox::Chatbox(tgui::Container& container, const ChatboxData& rData = ChatboxData()) : WidgetBase(rData), m_pChatBox(container), m_nw(rData.nwCompData, &Chatbox::pack, &Chatbox::unpack, this, game.getNwBoss().getNWFactoryTcp())
{
	f_initialize(rData);
}
Chatbox::~Chatbox()
{

}
void Chatbox::f_initialize(const ChatboxData& rData)
{
	f_assign(m_pChatBox.get());
	m_pChatBox->load(rData.configFile);
	m_pChatBox->setPosition(rData.screenCoords);
	m_pChatBox->setSize(rData.size.x, rData.size.y);

	EditBoxData ebd = rData.editBoxData;
	ebd.ioComp.name = rData.ioComp.name + "_editbox";
	ebd.screenCoords.x = rData.screenCoords.x;
	ebd.screenCoords.y = rData.screenCoords.y+rData.size.y;

	ebd.size.y = 25;
	ebd.size.x = rData.size.x;

	ebd.startingText = "";
	ebd.startHidden = false;

	Courier enterPressed;
	enterPressed.condition.reset(EventType::ReturnKeyPressed, 0, 'd', true);
	enterPressed.message.reset(rData.ioComp.name, "addLine", voidPacket, 0, true);
	ebd.ioComp.courierList.push_back(enterPressed);

	sf::Packet null;
	null << "";
	Courier enterPressedClear;
	enterPressedClear.condition.reset(EventType::ReturnKeyPressed, 0, 'd', true);
	enterPressedClear.message.reset(ebd.ioComp.name, "setText", null, 0.01f, false);
	ebd.ioComp.courierList.push_back(enterPressedClear);

	m_spEditBox.reset(new leon::EditBox(*m_pChatBox->getParent(), ebd));



	m_pChatBox->bindCallbackEx(&Chatbox::f_callback, this, tgui::ChatBox::AllChatBoxCallbacks);
}
void Chatbox::addLine(const std::string& rText)
{
	m_pChatBox->addLine(rText);
}
void Chatbox::f_callback(const tgui::Callback& callback)
{

}
void Chatbox::input(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "addLine")
	{
		if(game.getNwBoss().isClient())//if we are a client, send it to the nw component
		{
			rData >> m_latest;
			m_nw.toggleNewData(true);
		}
		else//if we are a host, dispatch it and enter it
		{
			///DISPATCH IT SOMEHOW USING NW COMPONENT m_nw.send(command, rData)
			rData >> m_latest;
			m_nw.toggleNewData(true);

			addLine(m_latest);
		}
	}
	else if(rCommand == "addLineLocal")
	{
		std::string line;
		rData >> line;
		addLine(line);
	}
	else if(rCommand == "clear")
	{
		m_pChatBox->removeAllLines();
	}
	else
		WidgetBase::input(rCommand, rData);
}
void Chatbox::pack(sf::Packet& rPacket)//give us data to send to the twin in the other world
{
	rPacket << m_latest;
	m_latest = "";
}
void Chatbox::unpack(sf::Packet& rPacket)//process data from our twin
{
	cout << "\nChatbox unpack.";
	std::string line;
	rPacket >> line;
	addLine(line);
	if(game.getNwBoss().getNWState() == NWState::Server)
	{
		m_nw.toggleNewData(true);
		m_latest = line;
	}
}
