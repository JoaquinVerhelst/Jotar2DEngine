#pragma once
#include <steam_api.h>
#include "Singleton.h"
#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }


namespace Jotar
{
	struct Achievement_t
	{
		int m_eAchievementID;
		const char* m_pchAchievementID;
		char m_rgchName[128];
		char m_rgchDescription[256];
		bool m_bAchieved;
		int m_iIconImage;
	};

	class CSteamAchievements : public Singleton<CSteamAchievements>
	{
	private:
		uint64 m_iAppID; // Our current AppID
		Achievement_t* m_pAchievements; // Achievements data
		int m_iNumAchievements; // The number of Achievements
		bool m_bInitialized; // Have we called Request stats and received the callback?

	public:
		CSteamAchievements();
		~CSteamAchievements();

		void Init(Achievement_t* Achievements, int NumAchievements);
		void Destroy();


		bool RequestStats();
		bool SetAchievement(const char* ID);

		STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
			m_CallbackUserStatsReceived);
		STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t,
			m_CallbackUserStatsStored);
		STEAM_CALLBACK(CSteamAchievements, OnAchievementStored,
			UserAchievementStored_t, m_CallbackAchievementStored);
	};
}