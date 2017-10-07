#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class DragUpdater;

	/// <summary>
	/// A small class to get the mouse coordinates when being dragged.
	/// </summary>
	class DragComponent : NonCopyable
	{
	public:
		template <typename T>
		DragComponent(void (T::*func)(const sf::Vector2f&), T* const classPtr, DragUpdater* pOwner)
		{
			m_cbFunction = std::bind(func, classPtr, std::placeholders::_1);
			m_pOwner = pOwner;
			f_giveThyself();
			m_isDragging = true;
		}
		~DragComponent();

		/// <summary>
		/// Return whether this object is being dragged.
		/// </summary>
		bool isDragging() const;
		/// <summary>
		/// Set whether this object is being dragged.
		/// </summary>
		void toggleDragging(bool dragging);
		/// <summary>
		/// Given the mouse position in screen coordinates, any DragComponents::isDragging == true objects get moved.
		/// </summary>
		void updateDrag(const sf::Vector2f& rPos);

	protected:
	private:
		void f_giveThyself();
		std::function<void(const sf::Vector2f&)> m_cbFunction;
		bool m_isDragging;
		DragUpdater* m_pOwner;
	};
}
