#include "HighScoreMenuComponent.h"
#include "GameObject.h"
#include "JsonHighScoreLoaderComponent.h"
#include "TextComponent.h"
#include "HUDComponent.h"


Jotar::HighScoreMenuComponent::HighScoreMenuComponent(GameObject* owner, const std::shared_ptr<Font>& font, glm::ivec2 windowSize, float baseOffset, float betweenOffset)
	:Component(owner)
	, m_Font{ font }
	, m_WindowSize{ windowSize }
	, m_pJsonHighScoreLoader{nullptr}
	, m_BaseOffset{baseOffset}
	, m_BetweenOffset{betweenOffset}		
{
}


void Jotar::HighScoreMenuComponent::Start()
{
	m_pJsonHighScoreLoader = GetOwner()->GetComponent<JsonHighScoreLoaderComponent>();
	UpdateHighScoreList();
}

void Jotar::HighScoreMenuComponent::UpdateHighScoreList()
{

	auto children = GetOwner()->GetChildren();

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->Destroy();
	}

	GetOwner()->CheckDestoryChildren();


	auto highScoreCoop = m_pJsonHighScoreLoader->LoadHighscoresForCoop();
	auto highScoreSinglePlayer = m_pJsonHighScoreLoader->LoadHighscoresForSinglePlayer();

	//Sort
	std::sort(highScoreSinglePlayer.begin(), highScoreSinglePlayer.end(), [](const HighscoreInfo& a, const HighscoreInfo& b)
		{
			return a.score1 > b.score1;
		});

	std::sort(highScoreCoop.begin(), highScoreCoop.end(), [](const HighscoreInfo& a, const HighscoreInfo& b)
		{
			int totalScore1 = a.score1 + a.score2;
			int totalScore2 = b.score1 + b.score2;
			return totalScore1 > totalScore2;
		});

	for (size_t i = 0; i < highScoreSinglePlayer.size(); ++i)
	{
		std::string scoreText = std::to_string(highScoreSinglePlayer[i].score1) + "             " + highScoreSinglePlayer[i].name1;
		glm::vec2 pos{ -m_WindowSize.x / 4.f,  m_BaseOffset + m_BetweenOffset * i};
		CreateScoreTextGameObject("Singleplayer", scoreText, pos);
	}

	for (size_t i = 0; i < highScoreCoop.size(); ++i)
	{
		std::string scoreText = std::to_string(highScoreCoop[i].score1) + ": " + highScoreCoop[i].name1 + "        "
			+ std::to_string(highScoreCoop[i].score2) + ": " + highScoreCoop[i].name2;

		glm::vec2 pos{ m_WindowSize.x / 4.f,  m_BaseOffset + m_BetweenOffset * i };
		CreateScoreTextGameObject("Coop", scoreText, pos);
	}
}

Jotar::JsonHighScoreLoaderComponent* Jotar::HighScoreMenuComponent::GetHighScoreLoader() const
{
	return m_pJsonHighScoreLoader;
}

void Jotar::HighScoreMenuComponent::CreateScoreTextGameObject(std::string gameMode, std::string scoreText, glm::vec2 position)
{
	auto go = GetOwner()->CreateChildGameObject(gameMode + "coreText", false, false);
	go->AddComponent<HUDComponent>();
	auto textComp = go->AddComponent<TextComponent>(scoreText, m_Font);
	go->GetTransform()->Translate({ position.x - textComp->GetSize().x / 2.f, position.y });
	go->Start();

}


