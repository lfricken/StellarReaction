#pragma once

#include "stdafx.hpp"

namespace leon
{
	class DragComponent;

	class DragUpdater
	{
	public:
		DragUpdater();
		~DragUpdater();

		void update(const sf::Vector2f& rPos);

		void give(leon::DragComponent* pComp);
		void free(leon::DragComponent* pCompToDelete);

	private:
		std::vector<leon::DragComponent*> m_components;
	};
}