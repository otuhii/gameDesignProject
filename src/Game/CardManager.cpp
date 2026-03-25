#include "pch.h"
#include "CardManager.h"
#include "utils.h"
#include "Player.h"

CardManager::CardManager(const Vector2f& drawPos)
	:m_DrawPos(drawPos)
{
	const Vector2f posStep{
		40.f,
		10.f
	};
	const float angleStep{ -10.f };

	Vector2f pos{};
	float rotationAngle{ 30.f };

	int decreaseAfter{ m_CurrentCardNumber / 2 };

	for (int index = 0; index < m_MaximumCardNumber; ++index)
	{
		Card::CardType type{ static_cast<Card::CardType>(rand() % 3) };

		m_pCards[index] = new Card{ m_DrawPos+pos, rotationAngle, type};

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

					//glTranslatef(Card::GetCardDimensions().x * 0.5f, Card::GetCardDimensions().y * 0.5f, 0.f);
					glTranslatef( cardPos.x, cardPos.y, 0.f);
					glRotatef(m_pCards[index]->GetRotationAngle(), 0.f, 0.f, 1.f);
					//glTranslatef(-Card::GetCardDimensions().x * 0.5f, -Card::GetCardDimensions().y * 0.5f, 0.f);

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

						//glTranslatef(Card::GetCardDimensions().x * 0.5f, Card::GetCardDimensions().y * 0.5f, 0.f);
						glTranslatef(cardPos.x, cardPos.y + hoveredOffset, 0.f);
						glScalef(1.5f, 1.5f, 1.f);
						//glTranslatef(-Card::GetCardDimensions().x * 0.5f, -Card::GetCardDimensions().y * 0.5f, 0.f);

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
			if (IsPointInCard(mousePosition, m_pCards[index]))
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
		40.f,
		10.f
	};
	const float angleStep{ -10.f };

	Vector2f pos{};
	float rotationAngle{ 30.f };
	int decreaseAfter{ m_CurrentCardNumber / 2 };

	for (int index{ 0 }; index < m_CurrentCardNumber; ++index)
	{
		m_pCards[index]->SetPosition(m_DrawPos+pos);
		m_pCards[index]->SetRotationAngle(rotationAngle);
		m_pCards[index]->SetBounds();

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

bool CardManager::IsPointInCard(const Vector2f& mousePos, const Card* m_pCards) const
{
	return utils::IsPointInPolygon(mousePos, m_pCards->GetCardBounds());
}
