#include "pch.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "utils.h"

#include <iostream>

/*
	QUESTIONS TO ANSWER FOR FIRST PLAYTEST:
	do players think that seeing each others cards will be a problem for gameplay?

	do players think that seeing where other player put a bomb will be a problem for gameplay?

*/


//TODO Timer for each player's turn
//TODO Change mechanic of trap placement because it is hard to implement in current enviroment without mulptiple screen(multiplayer) gameplay, every player can see the trap
//TODO animation for player movement


//VISUALS AND NOT NECCESSARY STUFF:
//TODO vhs like effect
//TODO maybe maybe maybe multiplayer but i need to remind myself about sockets and stuff
//TODO for wall generation i can just have some presets for wall structures and then just put in on the map 

const float Game::m_MapDimensions{ 720.f };

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	const Rectf viewport{ GetViewPort() };

	const Vector2f pos{
		(viewport.width-m_MapDimensions)*0.5f,
		(viewport.height-m_MapDimensions)*0.5f
	};

	m_pMap = new Map{ Rectf{pos.x, pos.y*2, m_MapDimensions, m_MapDimensions} };

	m_Players[0] = new Player{5, 12, "firstPlayerSprite.png"};
	m_Players[1] = new Player{5, 10, "secondPlayerSprite.png"};
}

void Game::Cleanup( )
{
	delete m_pMap;
	delete m_Players[0];
	delete m_Players[1];
}

void Game::Update( float elapsedSec )
{
	std::cout << 1 / elapsedSec << "FPS" << std::endl;

	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_pMap->Draw(false);

	m_Players[0]->Draw(m_pMap->GetMapBounds());
	m_Players[1]->Draw(m_pMap->GetMapBounds());
	m_Players[0]->DrawCards();
	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
	
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_pMap->ResetHighlight();
	if (utils::IsPointInRect(Vector2f{ static_cast<float>(e.x), static_cast<float>(e.y) }, m_pMap->GetMapBounds()))
	{
		m_pMap->ProcessMapHovering(m_Players[0],m_Players[1], Vector2f{ static_cast<float>(e.x), static_cast<float>(e.y) });
	}
	m_Players[0]->HoverCards(Vector2f{ static_cast<float>(e.x), static_cast<float>(e.y) });
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		if (utils::IsPointInRect(Vector2f{static_cast<float>(e.x), static_cast<float>(e.y)}, m_pMap->GetMapBounds()))
		{
			m_pMap->ProcessMapClick(m_Players[0], m_Players[1], Vector2f{static_cast<float>(e.x), static_cast<float>(e.y)});
		}
		m_Players[0]->ProcessHoveredCardClick();
		break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
