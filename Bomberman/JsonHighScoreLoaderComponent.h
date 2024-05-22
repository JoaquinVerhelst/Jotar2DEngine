#pragma once
#include "Component.h"

#include <string>
#include <vector>

namespace Jotar
{
	class GameObject;


	struct HighscoreInfo
	{
		HighscoreInfo()
		{
			name1 = "";
			score1 = 0;
			name2 = "";
			score2 = 0;
		}
		std::string name1;
		int score1;
		std::string name2;
		int score2;
	};



	class JsonHighScoreLoaderComponent : public Component
	{
	public:
		JsonHighScoreLoaderComponent(GameObject* owner, const std::string& highScoreFilePath );
		~JsonHighScoreLoaderComponent() = default;

		JsonHighScoreLoaderComponent(const JsonHighScoreLoaderComponent& other) = delete;
		JsonHighScoreLoaderComponent(JsonHighScoreLoaderComponent&& other) = delete;
		JsonHighScoreLoaderComponent& operator=(const JsonHighScoreLoaderComponent& other) = delete;
		JsonHighScoreLoaderComponent& operator=(JsonHighScoreLoaderComponent&& other) = delete;


		std::vector<HighscoreInfo> LoadHighscoresForSinglePlayer();
		std::vector<HighscoreInfo> LoadHighscoresForCoop();

		void SaveHighscoresToSinglePlayer(const HighscoreInfo& highscores);
		void SaveHighscoresToCoop(const HighscoreInfo& highscore);


	private:

		std::string m_HighScoreFilePath;

	};
}