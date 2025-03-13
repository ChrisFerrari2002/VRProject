/**
* @file move.h
* @brief Move class header file
*
* This file contains the definition of the Move class that represents a move made on the chessboard.
*
* @details The Move class stores information about a move, including the piece moved, the start and end positions, and whether the piece was captured.
*
* @date 2025
*
* @see Piece
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/
#pragma once

//////////////
// #INCLUDE //
//////////////
#include "piece.h"

/**
* @brief Move class
*
* The Move class represents a move made on the chessboard, storing the piece moved, the start and end positions, and whether the piece was captured.
*/
class Move {
public:
    /**
    * @brief Constructor
    *
    * Initializes a move with the given piece.
    *
    * @param movedPiece The piece that was moved.
    */
    Move(Piece* movedPiece);

    /**
    * @brief Get the moved piece
    *
    * @return The piece that was moved.
    */
    Piece* getMovedPiece() const;

    /**
    * @brief Get the start row
    *
    * @return The starting row of the move.
    */
    int getStartRow() const;

    /**
    * @brief Get the start column
    *
    * @return The starting column of the move.
    */
    int getStartCol() const;

    /**
    * @brief Get the end row
    *
    * @return The ending row of the move.
    */
    int getEndRow() const;

    /**
    * @brief Get the end column
    *
    * @return The ending column of the move.
    */
    int getEndCol() const;

    /**
    * @brief Get whether the piece was eliminated
    *
    * @return True if the piece was captured, false otherwise.
    */
    bool getIsEliminated() const;

    /**
    * @brief Get the start transform
    *
    * @return The starting transform matrix of the piece.
    */
    glm::mat4 getStartTransform() const;

    /**
    * @brief Get the end transform
    *
    * @return The ending transform matrix of the piece.
    */
    glm::mat4 getEndTransform() const;

    /**
    * @brief Set the end position
    *
    * Sets the ending position of the move.
    *
    * @param row The ending row of the move.
    * @param col The ending column of the move.
    */
    void setEndPosition(int row, int col);

    /**
    * @brief Set whether the piece was eliminated
    *
    * Sets whether the piece was captured.
    *
    * @param eliminated True if the piece was captured, false otherwise.
    */
    void setEliminated(bool eliminated);

private:
    Piece* movedPiece; /**< The piece that was moved */

    int startRow, startCol; /**< The starting row and column of the move */
    int endRow, endCol; /**< The ending row and column of the move */
    bool isEliminated; /**< Whether the piece was captured */

    glm::mat4 startTransform; /**< The starting transform matrix of the piece */
    glm::mat4 endTransform; /**< The ending transform matrix of the piece */
};