#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class DragUpdater;

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

		bool isDragging() const;
		void toggleDragging(bool dragging);
		void updateDrag(const sf::Vector2f& rPos);

	protected:
	private:
		void f_giveThyself();
		std::function<void(const sf::Vector2f&)> m_cbFunction;
		bool m_isDragging;
		DragUpdater* m_pOwner;
	};
}
