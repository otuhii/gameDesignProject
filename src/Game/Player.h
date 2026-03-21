#pragma once

class Card;

class Player final
{
public:
	Player(int posX, int posY);

	~Player();

	const Card* GetPickedCard() const;
private:
	static const int
		m_MaximumCardNumber{ 6 };

	int 
		m_CurrentMapNumber{ m_MaximumCardNumber };

	Vector2i m_MapPosition{};

	Card* m_pCards[m_MaximumCardNumber]{};
	Card* m_PickedCard{};
};

