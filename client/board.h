/**
* @file board.h
* @brief Board class header file
*
* This file contains the definition of the Board class that represents the chessboard.
*
* @details The Board class manages the state of the chessboard, including the pieces on the board,
* the cemetery for captured pieces, and the history of moves for undo/redo functionality.
*
* @date 2025
*
* @see Piece, Cemetery, Move
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
#include <vector>
#include <list>
#include "piece.h"
#include "cemetery.h"
#include "move.h"

/**
* @brief Board class
*
* The chessboard is a 8x8 matrix of chess pieces that includes also a cemetery for the captured pieces.
*
*/
class Board {
    // Public methods
public:
    /**
    * @brief Constructor
    *
    * Initializes an empty chessboard and sets up the cemetery.
    */
    Board();

    /**
    * @brief Fill the chessboard with the pieces
    *
    * @param nodes A list of nodes representing the pieces to be placed on the board.
    */
    void fillChessboard(const std::list<Eng::Node*>& nodes);

    /**
    * @brief Set the selected piece
    *
    * Sets the piece that the player wants to move.
    *
    * @param piece The piece to be selected.
    */
    void setSelectedPiece(Eng::Node* piece);

    /**
    * @brief Get the selected piece
    *
    * @return The currently selected piece.
    */
    Piece* getSelectedPiece() const;

    /**
    * @brief Print the entire chessboard in the current state
    *
    * Prints the current state of the chessboard to the console.
    */
    void printChessboard();

    /**
    * @brief Reset the board to the initial state
    *
    * Resets the chessboard to its initial state, with all pieces in their starting positions.
	* Resets also the cemetery and the list of moves.
    */
    void resetBoard();

    /**
    * @brief Save the current position of the selected piece
    *
    * Confirms the movement of the selected piece and updates its position on the board.
    */
    void confirmMovement();

    /**
    * @brief Add the move to the list of moves
    *
    * Adds a move to the list of moves for undo/redo functionality.
    *
    * @param move The move to be added.
    */
    void addMove(Move* move);

    /**
    * @brief Undo the last move
    *
    * Reverts the last move made on the chessboard.
    */
    void undo();

    /**
    * @brief Redo the undone move
    *
    * Reapplies the last move that was undone.
    */
    void redo();

    /**
    * @brief Capture a piece sending it to the cemetery
    *
    * Moves a captured piece to the cemetery.
    *
    * @param capturedPiece The piece to be captured.
    */
    void capturePiece(Piece* capturedPiece);

// Private methods and fields
private:
    Cemetery* cemetery = nullptr; /**< Pointer to the cemetery for captured pieces */
    Piece* selectedPiece = nullptr; /**< Pointer to the currently selected piece */
    std::vector<std::vector<Piece*>> table; /**< matrix representing the chessboard */
    std::vector<Piece*> pieces; /**< List of all pieces on the board */
    std::vector<Move*> moves; /**< List of all moves madem rappresent the log/history of the game*/
    int currentMove = -1; /**< Index of the current move in the moves list */

    /**
    * @brief Get current the position of a node
    *
    * @param nodeName The name of the node.
    * @return A pair representing the row and column of the node.
    */
    std::pair<int, int> getPosition(const std::string& nodeName);
};

