#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class DragComponent;

	/// <summary>
	/// Held by getGame()-> has update called to send mouse events to the drag components.
	/// </summary>
	class DragUpdater : NonCopyable
	{
	public:
		DragUpdater();
		~DragUpdater();

		/// <summary>
		/// Given the mouse position as screen coordinates, any DragComponents::isDragging == true objects get moved to that position.
		/// </summary>
		void update(const sf::Vector2f& rPos);

		/// Give a pointer to a DragComponent listening for dragging events.
		void give(leon::DragComponent* pComp);
		/// Remove an object from the listener.
		void freeThis(leon::DragComponent* pCompToDelete);

	private:
		List<leon::DragComponent*> m_components;
	};
}
