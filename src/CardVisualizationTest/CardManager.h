#pragma once
#include "Card.h"

class CardManager
{
public:
	CardManager(const Vector2f& drawPos);

	~CardManager();

	void DrawCards() const;

	void CardHoveringHandle(const Vector2f& mousePosition) const;

private:
	static const int m_MaximumCardNumber{ 6 };

	int m_CurrentCardNumber{ m_MaximumCardNumber };

	Vector2f cardPosition{};
	Vector2f m_DrawPos{};

	Card* m_pCards[m_MaximumCardNumber];

	void RecalculateCardPosition();
	void DrawDescription(Card::CardType type) const;

	bool IsPointInCard(const Vector2f& mousePos, const Card& card) const;
};

