#include "GameObject.hpp"

using namespace std;

GameObject::GameObject(const GameObjectData& rData) : m_io(rData.ioComp, &GameObject::input, this), m_nw(rData.nwComp, &GameObject::pack, &GameObject::unpack, this, game.getNwBoss().getNWFactory())
{

}
GameObject::~GameObject()
{

}
void GameObject::input(std::string rCommand, sf::Packet rData)
{
	cout << "\nCommand not found in [" << m_io.getName() << "].";
}
void GameObject::pack(sf::Packet& rPacket)
{

}
void GameObject::unpack(sf::Packet& rPacket)
{

}
