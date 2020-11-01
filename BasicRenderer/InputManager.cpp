#include "InputManager.h"

namespace BasicRenderer
{
    void BasicRenderer::InputManager::AddEvent(std::unique_ptr<InputEvent>& event)
    {
        m_events.push(std::move(event));
    }

    std::unique_ptr<InputEvent> BasicRenderer::InputManager::PopFrontEvent()
    {
       std::unique_ptr<InputEvent> event = std::move(m_events.front());
       m_events.pop();
       return event;
    }

    void BasicRenderer::InputManager::Clear()
    {
        std::queue<std::unique_ptr<InputEvent>> empty;
        std::swap(m_events, empty);
    }
}