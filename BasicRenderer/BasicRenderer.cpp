#include "BasicRenderer.h"
#include "World.h"
#include "Ray.h"
#include "Material.h"

namespace BasicRenderer
{
	std::vector<std::string> Renderer::renderingModeStrings =
	{
		"Rasterizer",
		"Raytracer"
	};

	std::vector<std::string> Renderer::shadingModeStrings =
	{
		"Lit",
		"Normal"
	};

	Renderer::Renderer() : m_rasterizer(), m_raytracer(), m_inputMgr()
	{
		m_renderSystems.emplace_back(&m_rasterizer);
		m_renderSystems.emplace_back(&m_raytracer);
	}

	const FrameBuffer* Renderer::Render(uint width, uint height, World& scene, RenderingMode mode, ShadingMode shading, const float deltaTime)
	{
		assert(width > 0 && height > 0);

		if (width != m_width || height != m_height || m_fBuffer == nullptr)
		{
			m_fBuffer = std::make_shared<FrameBuffer>(width, height);

			m_width = width;
			m_height = height;

			scene.GetMainCamera().SetAspectRatio(width, height);
		}

		m_fBuffer->Fill(scene.GetAmbientLightColor() * scene.GetAmbientLightIntensity());

		// TODO Move to separate thread for simulation update
		ProcessInput(m_inputMgr, scene);
		scene.ProcessForRendering();

		auto shadingFunc = &Material::LitShading;

		switch (shading)
		{
		case ShadingMode::NORMAL:
			shadingFunc = &Material::NormalShading;
			break;
		}

		switch (mode)
		{
			default:
			{
				m_rasterizer.Render(*m_fBuffer, scene, shadingFunc);
			}
			break;
			case RenderingMode::RAYTRACER:
			{
				m_raytracer.Render(*m_fBuffer, scene, shadingFunc);
			}
			break;
		}

		// Post Processing
		m_fBuffer->CorrectGamma();
		// TODO tonemapping

		return m_fBuffer.get();
	}

	void Renderer::ProcessInput(InputManager& inputMgr, World& scene)
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

	void Renderer::ProcessButtonInput(const ButtonInputEvent& input, World& scene)
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

	void Renderer::ProcessCursorInput(const InputManager& inputMgr, const CursorInputEvent& input, World& scene)
	{
		const Vector2& cursorDiff = input.GetPosition() - inputMgr.GetLastCursorPosition();
		Camera& camera = scene.GetMainCamera();
		const float cameraRotationSpeed = camera.GetRotationSpeed();

		float ratio = camera.GetAspectRatio();
		const Vector2 cameraRot = { cursorDiff.x * cameraRotationSpeed, cursorDiff.y * ratio * cameraRotationSpeed };

		camera.GetTransform().Rotate(cameraRot.y, cameraRot.x, 0.0f);
	}

	std::string Renderer::GenerateFilename(const std::string& customPrefix, uint imageHeight, RenderingMode mode, ShadingMode shading, uint pixelSamples, uint bounces)
	{
		std::string rtInfo = mode == RenderingMode::RAYTRACER 
							? "_" + std::to_string(pixelSamples) + "ps_" + std::to_string(bounces) + "b"
							: "";

			return customPrefix + "_"
			+ renderingModeStrings[std::underlying_type_t<RenderingMode>(mode)] + "_"
			+ shadingModeStrings[std::underlying_type_t<RenderingMode>(shading)] + "_"
			+ std::to_string(imageHeight) + "p" + rtInfo;
	}

	

}