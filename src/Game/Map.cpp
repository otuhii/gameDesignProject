#include "pch.h"
#include "utils.h"

#include "Map.h"
#include "Card.h"

Map::Map(const Rectf& mapBounds)
	:m_MapBounds(mapBounds)
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

void Map::Draw(bool debug) const
{
	const float
		cellWidth{ m_MapBounds.width / m_MapDimension },
		cellHeight{ m_MapBounds.height / m_MapDimension },

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
				m_MapBounds.left+column*cellWidth+squizeBy,
				m_MapBounds.bottom+row * cellHeight+squizeBy,
				cellWidth-squizeBy,
				cellHeight-squizeBy
				});
		}
	}

	if (debug)
	{
		utils::DrawRect(m_MapBounds, 5.f);
	}

}

bool Map::IsTrapPlacedOnCell(int index) const
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

void Map::ProcessMapClick(Player* player, const Vector2f& clickPos)
{
	const Card*
		playersCard{ nullptr }; /////TODO CHANGE THIS because i remove function that return card
	//TODO fix card hovering after first deletion of card
	
	if (playersCard != nullptr)
	{
		if (playersCard->GetCardType() == Card::CardType::trapCard)
		{
			PlaceTrap(GlobalToLocalPosition(m_MapBounds, clickPos));
		}
	}
	else
	{
		PlaceWall(GlobalToLocalPosition(m_MapBounds, clickPos));
	}
}

Rectf Map::GetMapBounds() const
{
	return m_MapBounds;
}

Vector2f Map::LocalToGlobalPosition(const Rectf& mapSize, const Vector2i& mapPos)
{
	const float
		cellWidth{ mapSize.width / m_MapDimension },
		cellHeight{ mapSize.height / m_MapDimension };

	float
		x{ mapPos.x * cellWidth + mapSize.left },
		y{ mapPos.y * cellHeight + mapSize.bottom };

	return Vector2f{ x, y };
}

int Map::GetDimensions()
{
	return m_MapDimension;
}

Vector2i Map::GlobalToLocalPosition(const Rectf& mapSize, const Vector2f& globalPosition)
{
	const float
		cellWidth{ mapSize.width / m_MapDimension },
		cellHeight{ mapSize.height / m_MapDimension };

	const Vector2f globalPositionStartsFromZero{
		globalPosition.x - mapSize.left,
		globalPosition.y - mapSize.bottom
	};

	int
		col{ static_cast<int>(globalPositionStartsFromZero.x / cellWidth) },
		row{ static_cast<int>(globalPositionStartsFromZero.y / cellHeight) };

	//safety 
	col = std::max(0, std::min(col, m_MapDimension - 1));
	row = std::max(0, std::min(row, m_MapDimension - 1));

	return Vector2i{ col, row };
}

void Map::PlaceTrap(const Vector2i& position)
{
	m_pMap[position.y][position.x]->PlaceTrap();
}

void Map::PlaceWall(const Vector2i& position)
{
	m_pMap[position.y][position.x]->PlaceWall();
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

void Map::Cell::PlaceTrap()
{
	m_State = CellState::trapPlaced;
}

void Map::Cell::PlaceWall()
{
	m_State = CellState::wallPlaced;
}

#pragma endregion CELL_DEFINITIONS
