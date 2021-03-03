#include "Model.h"
#include "RenderState.h"

namespace BasicRenderer
{
	Model::~Model()
	{
	}

	void Model::Update(const TimeDuration& deltaTime)
	{
		ProcessInput(m_inputMgr, *m_scene);

		m_scene->Update(deltaTime);
	}

	RenderState* Model::ProcessForRendering()
	{
		std::vector<Primitive*> copiedPrimitives;

		m_scene->ProcessForRendering(copiedPrimitives);

		return new RenderState(m_scene->GetMainCamera(), m_scene->GetEnvironmentSettings(), copiedPrimitives);
	}

	void Model::ProcessInput(InputManager& inputMgr, World& scene)
	{
		// deltaTime not used because of buffered input
		while (inputMgr.Empty() == false)
		{
			auto& event = inputMgr.PopFrontEvent();

			switch (event->GetCategory())
			{
			case InputCategory::BUTTON_EVENT:
			{
				const ButtonInputEvent& buttonEvent = dynamic_cast<ButtonInputEvent&>(*event.get());
				ProcessButtonInput(buttonEvent, scene);
				break;
			}
			case InputCategory::CURSOR_EVENT:
			{
				const CursorInputEvent& cursorEvent = dynamic_cast<CursorInputEvent&>(*event.get());
				ProcessCursorInput(inputMgr, cursorEvent, scene);
				break;
			}
			}

			// updating last cursor position
			if (event->HasValidPosition())
			{
				inputMgr.SetLastCursorPosition(event->GetPosition());
			}
		}
	}

	void Model::ProcessButtonInput(const ButtonInputEvent& input, World& scene)
	{
		Camera& camera = scene.GetMainCamera();
		Vector3 cameraPos(0.f, 0.f, 0.f);
		const float cameraSpeed = camera.GetMovementSpeed();

		if (input.GetButtonState() == ButtonState::PRESSED)
		{
			switch (input.GetButtonType())
			{
			case ButtonType::KEY_W:
			{
				cameraPos.z += cameraSpeed;
				break;
			}
			case ButtonType::KEY_A:
			{
				cameraPos.x -= cameraSpeed;
				break;
			}
			case ButtonType::KEY_S:
			{
				cameraPos.z -= cameraSpeed;
				break;
			}
			case ButtonType::KEY_D:
			{
				cameraPos.x += cameraSpeed;
				break;
			}
			case ButtonType::KEY_Q:
			{
				cameraPos.y -= cameraSpeed;
				break;
			}
			case ButtonType::KEY_E:
			{
				cameraPos.y += cameraSpeed;
				break;
			}

			case ButtonType::CURSOR_PRIMARY:
			{
				break;
			}

			case ButtonType::CURSOR_SECONDARY:
			{
				break;
			}
			}
		}
		else if (input.GetButtonState() == ButtonState::RELEASED)
		{
			switch (input.GetButtonType())
			{
			case ButtonType::CURSOR_PRIMARY:
			{
				break;
			}

			case ButtonType::CURSOR_SECONDARY:
			{
				break;
			}
			}
		}

		if (cameraPos.Length() != 0.f)
		{
			const Vector3 forward = camera.GetTransform().Right() * cameraPos.z;
			const Vector3 right = camera.GetTransform().Forward() * cameraPos.x;
			const Vector3 up = camera.GetTransform().Up() * cameraPos.y;
			camera.GetTransform().Translate(forward + right + up);
		}
	}

	void Model::ProcessCursorInput(const InputManager& inputMgr, const CursorInputEvent& input, World& scene)
	{
		const Vector2& cursorDiff = input.GetPosition() - inputMgr.GetLastCursorPosition();
		Camera& camera = scene.GetMainCamera();
		const float cameraRotationSpeed = camera.GetRotationSpeed();

		float ratio = camera.GetAspectRatio();
		const Vector2 cameraRot = { cursorDiff.x * cameraRotationSpeed, cursorDiff.y * ratio * cameraRotationSpeed };

		camera.GetTransform().Rotate(cameraRot.y, cameraRot.x, 0.0f);
	}

}