#include "pch.h"
#include "Card.h"

const Vector2f Card::m_CardDimensions{ 60.f, 100.f };

Card::Card(const Vector2f& cardPos, float angle)
    : m_Position{ cardPos }, m_RotationAngle(angle), m_Type{}
{}

void Card::SetPosition(const Vector2f& newPos)
{
    m_Position = newPos;
}

void Card::SetRotationAngle(float angle)
{
    m_RotationAngle = angle;
}

Card::CardType Card::GetCardType() const
{
    return m_Type;
}

const Vector2f& Card::GetCardPosition() const
{
    return m_Position;
}

float Card::GetRotationAngle() const
{
    return m_RotationAngle;
}

Rectf Card::GetCardBounds() const
{
    return Rectf{
        m_Position.x, 
        m_Position.y,
        m_CardDimensions.x,
        m_CardDimensions.y
    };
}

const Vector2f& Card::GetCardDimensions()
{
    return m_CardDimensions;
}
