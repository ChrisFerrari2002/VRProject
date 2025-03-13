/**
* @file piece.h
* @brief Piece class header file
*
* This file contains the definition of the Piece class that represents a chess piece.
*
* @details The Piece class manages the state and behavior of a chess piece, including its position, color, and transformations.
*
* @date 2025
*
* @see Eng::Node
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
#include "engine.h"

/**
* @brief Piece class
*
* The Piece class represents a chess piece, managing its state and behavior.
*/
class Piece {
public:
    /**
    * @brief Constructor
    *
    * Initializes a piece with the given node, color, and position.
    *
    * @param node The node representing the piece.
    * @param color The color of the piece (0 for black, 1 for white).
    * @param row The starting row of the piece.
    * @param col The starting column of the piece.
    */
    Piece(Eng::Node* node, int color, int row, int col);

    /**
    * @brief Get the node representing the piece
    *
    * @return The node representing the piece.
    */
    Eng::Node* getNode() const;

    /**
    * @brief Move the piece to the left
    */
    void moveLeft();

    /**
    * @brief Move the piece to the right
    */
    void moveRight();

    /**
    * @brief Move the piece up
    */
    void moveUp();

    /**
    * @brief Move the piece down
    */
    void moveDown();

    /**
    * @brief Remove the piece to the cemetery
    *
    * Moves the piece to the cemetery and updates its position.
    *
    * @param cemeteryPosition The position of the cemetery.
    * @param eliminatedPieces The number of pieces eliminated.
    */
    void remove(glm::mat4 cemeteryPosition, int eliminatedPieces);

    /**
    * @brief Replace the piece on the board
    *
    * Replaces the piece on the board from the cemetery.
    */
    void replace();

    /**
    * @brief Reset the piece to its initial state
    */
    void reset();

    /**
    * @brief Reset the piece to its initial position
    */
    void resetPosition();

    /**
    * @brief Set a new position for the piece
    *
    * @param newRow The new row of the piece.
    * @param newCol The new column of the piece.
    */
    void setNewPosition(int newRow, int newCol);

    /**
    * @brief Set a new transform for the piece
    *
    * @param newTransform The new transform matrix of the piece.
    */
    void setNewTransform(glm::mat4 newTransform);

    /**
    * @brief Get the color of the piece
    *
    * @return The color of the piece (0 for black, 1 for white).
    */
    int getColor() const;

    /**
	* @brief Check if the piece is eliminated/has been captured
    *
    * @return True if the piece is eliminated, false otherwise.
    */
    bool isEliminated() const;

    /**
    * @brief Get the current row of the piece
    *
    * @return The current row of the piece.
    */
    int getRow() const;

    /**
    * @brief Get the current column of the piece
    *
    * @return The current column of the piece.
    */
    int getCol() const;

    /**
    * @brief Get the temporary row of the piece
    *
    * @return The temporary row of the piece.
    */
    int getTempRow() const;

    /**
    * @brief Get the temporary column of the piece
    *
    * @return The temporary column of the piece.
    */
    int getTempCol() const;

    /**
    * @brief Get the current transform of the piece
    *
    * @return The current transform matrix of the piece.
    */
    glm::mat4 getPieceTransform() const;

    /**
    * @brief Get the initial transform of the piece
    *
    * @return The initial transform matrix of the piece.
    */
    glm::mat4 getInitialTransform();

    /**
    * @brief Get the starting row of the piece
    *
    * @return The starting row of the piece.
    */
    int getStartRow() const;

    /**
    * @brief Get the starting column of the piece
    *
    * @return The starting column of the piece.
    */
    int getStartCol() const;

private:
    Eng::Node* node; /**< The node representing the piece */
    int color; /**< The color of the piece (0 for black, 1 for white) */
    bool eliminated = false; /**< Whether the piece is eliminated */

    int row, col; /**< The current row and column of the piece */
    int tempRow, tempCol; /**< The temporary row and column of the piece */
    glm::mat4 pieceTransform; /**< The current transform matrix of the piece */

    glm::mat4 initialTransform; /**< The initial transform matrix of the piece */
    int startRow, startCol; /**< The starting row and column of the piece */
};