#include "pch.h"
#include "CardManager.h"
#include "utils.h"
#include "Player.h"

CardManager::CardManager(const Vector2f& drawPos)
	:m_DrawPos(drawPos)
{
	const Vector2f posStep{
		30.f,
		3.f
	};
	const float angleStep{ -10.f };

	Vector2f pos{};
	float rotationAngle{ 30.f };

	int decreaseAfter{ m_CurrentCardNumber / 2 };

	for (int index = 0; index < m_MaximumCardNumber; ++index)
	{
		Card::CardType type{ static_cast<Card::CardType>(rand() % 3) };

		m_pCards[index] = new Card{pos, rotationAngle, type};

		if (index < decreaseAfter)
		{
			pos.y += posStep.y;
		}
		else
		{
			pos.y -= posStep.y;
		}
		pos.x += posStep.x;
		rotationAngle += angleStep;
	}
}

CardManager::~CardManager()
{
	for (int index = 0; index < m_MaximumCardNumber; ++index)
	{
		delete m_pCards[index];
	}
}


void CardManager::DrawCards() const
{
	for (int index = 0; index < m_MaximumCardNumber; ++index)
	{
		if (m_pCards[index] != nullptr)
		{
			glPushMatrix();
			{
				if (Card::GetHoveredCard() != index)
				{
					Vector2f cardPos{ m_pCards[index]->GetCardPosition() };

					glTranslatef(Card::GetCardDimensions().x * 0.5f, Card::GetCardDimensions().y * 0.5f, 0.f);
					glTranslatef(m_DrawPos.x + cardPos.x, m_DrawPos.y + cardPos.y, 0.f);
					glRotatef(m_pCards[index]->GetRotationAngle(), 0.f, 0.f, 1.f);
					glTranslatef(-Card::GetCardDimensions().x * 0.5f, -Card::GetCardDimensions().y * 0.5f, 0.f);

					utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
					utils::FillRect(Vector2f{ 0.f, 0.f }, Card::GetCardDimensions().x, Card::GetCardDimensions().y);

					utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
					utils::DrawRect(Vector2f{ 0.f, 0.f }, Card::GetCardDimensions().x, Card::GetCardDimensions().y, 2.f);
				}
			}
			glPopMatrix();

			//drawing hovered card on top of every other card
			if (index == m_CurrentCardNumber - 1)
			{
				if (Card::GetHoveredCard() != -1)
				{
					DrawDescription(m_pCards[Card::GetHoveredCard()]->GetCardType());
					glPushMatrix();
					{
						const float
							hoveredOffset{ 40.f };
						const Vector2f
							cardPos{ m_pCards[Card::GetHoveredCard()]->GetCardPosition() };

						glTranslatef(Card::GetCardDimensions().x * 0.5f, Card::GetCardDimensions().y * 0.5f, 0.f);
						glTranslatef(m_DrawPos.x + cardPos.x, m_DrawPos.y + cardPos.y + hoveredOffset, 0.f);
						glScalef(1.5f, 1.5f, 1.f);
						glTranslatef(-Card::GetCardDimensions().x * 0.5f, -Card::GetCardDimensions().y * 0.5f, 0.f);

						//////SHADDDOWW
						utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.4f });
						utils::FillRect(Vector2f{ 0.12f * Card::GetCardDimensions().x, -0.12f * Card::GetCardDimensions().y }, Card::GetCardDimensions().x, Card::GetCardDimensions().y);
						//////////

						utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
						utils::FillRect(Vector2f{ 0.f, 0.f }, Card::GetCardDimensions().x, Card::GetCardDimensions().y);

						utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
						utils::DrawRect(Vector2f{ 0.f, 0.f }, Card::GetCardDimensions().x, Card::GetCardDimensions().y, 2.f);
					}
					glPopMatrix();
				}
			}
		}
	}
}

void CardManager::CardHoveringHandle(const Vector2f& mousePosition) const
{
	for (int index{0}; index < m_MaximumCardNumber; ++index)
	{
		if (m_pCards[index] != nullptr)
		{
			if (IsPointInCard(mousePosition, m_pCards[index]->GetCardPosition(), m_pCards[index]->GetRotationAngle()))
			{
				Card::SetHoveredCard(index);
				return;
			}
		}
		else
		{
			continue;
		}
	}

	Card::SetHoveredCard(-1);
}


///TODO rethink architecture for player -> cardmanager -> card because i am not sure is it good that i have it here 
//i can use this but i am not sure if it is a good decision
void CardManager::UseHoveredCard(Player* player)
{
	if (Card::GetHoveredCard() != -1)
	{
		m_CurrentCardNumber--;
		ApplyCardOnPlayer(player);
		RecalculateCardPosition(Card::GetHoveredCard());
		Card::SetHoveredCard(-1);
	}
}

void CardManager::RecalculateCardPosition(int deletionIndex)
{
	RecalculateCardIndex(deletionIndex);

	const Vector2f posStep{
		30.f,
		3.f
	};
	const float angleStep{ -10.f };

	Vector2f pos{};
	float rotationAngle{ 30.f };
	int decreaseAfter{ m_CurrentCardNumber / 2 };

	for (int index{ 0 }; index < m_CurrentCardNumber; ++index)
	{
		m_pCards[index]->SetPosition(pos);
		m_pCards[index]->SetRotationAngle(rotationAngle);

		if (index < decreaseAfter)
		{
			pos.y += posStep.y;
		}
		else
		{
			pos.y -= posStep.y;
		}
		pos.x += posStep.x;
		rotationAngle += angleStep;
	}
}

void CardManager::RecalculateCardIndex(int deletionIndex)
{
	delete m_pCards[deletionIndex];
	m_pCards[deletionIndex] = nullptr;

	for (int index{ m_MaximumCardNumber - 1 }; index > deletionIndex; --index)
	{
		if (m_pCards[index] != nullptr)
		{
			m_pCards[deletionIndex] = m_pCards[index];
			m_pCards[index] = nullptr;
			return;
		}
	}
}

void CardManager::DrawDescription(Card::CardType type) const
{
	const float
		bottomOffset{ 30.f };

	Vector2f pos{ m_DrawPos.x, m_DrawPos.y - bottomOffset };
	Card::OutputCardDescription( pos, type);
}

void CardManager::ApplyCardOnPlayer(Player* player)
{
	switch (m_pCards[Card::GetHoveredCard()]->GetCardType())
	{
	case Card::CardType::railgunCard:
		player->SetPlayerState(Player::PlayState::railgunAim);
		break;

	case Card::CardType::trapCard:
		player->SetPlayerState(Player::PlayState::trapPlacement);
		break;

	case Card::CardType::hookCard:
		player->SetPlayerState(Player::PlayState::hookPickDirection);
		break;
	}
}


//TODO having hovered card index as static can be a problem because it will mess up a separate player decks of cards
//SAT algo, something with rotation matrix, it works so idgaf
//TODO maybe look into it later
bool CardManager::IsPointInCard(const Vector2f& mousePos, const Vector2f& cardPos, float angle) const
{
	float 
		dx{ mousePos.x - (m_DrawPos.x + cardPos.x) },
		dy{ mousePos.y - (m_DrawPos.y + cardPos.y) };

	float 
		rad{ -angle * (static_cast<float>(M_PI) / 180.f) };

	float 
		localX{ dx * cosf(rad) - dy * sinf(rad) },
		localY{ dx * sinf(rad) + dy * cosf(rad) };

	float 
		halfW{ Card::GetCardDimensions().x * 0.5f }, 
		halfH{ Card::GetCardDimensions().y * 0.5f };

	return (localX >= -halfW && localX <= halfW &&
		localY >= -halfH && localY <= halfH);
}
////////////////////////////////////////////////////////////////////////