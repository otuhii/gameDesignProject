#include "pch.h"
#include "CardManager.h"
#include "utils.h"

CardManager::CardManager(const Vector2f& drawPos)
	:m_DrawPos(drawPos)
{
	const Vector2f posStep{
		50.f,
		10.f
	};
	const float angleStep{ -10.f };

	Vector2f pos{ };
	float rotationAngle{ 30.f };

	int decreaseAfter{ m_CurrentCardNumber / 2 };

	for (int index = 0; index < m_MaximumCardNumber; ++index)
	{
		m_pCards[index] = new Card{m_DrawPos+pos, rotationAngle};

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
		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		utils::DrawPolygon(m_pCards[index]->GetCardBounds());

		glPushMatrix();
		{
			if (Card::GetHoveredCard() != index)
			{
				Vector2f cardPos{ m_pCards[index]->GetCardPosition() };

				//glTranslatef(Card::GetCardDimensions().x * 0.5f, Card::GetCardDimensions().y * 0.5f, 0.f);
				glTranslatef(cardPos.x,cardPos.y, 0.f);
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
		if (index == m_MaximumCardNumber - 1)
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
					glTranslatef(cardPos.x, cardPos.y + hoveredOffset, 0.f);
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
			if (IsPointInCard(mousePosition, *m_pCards[index]))
			{
				Card::SetHoveredCard(index);
				return;
			}
		}
	}

	Card::SetHoveredCard(-1);
}

void CardManager::RecalculateCardPosition()
{

}

void CardManager::DrawDescription(Card::CardType type) const
{
	const float
		bottomOffset{ 30.f };

	Vector2f pos{ m_DrawPos.x, m_DrawPos.y - bottomOffset };
	Card::OutputCardDescription( pos, type);
}


bool CardManager::IsPointInCard(const Vector2f& mousePos, const Card& card) const
{
	return utils::IsPointInPolygon(mousePos, card.GetCardBounds());
}
