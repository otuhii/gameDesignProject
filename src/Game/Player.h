#pragma once

class CardManager;
class Texture;

class Player final
{
public:
	Player(int posX, int posY, const std::string texturePath);

	~Player();

	void Draw(const Rectf& mapSize) const;
	void DrawCards() const;

	void HoverCards(const Vector2f& m_MousePosition);

	void ProcessHoveredCardClick();
private:
	static const int
		m_MaximumCardNumber{ 6 };

	int 
		m_CurrentMapNumber{ m_MaximumCardNumber };

	Vector2i m_MapPosition{};
	Texture* m_pTexture{};
	CardManager* m_pCardManager{};

	
};

