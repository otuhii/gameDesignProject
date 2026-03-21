#include "pch.h"
#include "CardManager.h"
#include "Card.h"
#include "utils.h"
#include <iostream>


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

	for (int index = 0; index < m_MaximumCardNumber; ++index)
	{
		m_pCards[index] = new Card{pos, rotationAngle};
		pos += posStep;
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
			Vector2f cardPos{ m_pCards[index]->GetCardPosition() };

			glTranslatef(m_DrawPos.x+cardPos.x, m_DrawPos.y+cardPos.y, 0.f);
			glRotatef(m_pCards[index]->GetRotationAngle(), 0.f, 0.f, 1.f);
			glTranslatef(-Card::GetCardDimensions().x * 0.5f, -Card::GetCardDimensions().y * 0.5f, 0.f);

			utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
			utils::FillRect(cardPos, Card::GetCardDimensions().x, Card::GetCardDimensions().y);

			utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
			utils::DrawRect(cardPos, Card::GetCardDimensions().x, Card::GetCardDimensions().y, 2.f);

		}
		glPopMatrix();
	}
}


void CardManager::RecalculateCardPosition()
{

}

