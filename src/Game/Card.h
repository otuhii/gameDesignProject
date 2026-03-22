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

		hookCard, //player pick direction and if there is a wall or other player, he can hooks himself, otherwise he just moves 1 cell in that direction

		typeCount
	};
	Card(const Vector2f& cardPos, float angle, CardType type);

	~Card();

	void SetPosition(const Vector2f& newPos);
	void SetRotationAngle(float angle);
	

	CardType				GetCardType()		const;
	const Vector2f&			GetCardPosition()	const;
	float					GetRotationAngle()	const;
	Rectf					GetCardBounds()		const;	

	static void				SetHoveredCard(int index);

	static const int		GetHoveredCard();
	static const Vector2f&	GetCardDimensions();
	static void				OutputCardDescription(const Vector2f& pos, CardType type);
private:
	static const Vector2f m_CardDimensions;
	static int m_HoveredCard;
	static int m_CardCount;

	static Texture* m_CardDescriptions[static_cast<int>(CardType::typeCount)];

	CardType m_Type;

	Vector2f m_Position{};
	float m_RotationAngle{};

	void InitializeDescriptions();
};

