#include "Board.h"

#include <utility>
#include <algorithm>
#include <optional>
#include <iostream>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Operators.h"
#include "BoardConfiguration.h"
#include "Team.h"
#include "PieceType.h"
#include "BoardGraphicsComponent.h"

chess::Board::Board(file_io::BoardConfiguration board_config) noexcept
	: board_config_{board_config}
	, graphics_    {board_config.texture_key_} 	
{
	board_graphics_.Initialize(board_config_);
}

void chess::Board::GeneratePieces() noexcept
{
	auto info = factory_.GeneratePieces(*this, active_pieces_);
	white_king_coords_ = info.white_king_coords_;
	black_king_coords_ = info.black_king_coords_;
}

void chess::Board::SelectCoordinates(sf::Vector2u coords) noexcept
{
	auto it = active_pieces_.find(coords);
	if (it == active_pieces_.end())
	{
		return;
	}
	selected_coordinates_ = coords;
	board_graphics_.UpdateSelectedCellPosition(*this, selected_coordinates_.value());
}

void chess::Board::MoveSelectedPieceToCoordinates(sf::Vector2u target_coords)
{	
	// Check if Piece is valid
	bool is_selected_coords_valid = selected_coordinates_.has_value() 
									&& target_coords != selected_coordinates_
									&& IsCoordinatesWithinBounds(target_coords);
	if (!is_selected_coords_valid)
	{
		return;
	}
	auto it = active_pieces_.find(selected_coordinates_.value());		
	
	selected_coordinates_ = {};
	board_graphics_.UpdateSelectedCellPosition(*this, {9, 9});

	auto piece_team = it->second.GetTeam();
	bool is_piece_valid = it != active_pieces_.end() && it->second.GetTeam() == team_to_play_;
	if (!is_piece_valid)
	{
		return;
	}

	auto target_it = active_pieces_.find(target_coords);
	if (target_it != active_pieces_.end() && target_it->second.GetTeam() == team_to_play_)
	{
		SelectCoordinates(target_coords);
		return;
	}
	if (!it->second.CanMoveTo(*this, target_coords))
	{
		return;
	}

	// Move and Capture
	auto piece_type = it->second.GetPieceType();
	bool moved = MoveIfValid(it->first, target_coords);
	if (!moved)
	{
		return;
	}

	team_to_play_ = team_to_play_ == Team::White ? Team::Black : Team::White;

	// Update Kings coordinates
	if (piece_type != PieceType::King)
	{
		return;
	}
	UpdateKingCoordinates(team_to_play_, target_coords);
}

bool chess::Board::MoveIfValid(sf::Vector2u starting_coords, sf::Vector2u target_coords)
{
	auto captured_piece = CaptureAtCoordinates(target_coords);

	SwapPieceCoordinates(starting_coords, target_coords);
	
	auto it = active_pieces_.find(target_coords);
	if (it == active_pieces_.end())
	{
#ifdef _DEBUG
		std::cout << "\nERROR while moving piece!";
#endif // _DEBUG
		return false;
	}
	it->second.SetCoordinates(*this, target_coords);

	if (it->second.GetPieceType() == PieceType::King)
	{
		UpdateKingCoordinates(team_to_play_, target_coords);
	}
	if (!IsKingInCheck(team_to_play_))
	{
		it->second.Moved(*this, starting_coords);
		if (captured_piece.has_value())
		{
			AddInactivePiece(std::move(captured_piece.value()));
		}
		return true;
	}
	SwapPieceCoordinates(target_coords, starting_coords);
	it = active_pieces_.find(starting_coords);
	if (it == active_pieces_.end())
	{
#ifdef _DEBUG
		std::cout << "\nERROR while moving piece!";
#endif // _DEBUG
		return false;
	}
	it->second.SetCoordinates(*this, starting_coords);

	if (captured_piece.has_value())
	{
		active_pieces_.try_emplace(target_coords, std::move(captured_piece.value()));
	}
	if (it->second.GetPieceType() == PieceType::King)
	{
		UpdateKingCoordinates(team_to_play_, starting_coords);
	}
	return false;
}

void chess::Board::AddInactivePiece(Piece&& piece)
{
	if (piece.GetTeam() == Team::White)
	{
		inactive_white_pieces_.emplace_back(std::move(piece));
	}
	else
	{
		inactive_black_pieces_.emplace_back(std::move(piece));
	}
	board_graphics_.UpdateCapturedPiecesPosition(*this, inactive_black_pieces_, inactive_white_pieces_);
}

std::optional<chess::Piece> chess::Board::CaptureAtCoordinates(sf::Vector2u coords) noexcept
{
	std::optional<Piece> captured_piece;
	if (active_pieces_.contains(coords))
	{
		captured_piece = std::move(active_pieces_.at(coords));
		active_pieces_.erase(coords);
	}	
	return captured_piece;
}

const chess::Piece* chess::Board::GetPieceAtCoordinates(sf::Vector2u coords) const noexcept
{
	auto it = active_pieces_.find(coords);
	if (it == active_pieces_.end())
	{
		return nullptr;
	}
	return &it->second;
}

bool chess::Board::IsCoordinatesOccupied(const sf::Vector2u& coords) const noexcept
{
	return active_pieces_.contains(coords);
}

void chess::Board::SetConfig(file_io::BoardConfiguration config) noexcept
{
	board_config_ = config;	
	graphics_.SetTextureKey(config.texture_key_);

	board_graphics_.Initialize(board_config_);
}

const chess::file_io::BoardConfiguration& chess::Board::GetConfig() const noexcept
{
	return board_config_;
}

sf::Vector2f chess::Board::GetPositionFromCoordinates(const sf::Vector2u& coords) const noexcept
{
	auto position = coords * board_config_.cell_size_ + board_config_.margin_;
	return sf::Vector2f(position);
}

sf::Vector2u chess::Board::GetCoordinatesFromPosition(const sf::Vector2f& position) const noexcept
{
	auto local = position - sf::Vector2f(board_config_.margin_);

	local.x = std::max(local.x, 0.f);
	local.y = std::max(local.y, 0.f);

	return (sf::Vector2u(local) / board_config_.cell_size_);
}

bool chess::Board::IsCoordinatesWithinBounds(const sf::Vector2u& coords) const noexcept
{	
	return coords >= sf::Vector2u({0, 0}) && coords < board_config_.board_size_;
}

bool chess::Board::IsKingInCheck(Team team) const noexcept
{
	auto king_coords = team == Team::Black ? black_king_coords_ : white_king_coords_;
	for (auto& [coords, piece] : active_pieces_)
	{
		if (piece.GetTeam() == team || coords == king_coords)
		{
			continue;
		}
		if (piece.CanMoveTo(*this, king_coords))
		{
			return true;
		}
	}
	return false;
}

void chess::Board::UpdateKingCoordinates(Team team, sf::Vector2u coords) noexcept
{
	if (team == Team::Black)
	{
		black_king_coords_ = std::move(coords);
		return;
	}
	white_king_coords_ = std::move(coords);
}

void chess::Board::SwapPieceCoordinates(const sf::Vector2u& from, const sf::Vector2u& to)
{
	auto it = active_pieces_.find(from);
	if (it == active_pieces_.end())
	{
		return;
	}
	auto node = active_pieces_.extract(it);
	node.key() = to;
	active_pieces_.erase (to);
	active_pieces_.insert(std::move(node));
}

void chess::Board::Update(float delta) noexcept
{
	board_graphics_.Update(*this);

	if (!selected_coordinates_.has_value())
	{
		return;
	}
	auto it = active_pieces_.find(selected_coordinates_.value());
	if (it != active_pieces_.end())
	{	
		std::string type_name{};
		switch (active_pieces_.at(selected_coordinates_.value()).GetPieceType())
		{
		case chess::PieceType::Bishop:
			type_name = "bishop";
			break;
		case chess::PieceType::Knight:
			type_name = "knight";
			break;
		case chess::PieceType::Queen:
			type_name = "queen";
			break;
		case chess::PieceType::Rook:
			type_name = "rook";
			break;
		case chess::PieceType::Pawn:
			type_name = "pawn";
			break;
		case chess::PieceType::King:
			type_name = "king";
			break;
		default:
			break;
		}


		std::cout << "\nSelected piece is a ´" << type_name << "´ at coordinates: (x: " << selected_coordinates_.value().x << "y: " << selected_coordinates_.value().y << ")";
	}
}

void chess::Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(graphics_, states);

	for (auto& [coords, piece] : active_pieces_)
	{
		target.draw(piece);
	}
	board_graphics_.DrawPieces(active_pieces_, inactive_black_pieces_, inactive_white_pieces_, target, states);
}