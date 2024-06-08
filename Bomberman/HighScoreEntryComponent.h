#pragma once
#include "Component.h"
#include <string>
#include "HighScoreMenuComponent.h"



namespace Jotar
{
	class GameObject;
	class TextComponent;

	class HighScoreEntryComponent : public Component
	{
	public:
		HighScoreEntryComponent(GameObject* owner, HighScoreMenuComponent* highScoreMenuComp, int maxNameLemgth = 10);
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

		HighScoreMenuComponent* m_pHighScoreMenuComponent;
		HighscoreInfo m_HighscoreEntry;
		std::string m_NameText;
		std::string m_DisplayText;
		TextComponent* m_pTextComponent;
		int m_PlayersSaved;
		int m_MaxNameLength;
	};
}