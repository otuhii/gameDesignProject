#include "pch.h"
#include "CardManager.h"
#include "Card.h"
#include "utils.h"
#include <iostream>


CardManager::CardManager(const Vector2f& drawPos)
	:m_DrawPos(drawPos)
{
	const Vector2f posStep{
		50.f,
		10.f
	};
	const float angleStep{ -10.f };

	Vector2f pos{};
	float rotationAngle{ 30.f };

	int decreaseAfter{ m_CurrentCardNumber / 2 };

	for (int index = 0; index < m_MaximumCardNumber; ++index)
	{
		m_pCards[index] = new Card{pos, rotationAngle};

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
		if (index == m_MaximumCardNumber - 1)
		{
			if (Card::GetHoveredCard() != -1)
			{
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

void CardManager::CardHoveringHandle(const Vector2f& mousePosition) const
{
	for (int index{0}; index < m_MaximumCardNumber; ++index)
	{
		if (m_pCards[index] != nullptr)
		{
			if (IsPointInCard(mousePosition, m_pCards[index]->GetCardPosition(), m_pCards[index]->GetRotationAngle()))
			{
				Card::SetHoveredCard(index);
				break;
			}
		}
	}
}

void CardManager::RecalculateCardPosition()
{
}


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