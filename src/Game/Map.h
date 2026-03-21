#pragma once
#include "Player.h"

class Map final
{
public:
	explicit Map(const Rectf& mapBounds);

	~Map();

	void Draw(bool debug) const;

	bool IsTrapPlacedOnCell(int index) const;
	bool IsWallPlacedOnCell(int index) const;

	void ProcessMapClick(Player* player, const Vector2f& clickPos);
	
	Rectf GetMapBounds() const;

	static Vector2i GlobalToLocalPosition(const Rectf& mapSize, const Vector2f& globalPosition);
	static Vector2f LocalToGlobalPosition(const Rectf& mapSize, const Vector2i& mapPos);
	static int		GetDimensions();
	
private:
	class Cell {
	public:
		enum class CellState {
			empty,
			trapPlaced,
			wallPlaced
		};

		explicit Cell(CellState state);

		bool IsTrapPlaced() const;
		bool IsWallPlaced() const;
		
		void PlaceTrap();
		void PlaceWall();

	private:
		CellState m_State{ CellState::empty };
	};

	static const int m_MapDimension{15};

	Cell* m_pMap[m_MapDimension][m_MapDimension];

	const Rectf 
		m_MapBounds{};

	void PlaceTrap(const Vector2i& position);
	void PlaceWall(const Vector2i& position);

	int RowFromIndex(int index) const;
	int ColumnFromIndex(int index) const;
};

