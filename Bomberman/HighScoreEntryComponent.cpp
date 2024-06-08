#include "HighScoreEntryComponent.h"
#include "InputManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameManager.h"
#include "ScoreComponent.h"
#include "TransformComponent.h"


Jotar::HighScoreEntryComponent::HighScoreEntryComponent(GameObject* owner, HighScoreMenuComponent* jsonHighScorLoader, int maxNameLemgth)
	:Component(owner)
	, m_NameText{}
	, m_DisplayText{}
	, m_PlayersSaved{}
	, m_pTextComponent{}
	, m_pHighScoreMenuComponent{ jsonHighScorLoader }
	, m_MaxNameLength{ maxNameLemgth}
{

}

void Jotar::HighScoreEntryComponent::Update()
{
	CheckInput();
}

void Jotar::HighScoreEntryComponent::Start()
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	m_DisplayText = "Player1 Name: ";
	m_pTextComponent->SetText(m_DisplayText + m_NameText);
}



void Jotar::HighScoreEntryComponent::CheckInput()
{
	auto& input = InputManager::GetInstance();
	std::string text = input.GetPressedKeyString();

	if (!text.empty())
	{
		if (m_NameText.length() <= m_MaxNameLength)
		{
			m_NameText += text;
			m_pTextComponent->SetText(m_DisplayText + m_NameText);
		}
	}
	else if (input.IsKeyUp(KeyboardButton::Key_Return))
		SaveName();
	else if (input.IsKeyUp(KeyboardButton::Key_Backscpace))
	{
		size_t length = m_NameText.length() - 1;

		if (length >= 0)
		{
			m_NameText.erase(length);
			m_pTextComponent->SetText(m_DisplayText + m_NameText);
		}
	}
}

void Jotar::HighScoreEntryComponent::UploadHighScoreToJson(bool isCoop)
{
	auto& gameManager = GameManager::GetInstance();
	auto players = gameManager.GetPlayers();

	m_HighscoreEntry.score1 = players[0]->GetOwner()->GetComponent<ScoreComponent>()->GetScore();

	if (isCoop)
	{
		m_HighscoreEntry.score2 = players[1]->GetOwner()->GetComponent<ScoreComponent>()->GetScore();
		m_pHighScoreMenuComponent->GetHighScoreLoader()->SaveHighscoresToCoop(m_HighscoreEntry);
	}
	else
	{
		m_pHighScoreMenuComponent->GetHighScoreLoader()->SaveHighscoresToSinglePlayer(m_HighscoreEntry);
	}

	m_pHighScoreMenuComponent->UpdateHighScoreList();
	GetOwner()->Destroy();
}

void Jotar::HighScoreEntryComponent::SaveName()
{
	if (GameManager::GetInstance().GetGamemode() == GameMode::Coop)
	{
		if (m_PlayersSaved == 0)
		{
			m_HighscoreEntry.name1 = m_NameText;

			++m_PlayersSaved;
			m_NameText = " ";
			m_DisplayText = "Player2 Name: ";
			m_pTextComponent->SetText(m_DisplayText + m_NameText);
		}
		else
		{
			m_HighscoreEntry.name2 = m_NameText;
			UploadHighScoreToJson(true);
		}
	}
	else
	{
		m_HighscoreEntry.name1 = m_NameText;
		UploadHighScoreToJson(false);
	}
}
