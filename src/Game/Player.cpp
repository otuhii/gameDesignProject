#include "pch.h"
#include "Player.h"
#include "CardManager.h"
#include "Texture.h"
#include "Map.h"
#include "utils.h"
#include <iostream>

Player::Player(int posX, int posY, const std::string texturePath)
	: m_MapPosition(posX, posY), m_PlayerState(PlayState::none)
{
	// TODO: randomly initialize cards for players
	m_pCardManager = new CardManager{ Vector2f{200.f, 50.f} };
	m_pTexture = new Texture{ texturePath };
}

Player::~Player()
{
	delete m_pCardManager;
	delete m_pTexture;
}

void Player::Draw(const Rectf& mapSize) const
{
	const float
		cellWidth{ mapSize.width / Map::GetDimensions() },
		cellHeight{ mapSize.height / Map::GetDimensions() },
		squizeBy{ 5.f };

	Vector2f globalPosition{ Map::LocalToGlobalPosition(mapSize, m_MapPosition) };

	glPushMatrix();
	{
		glTranslatef(globalPosition.x+squizeBy, globalPosition.y+squizeBy, 0.f);
		glScalef(2.5f, 2.5f, 1.f);

		m_pTexture->Draw(Vector2f{ 0.f,0.f });
	}
	glPopMatrix();
}

void Player::DrawCards() const
{
	m_pCardManager->DrawCards();
}

void Player::SetPlayerState(PlayState state)
{
	m_PlayerState = state;
}

Player::PlayState Player::GetPlayerState() const
{
	return m_PlayerState;
}

void Player::SetPlayerPosition(const Vector2i& position)
{
	m_MapPosition = position;
}

Vector2i Player::GetPlayerPosition() const
{
	return m_MapPosition;
}

void Player::HoverCards(const Vector2f& mousePosition)
{
	m_pCardManager->CardHoveringHandle(mousePosition);
}

void Player::ProcessHoveredCardClick()
{
	m_pCardManager->UseHoveredCard(this);

}
