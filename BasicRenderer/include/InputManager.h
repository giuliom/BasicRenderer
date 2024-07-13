#pragma once

#include <queue>
#include <memory>
#include "InputEvent.h"

namespace BasicRenderer
{
	class InputManager
	{
		std::queue<std::unique_ptr<InputEvent>> m_events;
		Vector2 m_lastCursorPosition;

	public:

		InputManager() : m_events(), m_lastCursorPosition(0.f, 0.f) {}

		inline bool Empty() const { return m_events.size() == 0; }

		inline const Vector2& GetLastCursorPosition() const { return m_lastCursorPosition; }
		inline void SetLastCursorPosition(const Vector2& position) { m_lastCursorPosition = position; }

		void AddEvent(std::unique_ptr<InputEvent>& event);
		std::unique_ptr<InputEvent> PopFrontEvent();
		void Clear();
	};
}