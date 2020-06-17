#pragma once

#include "Vector2.h"

namespace BasicRenderer
{
	enum class InputType
	{
		BUTTON_EVENT,
		MOUSE_EVENT
	};


	struct InputEvent
	{
	public:
		virtual InputType GetType() const = 0;
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
		MOUSE_LEFT,
		MOUSE_RIGHT
	};

	struct ButtonInputEvent : public InputEvent
	{
		const ButtonType buttonType;
		const ButtonState buttonState;

		ButtonInputEvent(ButtonType button, ButtonState state) : buttonType(button), buttonState(state) {}

		InputType GetType() const override { return InputType::BUTTON_EVENT; }
	};

	struct MouseInputEvent : public InputEvent
	{
		const Vector2 position;

		MouseInputEvent(const Vector2 pos) : position(pos) {}

		InputType GetType() const override { return InputType::MOUSE_EVENT; }
	};

}