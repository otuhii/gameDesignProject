#include "pch.h"
#include "Card.h"
#include "Texture.h"

#include "Matrix2x3.h"

#include <iostream>

const Vector2f Card::m_CardDimensions{ 60.f, 100.f };
Texture* Card::m_CardDescriptions[static_cast<int>(Card::CardType::typeCount)]{};

int Card::m_HoveredCard{ -1 };
int Card::m_CardCount{0};

Card::Card(const Vector2f& cardPos, float angle)
    : m_Position{ cardPos }, m_RotationAngle(angle), m_Type{CardType::railgunCard}
{
    if (m_CardCount == 0)
    {
        InitializeDescriptions();
        std::cout << "DESCRIPTIONS INITIALIZED" << std::endl;
    }

    SetBounds();

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

void Card::SetPosition(const Vector2f& newPos)
{
    m_Position = newPos;
}

void Card::SetRotationAngle(float angle)
{
    m_RotationAngle = angle;
}

void Card::SetBounds()
{
    std::vector<Vector2f> notRotatedBounds{
        Vector2f{ 0, 0 },
        Vector2f{ m_CardDimensions.x, 0},
        Vector2f{ m_CardDimensions.x, m_CardDimensions.y},
        Vector2f{0, m_CardDimensions.y} };

    Matrix2x3
        rotation{},
        translation{},
        transformation{};

    rotation.SetAsRotate(m_RotationAngle);
    translation.SetAsTranslate(m_Position);

    transformation = translation * rotation;

    m_RotatedBounds = transformation.Transform(notRotatedBounds);
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

const std::vector<Vector2f>& Card::GetCardBounds() const
{
    return m_RotatedBounds;
}

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

void Card::OutputCardDescription(const Vector2f& pos, CardType type)
{
    m_CardDescriptions[static_cast<int>(type)]->Draw(pos);
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
        "Target a wall or player up to 4 tiles away and pull yourself there",fontPath, textSize, textColor
    };

    m_CardDescriptions[static_cast<int>(Card::CardType::absoluteMovement)] = new Texture{
        "You choose movement direction and all players move 2 cells in that direction",fontPath, textSize, textColor
    };
}
