#include "../Headers/PieceState.hpp"

void PieceState::renderPiece(Renderer* renderer, std::unique_ptr<State>& state, PlayerColor* playerColor, PieceToDraw pieceToDraw, u16 boardXOffset, u16 boardYOffset, u8 ghostPieceOffset) {
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
bool PieceState::getPieceData(u8 x, u8 y, std::unique_ptr<Piece>& p, u8 rotation) {
	switch (rotation) {
	case 0:
		return (p->data[y * p->width + x]);
	case 1:
		return (p->data[(p->width - x - 1) * p->width + y]);
	case 2:
		return (p->data[(p->width - y - 1) * p->width + (p->width - x - 1)]);
	case 3:
		return (p->data[(p->width * x) + (p->width - y - 1)]);
	default:
		return 0;
	}
}