#include "Chatbox.hpp"
#include "Globals.hpp"
#include "NetworkBoss.hpp"
#include "Player.hpp"

using namespace leon;

Chatbox::Chatbox(tgui::Gui& gui, const ChatboxData& rData) : WidgetBase(gui, rData), m_pChatBox(gui), m_nw(rData.nwCompData, &Chatbox::pack, &Chatbox::unpack, this, getGame()->getNwBoss().getNWFactoryTcp())
{
	f_initialize(rData);
}
Chatbox::Chatbox(tgui::Container& container, const ChatboxData& rData = ChatboxData()) : WidgetBase(container, rData), m_pChatBox(container), m_nw(rData.nwCompData, &Chatbox::pack, &Chatbox::unpack, this, getGame()->getNwBoss().getNWFactoryTcp())
{
	f_initialize(rData);
}
Chatbox::~Chatbox()
{

}
void Chatbox::f_initialize(const ChatboxData& rData)
{
	f_assign(m_pChatBox.get(), rData);

	{//components
		EditBoxData ebd = rData.editBoxData;
		ebd.ioComp.name = rData.ioComp.name + "_editbox";
		ebd.screenCoords.x = rData.screenCoords.x;
		ebd.screenCoords.y = rData.screenCoords.y + rData.size.y;

		ebd.size.y = 25;
		ebd.size.x = rData.size.x;

		ebd.startingText = "";
		ebd.startHidden = false;

		Courier enterPressed;
		enterPressed.condition.reset(EventType::ReturnKeyPressed, 0, 'd', true);
		enterPressed.message.reset(rData.ioComp.name, "chat", voidPacket, 0, true);
		ebd.ioComp.courierList.push_back(enterPressed);

		sf::Packet clear;//TODO WHAT IS THIS CONDITION FOR???
		clear << "";
		Courier enterPressedClear;
		enterPressedClear.condition.reset(EventType::ReturnKeyPressed, 0, 'd', true);
		enterPressedClear.message.reset(ebd.ioComp.name, "setText", clear, 0.01f, false);
		ebd.ioComp.courierList.push_back(enterPressedClear);

		m_spEditBox.reset(new leon::EditBox(*m_pChatBox->getParent(), ebd));
	}
}
void Chatbox::load(const String& fullFilePath)
{
	m_pChatBox->load(fullFilePath);
}
void Chatbox::addLine(const String& rText)
{
	m_pChatBox->addLine(rText);
}
bool Chatbox::inputHook(const String rCommand, sf::Packet rData)
{
	if(rCommand == "chat")
	{
		String text;
		rData >> text;
		text = getGame()->getLocalPlayer().getName() + ": " + text;

		m_latest = text;
		m_nw.toggleNewData(true);

		if(!getGame()->getNwBoss().isClient())//if we are a host, put it locally as well as to clients
			addLine(m_latest);
		return true;
	}
	else if(rCommand == "addLine")
	{
		String text;
		rData >> text;

		m_latest = text;
		m_nw.toggleNewData(true);

		if(!getGame()->getNwBoss().isClient())//if we are a host, put it locally as well as to clients
			addLine(m_latest);
		return true;
	}
	else if(rCommand == "addLineLocal")
	{
		String line;
		rData >> line;
		addLine(line);
		return true;
	}
	else if(rCommand == "clear")
	{
		m_pChatBox->removeAllLines();
		return true;
	}
	return false;
}
void Chatbox::pack(sf::Packet& rPacket)//give us data to send to the twin in the other world
{
	rPacket << m_latest;
	m_latest = "";
}
void Chatbox::unpack(sf::Packet& rPacket)//process data from our twin
{
	String line;
	rPacket >> line;
	addLine(line);
	if(getGame()->getNwBoss().getNWState() == NWState::Server)
	{
		m_nw.toggleNewData(true);
		m_latest = line;
	}
}
