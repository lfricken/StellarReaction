#pragma once

#include "WidgetBase.hpp"
#include "EditBox.hpp"
#include "Universe.hpp"
#include "NetworkComponent.hpp"

namespace leon
{
	/// Data to initialize a Chatbox
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
		String backgroundTex;//background texture
	};

	/// Provides a way to send text messages between players in the same lobby.
	class Chatbox : public WidgetBase
	{
	public:
		Chatbox(tgui::Gui& gui, const ChatboxData& rData);///menubar struct constructor
		Chatbox(tgui::Container& container, const ChatboxData& rData);
		virtual ~Chatbox();

		/// Add a line of text to this Chatbox.
		void addLine(const String& rText);

	protected:
		void pack(sf::Packet& rPacket);//give us data to send to the twin in the other world
		void unpack(sf::Packet& rPacket);//process data from our twin

		/**HOOK**/
		virtual bool inputHook(const String rCommand, sf::Packet rData) final;
		virtual void load(const String& fullFilePath);
	private:
		void f_initialize(const ChatboxData& rData);

		NetworkComponent m_nw;
		String m_latest;//the latest line that we tried to enter
		tgui::ChatBox::Ptr m_pChatBox;//the box where it shows up
		sptr<EditBox> m_spEditBox;//the box for entering info
	};
}
