#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class DragComponent;

	/// Used to update 
	class DragUpdater : NonCopyable
	{
	public:
		DragUpdater();
		~DragUpdater();

		/// Given the mouse position, and any DragComponents::isDragging == true objects get moved.
		void update(const sf::Vector2f& rPos);

		/// Give a pointer to a DragComponent listening for dragging events.
		void give(leon::DragComponent* pComp);
		/// Remove an object from the listener.
		void free(leon::DragComponent* pCompToDelete);

	private:
		std::vector<leon::DragComponent*> m_components;
	};
}
