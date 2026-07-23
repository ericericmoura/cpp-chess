#include "Board.h"

#include <utility>
#include <algorithm>
#include <optional>
#include <iostream>
#include <functional>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Operators.h"
#include "BoardConfiguration.h"
#include "Team.h"
#include "PieceType.h"
#include "BoardGraphicsComponent.h"
#include "Constants.h"

chess::Board::Board(file_io::BoardConfiguration board_config) noexcept
	: board_config_{board_config}
	, graphics_    {board_config.texture_key_} 	
{
	board_graphics_.Initialize(board_config_);
}

unsigned int chess::Board::OnTurnChanged(std::function<void(Team)> observer)
{
	return turn_changed_.Subscribe(observer);
}

void chess::Board::RemoveOnTurnChanged(unsigned int id)
{
	turn_changed_.Remove(id);
}

unsigned int chess::Board::OnPromotionWidgetRequested(std::function<void(Team, sf::Vector2f)> observer)
{
	return promotion_widget_requested_.Subscribe(observer);
}

void chess::Board::RemoveOnPromotionWidgetRequested(unsigned int id)
{
	promotion_widget_requested_.Remove(id);
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
	if (promoting_)
	{
		return;
	}

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
		if (target_it->second.GetPieceType() == PieceType::Rook && it->second.GetPieceType() == PieceType::King)
		{
			TryCastling(it->first, target_it->first);
			return;
		}
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

	if (piece_type == PieceType::Pawn)
	{
		float promotion_coords_y = team_to_play_ == Team::White ? 0.f : 7.f;
		if (target_coords.y == promotion_coords_y)
		{
			promoting_ = true;

			promotion_widget_requested_.Notify(team_to_play_, GetPositionFromCoordinates(target_coords));
			return;
		}
	}

	FinishTurn();

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

	auto it = active_pieces_.find(starting_coords);
	if (it == active_pieces_.end())
	{
#ifdef _DEBUG
		std::cout << "\nERROR while finding selected piece!";
#endif // _DEBUG
		return false;
	}
	auto piece_type = it->second.GetPieceType();
	auto piece_team = it->second.GetTeam();

	SwapPieceCoordinates(starting_coords, target_coords);

	if (piece_type == PieceType::King)
	{
		UpdateKingCoordinates(team_to_play_, target_coords);
	}
	if (!IsKingInCheck(team_to_play_))
	{
		if (piece_type == PieceType::King)
		{
			if (piece_team == Team::White)
			{
				white_king_moved_ = true;
			}
			else
			{
				black_king_moved_ = true;
			}
		}

		it = active_pieces_.find(target_coords);
		it->second.Moved(*this, starting_coords);
		if (captured_piece.has_value())
		{
			AddInactivePiece(std::move(captured_piece.value()));
		}
		return true;
	}
	SwapPieceCoordinates(target_coords, starting_coords);

	if (captured_piece.has_value())
	{
		active_pieces_.try_emplace(target_coords, std::move(captured_piece.value()));
	}
	if (piece_type == PieceType::King)
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

	return IsCoordsAttacked(king_coords, team);
}

bool chess::Board::IsCoordsAttacked(const sf::Vector2u& target_coords, Team team) const noexcept
{
	for (auto& [coords, piece] : active_pieces_)
	{
		if (piece.GetTeam() == team || coords == target_coords)
		{
			continue;
		}
		if (piece.CanMoveTo(*this, target_coords))
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

	auto ending_it = active_pieces_.find(to);
	if (ending_it == active_pieces_.end())
	{
#ifdef _DEBUG
		std::cerr << "\nError while moving piece!";
#endif // _DEBUG
		return;
	}
	ending_it->second.SetCoordinates(*this, to);
}

void chess::Board::Promote(sf::Vector2u coords, PieceType target_piece) noexcept
{
	if (target_piece == PieceType::Pawn || target_piece == PieceType::King)
	{
#ifdef _DEBUG
		std::cout << "Invalid piece type!!";
#endif // _DEBUG
		return;
	}
	active_pieces_.erase(coords);
	auto piece = factory_.GeneratePiece(*this, coords, team_to_play_, target_piece);
	active_pieces_.try_emplace(coords, std::move(piece));

	FinishTurn();

	promoting_ = false;
}

void chess::Board::FinishTurn() noexcept
{
	team_to_play_ = team_to_play_ == Team::White ? Team::Black : Team::White;
	turn_changed_.Notify(team_to_play_);	
}

void chess::Board::TryCastling(const sf::Vector2u king_coords, const sf::Vector2u rook_coords) noexcept
{	
	auto king_it = active_pieces_.find(king_coords);
	auto rook_it = active_pieces_.find(rook_coords);

	auto team = king_it->second.GetTeam();
	
	if (   (team == Team::White && white_castled_)
		|| (team == Team::Black && black_castled_))
	{
		return;
	}

	if (   (team == Team::White && white_king_moved_)
		|| (team == Team::Black && black_king_moved_))
	{
		return;
	}

	// Check if movement is valid
	if ((king_it == active_pieces_.end() || king_it->second.GetPieceType() != PieceType::King) ||
		(rook_it == active_pieces_.end() || rook_it->second.GetPieceType() != PieceType::Rook) ||
		king_it->second.GetTeam() != rook_it->second.GetTeam())
	{
		return;
	}

	bool has_rook_moved = rook_coords.x != 0 && rook_coords.x != 7;

	if (has_rook_moved)
	{
		return;
	}

	// Initialize target variables
	auto king_target_x = king_coords.x;
	auto rook_target_x = rook_coords.x;

	auto start_x = king_coords.x + 1;
	auto end_x   = rook_coords.x - 1;

	if (king_coords.x > rook_coords.x)
	{
		start_x = rook_coords.x + 1;
		end_x   = king_coords.x - 1;

		king_target_x = king_coords.x - 2;
		rook_target_x = king_target_x + 1;
	}
	else
	{
		king_target_x = king_coords.x + 2;
		rook_target_x = king_target_x - 1;
	}

	// Check if nothing's on the way
	for (auto x = start_x; x < end_x; ++x)
	{
		if (active_pieces_.contains({ x, king_coords.y }))
		{
			return;
		}
	}

	// Move pieces
	sf::Vector2u target_king_coords = { king_target_x, king_coords.y };
	sf::Vector2u target_rook_coords = { rook_target_x, king_coords.y };

	SwapPieceCoordinates(king_coords, target_king_coords);
	SwapPieceCoordinates(rook_coords, target_rook_coords);

	UpdateKingCoordinates(team, target_king_coords);

	if (IsKingInCheck(team) || IsCoordsAttacked(target_rook_coords, team))
	{	
		SwapPieceCoordinates(target_king_coords, king_coords);
		SwapPieceCoordinates(target_rook_coords, rook_coords);

		UpdateKingCoordinates(team, king_coords);

		return;
	}
	if (team == Team::White)
	{
		white_castled_ = true;
	}
	else
	{
		black_castled_ = true;
	}
	FinishTurn();
}

void chess::Board::Update(float delta) noexcept
{
	board_graphics_.Update(*this);
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