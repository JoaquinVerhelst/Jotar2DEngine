#pragma once
#include <chrono>
#include "Singleton.h"


namespace Jotar
{
    class WorldTimeManager final : public Singleton<WorldTimeManager>
    {
    public:

        void Init();
        void Update();

        float GetTime() const { return m_Time; }
        float GetDeltaTime() const { return m_DeltaTime; }
        float GetFPS() const { return m_Fps; }

    private:
        friend class Singleton<WorldTimeManager>;
        WorldTimeManager() = default;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFrameTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

        float m_Time = 0; 
        float m_DeltaTime = 0; 
        float m_Fps = 0;
        int m_FrameCount = 0;
    };


}