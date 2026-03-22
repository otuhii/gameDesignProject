#include "pch.h"
#include "utils.h"

#include "Map.h"
#include "Card.h"
#include "UserUtils.h"

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

			Rectf cellRect{
				m_MapBounds.left + column * cellWidth + squizeBy,
				m_MapBounds.bottom + row * cellHeight + squizeBy,
				cellWidth - squizeBy,
				cellHeight - squizeBy
			};

			utils::FillRect(cellRect);
			utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
			if (m_pMap[row][column]->IsHighlighted())
			{
				utils::DrawRect(cellRect, 5);
			}
			
		}
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




void Map::ProcessMapClick(Player* player, Player* enemyPlayer, const Vector2f& clickPos)
{
	switch (player->GetPlayerState())
	{
	case Player::PlayState::railgunAim:
		HandleRailgunStateClick(player, clickPos);
		break;
	case Player::PlayState::trapPlacement:
		HandleTrapStateClick(player, clickPos);
		break;
	case Player::PlayState::hookPickDirection:
		HandleHookStateClick(player, enemyPlayer, clickPos);
		break;
	}
}

void Map::ProcessMapHovering(Player* player, Player* enemyPlayer, const Vector2f& mousePos)
{
	switch (player->GetPlayerState())
	{
	case Player::PlayState::railgunAim:
		HandleRailgunStateHovering(player, mousePos);
		break;
	case Player::PlayState::trapPlacement:
		HandleTrapStateHovering(player, mousePos);
		break;
	case Player::PlayState::hookPickDirection:
		HandleHookStateHovering(player, enemyPlayer, mousePos);
		break;
	}
}

void Map::ResetHighlight()
{
	for (int row{ 0 }; row < m_MapDimension; ++row)
	{
		for (int column{ 0 }; column < m_MapDimension; ++column)
		{
			m_pMap[row][column]->Dehighlight();
		}
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


#pragma region playerStateCursorHandling


void Map::HandleRailgunStateClick(Player* player, const Vector2f& mousePos)
{
	player->SetPlayerState(Player::PlayState::none);
}

void Map::HandleTrapStateClick(Player* player, const Vector2f& mousePos)
{
	player->SetPlayerState(Player::PlayState::none);
}

void Map::HandleHookStateClick(Player* player, Player* enemyPlayer, const Vector2f& mousePos)
{
	player->SetPlayerState(Player::PlayState::none);
}

void Map::HandleRailgunStateHovering(Player* player, const Vector2f& mousePos)
{
	Vector2i
		playerPosition{ player->GetPlayerPosition() },
		localMousePosition{ Map::GlobalToLocalPosition(Map::GetMapBounds(), mousePos) };

	const int
		damageRange{ 4 };

	Vector2i
		stepVector{ 0, 0 };

	int
		dx{ localMousePosition.x - playerPosition.x },
		dy{ localMousePosition.y - playerPosition.y };

	if (dx == 0 && dy == 0)
	{
		//mouse on player tile handling
		return;
	}

	if (std::abs(dx) > std::abs(dy))
	{
		stepVector.x = dx / std::abs(dx);
	}
	else
	{
		stepVector.y = dy / std::abs(dy);
	}

	for (int index{ 1 }; index <= damageRange; ++index)
	{
		Vector2i targetTile{
			playerPosition.x + index * stepVector.x,
			playerPosition.y + index * stepVector.y
		};
		if (targetTile.x < m_MapDimension && targetTile.y < m_MapDimension &&
			targetTile.x >= 0 && targetTile.y >= 0)
		{
			if (!m_pMap[targetTile.y][targetTile.x]->IsWallPlaced())
			{
				m_pMap[targetTile.y][targetTile.x]->Highlight();
			}
			else
			{
				return;
			}
		}
		else
		{
			return; // break after hitting edge of the map
		}
	}
}

void Map::HandleTrapStateHovering(Player* player, const Vector2f& mousePos)
{
	Vector2i
		localMousePos{ Map::GlobalToLocalPosition(m_MapBounds, mousePos) },
		playerPosition{ player->GetPlayerPosition() };

	int dx{ localMousePos.x - playerPosition.x };
	int dy{ localMousePos.y - playerPosition.y };

	const int 
		trapPlacementRange{ 2 };

	Vector2i targetTile{
		playerPosition.x + UserUtils::myClamp(dx, -trapPlacementRange, trapPlacementRange),
		playerPosition.y + UserUtils::myClamp(dy, -trapPlacementRange, trapPlacementRange)
	};

	if (targetTile.x >= 0 && targetTile.x < m_MapDimension &&
		targetTile.y >= 0 && targetTile.y < m_MapDimension)
	{
		if (m_pMap[targetTile.y][targetTile.x] != nullptr)
		{
			if (!m_pMap[targetTile.y][targetTile.x]->IsWallPlaced())
			{
				m_pMap[targetTile.y][targetTile.x]->Highlight();
			}
		}
	}
}

void Map::HandleHookStateHovering(Player* player, Player* enemyPlayer, const Vector2f& mousePos)
{
	Vector2i
		playerPosition{ player->GetPlayerPosition() },
		enemyPlayerPosition{ enemyPlayer->GetPlayerPosition() },
		localMousePosition{ Map::GlobalToLocalPosition(Map::GetMapBounds(), mousePos) };

	const int
		maxHookRange{ 4 };

	int
		finalHookRange{ 1 };

	Vector2i
		stepVector{ 0, 0 };

	int
		dx{ localMousePosition.x - playerPosition.x },
		dy{ localMousePosition.y - playerPosition.y };

	if (dx == 0 && dy == 0)
	{
		//mouse on player tile handling
		return;
	}
	if (std::abs(dx) > std::abs(dy))
	{
		stepVector.x = dx / std::abs(dx);
	}
	else
	{
		stepVector.y = dy / std::abs(dy);
	}

	for (int index{ 1 }; index <= maxHookRange+1; ++index)
	{
		Vector2i targetTile{
			playerPosition.x + index * stepVector.x,
			playerPosition.y + index * stepVector.y
		};
		if (targetTile.x >= m_MapDimension || targetTile.y >= m_MapDimension ||
			targetTile.x < 0 || targetTile.y < 0)
		{
			break;
		}
		bool isEnemy{
			(targetTile.x == enemyPlayerPosition.x) && 
			(targetTile.y == enemyPlayerPosition.y)
		};

		if (isEnemy || m_pMap[targetTile.y][targetTile.x]->IsWallPlaced())
		{
			finalHookRange = index-1;
			break;
		}

	}

	if (finalHookRange > 0)
	{
		for (int index{ 1 }; index <= finalHookRange; ++index)
		{
			Vector2i targetTile{
			playerPosition.x + index * stepVector.x,
			playerPosition.y + index * stepVector.y
			};

			m_pMap[targetTile.y][targetTile.x]->Highlight();
		}
	}
}

#pragma endregion playerStateCursorHandling





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

bool Map::Cell::IsHighlighted() const
{
	return m_Highlighted;
}

void Map::Cell::PlaceTrap()
{
	m_State = CellState::trapPlaced;
}

void Map::Cell::PlaceWall()
{
	m_State = CellState::wallPlaced;
}

void Map::Cell::Dehighlight()
{
	m_Highlighted = false;
}

void Map::Cell::Highlight()
{
	m_Highlighted = true;
}

#pragma endregion CELL_DEFINITIONS

