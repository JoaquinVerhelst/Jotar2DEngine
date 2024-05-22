#pragma once
#include "Component.h"
#include <string>
#include "JsonHighScoreLoaderComponent.h"



namespace Jotar
{
	class GameObject;
	class TextComponent;

	class HighScoreEntryComponent : public Component
	{
	public:
		HighScoreEntryComponent(GameObject* owner);
		~HighScoreEntryComponent() = default;

		void Update() override;
		void Start() override;

		HighScoreEntryComponent(const HighScoreEntryComponent& other) = delete;
		HighScoreEntryComponent(HighScoreEntryComponent&& other) = delete;
		HighScoreEntryComponent& operator=(const HighScoreEntryComponent& other) = delete;
		HighScoreEntryComponent& operator=(HighScoreEntryComponent&& other) = delete;



	private:

		void UploadHighScoreToJson(bool isCoop);

		void SaveName();

		void CheckInput();

		HighscoreInfo m_HighscoreEntry;
		std::string m_NameText;
		std::string m_DisplayText;
		TextComponent* m_pTextComponent;
		int m_PlayersSaved;
	};
}