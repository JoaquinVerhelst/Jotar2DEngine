#include "GameManager.h"
#include "GameObject.h"

void Jotar::GameManager::SetPlayerOne(std::shared_ptr<GameObject> ptr)
{
    m_PlayerOne = ptr;
}

void Jotar::GameManager::SetPlayerTwo(std::shared_ptr<GameObject> ptr)
{
    m_PlayerTwo = ptr;
}

std::shared_ptr<Jotar::GameObject> Jotar::GameManager::GetPlayerOne()
{
    return m_PlayerOne;
}

std::shared_ptr<Jotar::GameObject> Jotar::GameManager::GetPlayerTwo()
{
    return m_PlayerTwo;
}

void Jotar::GameManager::ResetAndInitializeWorldGrid(int rows, int columns, int size)
{
    m_pWorldGrid = std::make_unique<WorldGrid>(rows, columns, size);
}

Jotar::WorldGrid* Jotar::GameManager::GetWorldGrid()
{
    return m_pWorldGrid.get();
}
