#include "pch.h"
#include "Map.h"
#include "utils.h"

Map::Map()
{
	for (int row{ 0 }; row < m_MapDimension; ++row)
	{
		for (int column{ 0 }; column < m_MapDimension; ++column)
		{
			Cell::CellState state{ Cell::CellState::empty };

			if (rand() % 10 == 1) //TODO: change this randomization to something better because some non-sense form pop out
			{
				state = Cell::CellState::wallPlaced;
			}
			m_pMap[row][column] = new Cell{state};
		}
	}
}

Map::~Map()
{
	for (int row{ 0 }; row < m_MapDimension; ++row)
	{
		for (int column{ 0 }; column < m_MapDimension; ++column)
		{
			delete m_pMap[row][column];
		}
	}
}

void Map::Draw(const Rectf& viewport) const
{
	const float
		cellWidth{ viewport.width / m_MapDimension },
		cellHeight{ viewport.height / m_MapDimension },

		squizeBy{ 5.f };

	Color4f
		gray{ 0.3f, 0.3f, 0.3f, 1.f },
		blue{ 0.f, 0.f, 1.f, 1.f };

	for (int row{ 0 }; row < m_MapDimension; ++row)
	{
		for (int column{ 0 }; column < m_MapDimension; ++column)
		{
			if (m_pMap[row][column]->IsWallPlaced())
			{
				utils::SetColor(blue);
			}
			else 
			{
				utils::SetColor(gray);
			}

			utils::FillRect(Rectf{
				column*cellWidth+squizeBy,
				row * cellHeight+squizeBy,
				cellWidth-squizeBy,
				cellHeight-squizeBy
				});
		}
	}

}

void Map::PlaceTrap()
{

}

bool Map::IsBombPlacedOnCell(int index) const
{
	const int
		row{ RowFromIndex(index) },
		column{ ColumnFromIndex(index) };

	return m_pMap[row][column]->IsTrapPlaced();
	
}

bool Map::IsWallPlacedOnCell(int index) const
{
	const int
		row{ RowFromIndex(index) },
		column{ ColumnFromIndex(index) };

	return m_pMap[row][column]->IsWallPlaced();
}

int Map::RowFromIndex(int index) const
{
	return index/m_MapDimension;
}

int Map::ColumnFromIndex(int index) const
{
	return index % m_MapDimension;
}




#pragma region CELL_DEFINITIONS

Map::Cell::Cell(CellState state)
	:m_State(state)
{
}

bool Map::Cell::IsTrapPlaced() const
{
	return (m_State == Cell::CellState::trapPlaced);
}

bool Map::Cell::IsWallPlaced() const
{
	return (m_State == Cell::CellState::wallPlaced);
}


#pragma endregion CELL_DEFINITIONS
