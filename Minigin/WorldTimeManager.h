#pragma once
#include <chrono>
#include "Singleton.h"


namespace dae
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
        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFrameTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

        float m_Time = 0; // The total time elapsed;
        float m_DeltaTime = 0; // The time elapsed since the last frame
        float m_Fps = 0;
        int m_FrameCount = 0;

        //// Not sure if this is necessary, since not all other singleton classes do this
        ////TODO : look it up!
        //Time() = default;
        //~Time() = default;
    };


}