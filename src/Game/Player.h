#pragma once

class CardManager;
class Texture;

class Player final
{
public:
	enum class PlayState {
		none = -1,

		railgunAim,
		trapPlacement,
		
		hookPickDirection,
		absoluteMovementPickDirection
	};

	Player(int posX, int posY, const std::string texturePath);

	~Player();

	void Draw(const Rectf& mapSize) const;
	void DrawCards() const;

	void		 SetPlayerState(PlayState state);
	PlayState	 GetPlayerState() const;

	void		 SetPlayerPosition(const Vector2i& position);
	Vector2i	 GetPlayerPosition() const;

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

	PlayState m_PlayerState{PlayState::none};
};

