#pragma once
#include <vector>

class Texture;

class Card
{
public:
	enum class CardType {
		none = -1,
		railgunCard, //deal damage in a straight line to any of the sides
		trapCard, //place a trap on an adjacent tile, first player who will step on it will receive some damage

		hookCard, //player pick direction and it pull player 4 tiles there
		absoluteMovement, //player who casts this card picks a direction and all players are moved 2 tiles in that direction

		typeCount
	};
	Card(const Vector2f& cardPos, float angle);

	~Card();

	void SetPosition(const Vector2f& newPos);
	void SetRotationAngle(float angle);
	void SetBounds();

	CardType				     GetCardType()		const;
	const Vector2f&			     GetCardPosition()	const;
	float					     GetRotationAngle()	const;
	const std::vector<Vector2f>& GetCardBounds()    const;	

	static void				SetHoveredCard(int index);

	static const int		GetHoveredCard();
	static const Vector2f&	GetCardDimensions();
	static void				OutputCardDescription(const Vector2f& pos, CardType type);
private:
	static const Vector2f m_CardDimensions;
	static int m_HoveredCard;
	static int m_CardCount;

	static Texture* m_CardDescriptions[static_cast<int>(CardType::typeCount)];

	std::vector<Vector2f> m_RotatedBounds{ 4, Vector2f{} };

	CardType m_Type;

	Vector2f m_Position{};
	float m_RotationAngle{};

	void InitializeDescriptions();
};

