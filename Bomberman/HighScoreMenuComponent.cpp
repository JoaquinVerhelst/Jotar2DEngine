#include "HighScoreMenuComponent.h"
#include "GameObject.h"
#include "JsonHighScoreLoaderComponent.h"
#include "TextComponent.h"


Jotar::HighScoreMenuComponent::HighScoreMenuComponent(GameObject* owner, const std::shared_ptr<Font>& font, glm::ivec2 windowSize)
	:Component(owner)
	, m_Font{ font }
	, m_WindowSize{ windowSize }
{ 
}

void Jotar::HighScoreMenuComponent::Update()
{
}

void Jotar::HighScoreMenuComponent::Start()
{
	m_pJsonHighScoreLoader = GetOwner()->GetComponent<JsonHighScoreLoaderComponent>();
	UpdateHighScoreList();
}


void Jotar::HighScoreMenuComponent::UpdateHighScoreList()
{
	for (size_t i = 0; i < m_ScoreTextObjects.size(); i++)
	{
		m_ScoreTextObjects[i]->Destroy();
	}

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

	float offset = 50.f;

	for (size_t i = 0; i < highScoreSinglePlayer.size(); ++i)
	{
		auto go = GetOwner()->CreateChildGameObject("SinglePlayer ScoreText" + std::to_string(i), false, false);

		std::string scoreText = std::to_string(highScoreSinglePlayer[i].score1) + "             " + highScoreSinglePlayer[i].name1;

		auto textComp = go->AddComponent<TextComponent>(scoreText, m_Font);
		go->GetTransform()->Translate({ -m_WindowSize.x / 4.f - textComp->GetSize().x / 2,  -100.f + offset * i});
			
		m_ScoreTextObjects.emplace_back(go.get());
	}


	for (size_t i = 0; i < highScoreCoop.size(); ++i)
	{
		auto go = GetOwner()->CreateChildGameObject("Coop ScoreText" + std::to_string(i), false, false);

		std::string scoreText = std::to_string(highScoreCoop[i].score1) + ": " + highScoreCoop[i].name1 + "        "
			+ std::to_string(highScoreCoop[i].score2) + ": " + highScoreCoop[i].name2;

		auto textComp = go->AddComponent<TextComponent>(scoreText, m_Font);
		go->GetTransform()->Translate({ m_WindowSize.x / 4.f - textComp->GetSize().x / 2,  -100.f + offset * i });


		m_ScoreTextObjects.emplace_back(go.get());
	}

}


