#pragma once

class Map final
{
public:
	explicit Map();

	~Map();

	void Draw(const Rectf& viewport) const;
		
	void PlaceTrap();

	bool IsBombPlacedOnCell(int index) const;
	bool IsWallPlacedOnCell(int index) const;


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

	private:
		CellState m_State{ CellState::empty };
	};

	static const int m_MapDimension{15};

	Cell* m_pMap[m_MapDimension][m_MapDimension];


	int RowFromIndex(int index) const;
	int ColumnFromIndex(int index) const;
};

