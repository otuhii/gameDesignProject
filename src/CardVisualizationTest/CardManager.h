#pragma once

class Card;

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

	bool IsPointInCard(const Vector2f& mousePos, const Vector2f& cardPos, float angle) const;
};

