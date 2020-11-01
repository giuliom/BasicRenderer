#pragma once

#include "Vector2.h"

namespace BasicRenderer
{
	enum class InputCategory
	{
		BUTTON_EVENT,
		CURSOR_EVENT
	};

	class InputEvent
	{
	protected:
		const Vector2 m_position;
		const bool m_validPosition;

	public:
		InputEvent(const Vector2& pos, bool validPos) : m_position(pos), m_validPosition(validPos) {}

		inline virtual InputCategory GetCategory() const = 0;
		inline bool HasValidPosition() { return m_validPosition; }
		inline const Vector2& GetPosition() const { return m_position; }
	};

	enum class ButtonState
	{
		PRESSED,
		RELEASED
	};

	enum class ButtonType
	{
		KEY_W,
		KEY_A,
		KEY_S,
		KEY_D,
		KEY_Q,
		KEY_E,
		KEY_ESCAPE,
		CURSOR_PRIMARY,
		CURSOR_SECONDARY
	};

	class ButtonInputEvent : public InputEvent
	{
		const ButtonType m_buttonType;
		const ButtonState m_buttonState;

	public:
		ButtonInputEvent(ButtonType button, ButtonState state) : InputEvent({ 0.f, 0.f }, false), m_buttonType(button), m_buttonState(state) {}
		ButtonInputEvent(ButtonType button, ButtonState state, const Vector2& pos) : InputEvent(pos, true), m_buttonType(button), m_buttonState(state) {}

		inline InputCategory GetCategory() const override { return InputCategory::BUTTON_EVENT; }
		inline ButtonType GetButtonType() const { return m_buttonType; }
		inline ButtonState GetButtonState() const { return m_buttonState; }
	};

	class CursorInputEvent : public InputEvent
	{
	public:
		CursorInputEvent(const Vector2 pos) : InputEvent(pos, true) {}

		inline InputCategory GetCategory() const override { return InputCategory::CURSOR_EVENT; }
	};

}