#include "JsonHighScoreLoaderComponent.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;



Jotar::JsonHighScoreLoaderComponent::JsonHighScoreLoaderComponent(GameObject* owner, const std::string& highScoreFilePath)
    :Component(owner)
    ,m_HighScoreFilePath{ highScoreFilePath }
{
}



std::vector<Jotar::HighscoreInfo> Jotar::JsonHighScoreLoaderComponent::LoadHighscoresForSinglePlayer()
{
    std::ifstream file(m_HighScoreFilePath);
    if (!file.is_open())
    {
        std::cout << "Failed to open JSON file: " << m_HighScoreFilePath << std::endl;
        return std::vector<HighscoreInfo>();
    }

    try
    {
        json jsonData;
        file >> jsonData;

        std::vector<HighscoreInfo> highscores;

        // Extract highscores from JSON
        const json& entries = jsonData["singleplayer"];
        for (const auto& entry : entries)
        {
            HighscoreInfo highscore;
            highscore.name1 = entry["name"];
            highscore.score1 = entry["score"];
            highscores.push_back(highscore);
        }

        return highscores;
    }
    catch (const json::exception& ex)
    {
        std::cout << "Failed to parse JSON file: " << m_HighScoreFilePath << std::endl;
        std::cout << "Error: " << ex.what() << std::endl;
        return std::vector<HighscoreInfo>();
    }
}

std::vector<Jotar::HighscoreInfo> Jotar::JsonHighScoreLoaderComponent::LoadHighscoresForCoop()
{
    std::ifstream file(m_HighScoreFilePath);
    if (!file.is_open())
    {
        std::cout << "Failed to open JSON file: " << m_HighScoreFilePath << std::endl;
        return std::vector<HighscoreInfo>();
    }

    try
    {
        json jsonData;
        file >> jsonData;

        std::vector<HighscoreInfo> highscores;

        // Extract highscores from JSON
        const json& entries = jsonData["coop"];
        for (const auto& entry : entries)
        {
            HighscoreInfo highscore;

            highscore.name1 = entry["name1"];
            highscore.score1 = entry["score1"];

            highscore.name2 = entry["name2"];
            highscore.score2 = entry["score2"];

            highscores.push_back(highscore);
        }

        return highscores;
    }
    catch (const json::exception& ex)
    {
        std::cout << "Failed to parse JSON file: " << m_HighScoreFilePath << std::endl;
        std::cout << "Error: " << ex.what() << std::endl;
        return std::vector<HighscoreInfo>();
    }
}

void Jotar::JsonHighScoreLoaderComponent::SaveHighscoresToSinglePlayer(const HighscoreInfo& highscores)
{
    json jsonData;

    // Load existing JSON data from file
    std::ifstream inputFile(m_HighScoreFilePath);
    if (inputFile.is_open())
    {
        inputFile >> jsonData;
        inputFile.close();
    }
    else
    {
        std::cout << "Failed to open JSON file for loading: " << m_HighScoreFilePath << std::endl;
        return;
    }

    // Convert highscores to JSON
    json entryJson;
    entryJson["name"] = highscores.name1;
    entryJson["score"] = highscores.score1;
    jsonData["singleplayer"].push_back(entryJson);

    // Write updated JSON data to file
    std::ofstream outputFile(m_HighScoreFilePath);
    if (outputFile.is_open())
    {
        outputFile << jsonData.dump(4); // Write JSON data to file with indentation
        outputFile.close();
        std::cout << "Highscores saved to JSON file: " << m_HighScoreFilePath << std::endl;
    }
    else
    {
        std::cout << "Failed to open JSON file for saving: " << m_HighScoreFilePath << std::endl;
    }
}

void Jotar::JsonHighScoreLoaderComponent::SaveHighscoresToCoop(const HighscoreInfo& highscores)
{

    json jsonData;

    // Load existing JSON data from file
    std::ifstream inputFile(m_HighScoreFilePath);
    if (inputFile.is_open())
    {
        inputFile >> jsonData;
        inputFile.close();
    }
    else
    {
        std::cout << "Failed to open JSON file for loading: " << m_HighScoreFilePath << std::endl;
        return;
    }

    // Convert highscores to JSON
    json entryJson;

    entryJson["name1"] = highscores.name1;
    entryJson["score1"] = highscores.score1;
    entryJson["name2"] = highscores.name2;
    entryJson["score2"] = highscores.score2;

    jsonData["coop"].push_back(entryJson);

    // Write updated JSON data to file
    std::ofstream outputFile(m_HighScoreFilePath);
    if (outputFile.is_open())
    {
        outputFile << jsonData.dump(8); // Write JSON data to file with indentation
        outputFile.close();
        std::cout << "Highscores saved to JSON file: " << m_HighScoreFilePath << std::endl;
    }
    else
    {
        std::cout << "Failed to open JSON file for saving: " << m_HighScoreFilePath << std::endl;
    }
}
