#include "pch.h"
#include "Player.h"
#include "Card.h"


Player::Player(int posX, int posY)
{
	// TODO: randomly initialize cards for players
}

Player::~Player()
{
	for (int index{ 0 }; index < m_MaximumCardNumber; ++index)
	{
		if (m_pCards[index] != nullptr)
		{
			delete m_pCards[index];
		}
	}
}

const Card* Player::GetPickedCard() const
{
	return m_PickedCard;
}
