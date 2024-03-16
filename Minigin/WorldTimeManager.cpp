#include "WorldTimeManager.h"



namespace Jotar
{
    void WorldTimeManager::Init()
    {
        m_LastFrameTime = m_StartTime = std::chrono::high_resolution_clock::now();
    }

    void WorldTimeManager::Update()
    {
        // Get the current time and calculate the delta time since the last frame
        auto currentTime = std::chrono::high_resolution_clock::now();
        m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastFrameTime).count();
        m_LastFrameTime = currentTime;
        m_Time += m_DeltaTime;

        // Update the FPS counter
        m_FrameCount++;
        auto elapsedSeconds = std::chrono::duration<float>(currentTime - m_StartTime).count();

        if (elapsedSeconds >= 1.0f) {
            m_Fps = static_cast<float>(m_FrameCount) / elapsedSeconds;
            m_FrameCount = 0;
            m_StartTime = currentTime;
        }
    }

}