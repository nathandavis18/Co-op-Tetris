#include "../Headers/PieceState.hpp"

void PieceState::renderPiece(Renderer* const renderer, const std::unique_ptr<State>& state, PlayerColor* const playerColor, 
	const PieceToDraw pieceToDraw, const u16 boardXOffset, const u16 boardYOffset, const u8 ghostPieceOffset) 
{
	for (int y = 0; y < state->piece->width; ++y) {
		for (int x = 0; x < state->piece->width; ++x) {
			if (getPieceData(x, y, state->piece, state->rotation)) {
				switch (pieceToDraw) {
				case PieceToDraw::NormalPiece:
					renderer->drawPiece(x + state->xOffset + boardXOffset, y + state->yOffset + boardYOffset, playerColor->fillColor, sf::Color::White);
					break;
				case PieceToDraw::GhostPiece:
					renderer->drawPiece(x + state->xOffset + boardXOffset, y + state->yOffset + boardYOffset + ghostPieceOffset, playerColor->ghostFillColor, m_ghostOutlineColor);
					break;
				}
			}
		}
	}
}

/// <summary>
/// Gets the data at the current position, doing some math to turn a 1d vector into a 2d vector
/// </summary>
/// <param name="x">The x position of the piece</param>
/// <param name="y">The y position of the piece</param>
/// <param name="p">The piece</param>
/// <param name="rotation">The piece's rotation, or the rotation to check if checking for rotation validity</param>
/// <returns>Returns false if the data is 0, true if data is greater than 0</returns>
bool PieceState::getPieceData(const u8 x, const u8 y, const std::unique_ptr<Piece>& piece, const u8 rotation) {
	switch (rotation) {
	case 0:
		return (piece->data[y * piece->width + x]);
	case 1:
		return (piece->data[(piece->width - x - 1) * piece->width + y]);
	case 2:
		return (piece->data[(piece->width - y - 1) * piece->width + (piece->width - x - 1)]);
	case 3:
		return (piece->data[(piece->width * x) + (piece->width - y - 1)]);
	default:
		return 0;
	}
}