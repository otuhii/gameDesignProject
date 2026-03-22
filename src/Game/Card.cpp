#include "pch.h"
#include "Card.h"
#include "Texture.h"
#include <iostream>

const Vector2f Card::m_CardDimensions{ 60.f, 100.f };
Texture* Card::m_CardDescriptions[static_cast<int>(CardType::typeCount)];

int Card::m_HoveredCard{ -1 };
int Card::m_CardCount{0};

Card::Card(const Vector2f& cardPos, float angle, CardType type)
    : m_Position{ cardPos }, m_RotationAngle(angle), m_Type{type}
{
    if (m_CardCount == 0)
    {
        InitializeDescriptions();
    }

    m_CardCount++;
}

Card::~Card()
{
    m_CardCount--;
    if (m_CardCount <= 0)
    {
        for (Texture* pTexture : m_CardDescriptions)
        {
            delete pTexture;
        }
    }
}
#pragma region setters
void Card::SetPosition(const Vector2f& newPos)
{
    m_Position = newPos;
}

void Card::SetRotationAngle(float angle)
{
    m_RotationAngle = angle;
}

#pragma endregion setters

#pragma region getters
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
#pragma endregion getters

#pragma region statics 
void Card::SetHoveredCard(int index)
{
    m_HoveredCard = index;
}

const int Card::GetHoveredCard()
{
    return m_HoveredCard;
}

const Vector2f& Card::GetCardDimensions()
{
    return m_CardDimensions;
}

#pragma endregion statics

#pragma region descriptionHandling
void Card::OutputCardDescription(const Vector2f& pos, CardType type)
{
    Vector2f drawPos{ 0, pos.y };

    m_CardDescriptions[static_cast<int>(type)]->Draw(drawPos);
}

void Card::InitializeDescriptions()
{
    Color4f textColor{ 0.f, 1.f, 0.f, 1.f };
    int textSize{ 15 };
    std::string fontPath{ "cornella-Regular.otf" };

    m_CardDescriptions[static_cast<int>(Card::CardType::railgunCard)] = new Texture{
        "Deal high damage to other player in a straight line from your position",fontPath, textSize, textColor
    };

    m_CardDescriptions[static_cast<int>(Card::CardType::trapCard)] = new Texture{
        "Place a hidden trap on an adjacent tile. The firt player to step on it takes massive damage and ends his turn",fontPath, textSize, textColor
    };

    m_CardDescriptions[static_cast<int>(Card::CardType::hookCard)] = new Texture{
        "Target a wall or player up to 4 tiles away and pull yourself there, in case there is neither player nor a wall, you move 1 cell in that direction",fontPath, textSize, textColor
    };

}

#pragma endregion descriptionHandling