#pragma once

#include "Global.h"
#include "World.h"
#include "InputManager.h"

namespace BasicRenderer
{
	class Model
	{
	protected:

		std::unique_ptr<World> m_scene;
		InputManager m_inputMgr;

		const uint m_updateFreq = 60u;
		const TimeDuration m_updateTime = TimeDuration(1000000000 / m_updateFreq);

	public:

		Model(World* scene) : m_scene(scene), m_inputMgr() {}
		Model(const Model& other) = delete;
		virtual ~Model();

		Model& operator=(const Model& other) = delete;

		void update();

		inline const World& GetScene()		const	{ return *m_scene; }
		InputManager& GetInputManager()				{ return m_inputMgr; }
		inline uint UpdateFreq()			const	{ return m_updateFreq; }
		inline TimeDuration UpdateTime()	const	{ return m_updateTime; }

		void SetMainCameraAspectRatio(float w, float h) { m_scene->GetMainCamera().SetAspectRatio(static_cast<int>(w), static_cast<int>(h)); }

	protected:

		void ProcessInput(InputManager& inputMgr, World& scene);
		void ProcessButtonInput(const ButtonInputEvent& input, World& scene);
		void ProcessCursorInput(const InputManager& inputMgr, const CursorInputEvent& input, World& scene);
	};
}