#pragma once
#include "Card.h"

class Player;

class CardManager
{
public:
	CardManager(const Vector2f& drawPos);

	~CardManager();

	void DrawCards() const;

	void CardHoveringHandle(const Vector2f& mousePosition) const;

	void UseHoveredCard(Player* player);

private:
	static const int m_MaximumCardNumber{ 6 };

	int m_CurrentCardNumber{ m_MaximumCardNumber };

	Vector2f cardPosition{};
	Vector2f m_DrawPos{};

	Card* m_pCards[m_MaximumCardNumber];

	void RecalculateCardPosition(int deletionIndex);
	void RecalculateCardIndex(int deletionIndex);

	void DrawDescription(Card::CardType type) const;

	void ApplyCardOnPlayer(Player* player);

	bool IsPointInCard(const Vector2f& mousePos, const Vector2f& cardPos, float angle) const;
};

