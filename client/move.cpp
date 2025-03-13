/**
* @file move.cpp
* @brief Implementation of the Move class
*
* This file contains the implementation of the Move class methods.
*
* @see Move
* @see move.h
*
* @date 2025
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#include "move.h"

/**
* @brief Constructor
*
* Initializes a move with the given piece.
*
* @param movedPiece The piece that was moved.
*/
Move::Move(Piece* movedPiece) : movedPiece(movedPiece) {
    startRow = movedPiece->getRow();
    startCol = movedPiece->getCol();
    endRow = movedPiece->getTempRow();
    endCol = movedPiece->getTempCol();
    isEliminated = movedPiece->isEliminated();
    startTransform = movedPiece->getPieceTransform();
    endTransform = movedPiece->getNode()->getTransform();
}

/**
* @brief Get the moved piece
*
* @return The piece that was moved.
*/
Piece* Move::getMovedPiece() const {
    return movedPiece;
}

/**
* @brief Get the start row
*
* @return The starting row of the move.
*/
int Move::getStartRow() const {
    return startRow;
}

/**
* @brief Get the start column
*
* @return The starting column of the move.
*/
int Move::getStartCol() const {
    return startCol;
}

/**
* @brief Get the end row
*
* @return The ending row of the move.
*/
int Move::getEndRow() const {
    return endRow;
}

/**
* @brief Get the end column
*
* @return The ending column of the move.
*/
int Move::getEndCol() const {
    return endCol;
}

/**
* @brief Set the end position
*
* Sets the ending position of the move.
*
* @param row The ending row of the move.
* @param col The ending column of the move.
*/
void Move::setEndPosition(int row, int col) {
    endRow = row;
    endCol = col;
}

/**
* @brief Get whether the piece was eliminated
*
* @return True if the piece was captured, false otherwise.
*/
bool Move::getIsEliminated() const {
    return isEliminated;
}

/**
* @brief Set whether the piece was eliminated
*
* Sets whether the piece was captured.
*
* @param eliminated True if the piece was captured, false otherwise.
*/
void Move::setEliminated(bool eliminated) {
    isEliminated = eliminated;
}

/**
* @brief Get the start transform
*
* @return The starting transform matrix of the piece.
*/
glm::mat4 Move::getStartTransform() const {
    return startTransform;
}

/**
* @brief Get the end transform
*
* @return The ending transform matrix of the piece.
*/
glm::mat4 Move::getEndTransform() const {
    return endTransform;
}