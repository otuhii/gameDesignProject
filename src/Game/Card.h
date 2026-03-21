#pragma once
class Card
{
public:
	enum class CardType {
		railgunCard, //deal damage in a straight line to any of the sides
		trapCard, //place a trap on an adjacent tile, first player who will step on it will receive some damage

		hookCard, //player pick direction and it pull player 4 tiles there
		absoluteMovement, //player who casts this card picks a direction and all players are moved 2 tiles in that direction
	};


	CardType GetCardType() const;

private:
	CardType m_Type;
};

