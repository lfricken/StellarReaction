#pragma once

#include "WidgetBase.hpp"
#include "EditBox.hpp"
#include "Universe.hpp"
#include "NetworkComponent.hpp"

namespace leon
{
	struct ChatboxData : public WidgetBaseData
	{
		ChatboxData() :
			WidgetBaseData(),
			backgroundColor(sf::Color(255,0,255,128))
		{
			ioComp.name = "ChatboxName";
		}

		EditBoxData editBoxData;
		NetworkComponentData nwCompData;

		sf::Color backgroundColor;//background color if not given a texture
		std::string backgroundTex;//background texture
	};


	class Chatbox : public WidgetBase
	{
	public:
		Chatbox(tgui::Gui& gui, const ChatboxData& rData);///menubar struct constructor
		Chatbox(tgui::Container& container, const ChatboxData& rData);
		virtual ~Chatbox();

		void addLine(const std::string& rText);

	protected:
		void pack(sf::Packet& rPacket);//give us data to send to the twin in the other world
		void unpack(sf::Packet& rPacket);//process data from our twin

		/**HOOK**/
		virtual bool inputHook(const std::string rCommand, sf::Packet rData) final;

	private:
		void f_initialize(const ChatboxData& rData);

		NetworkComponent m_nw;
		std::string m_latest;//the latest line that we tried to enter
		tgui::ChatBox::Ptr m_pChatBox;
		sptr<EditBox> m_spEditBox;
	};
}
