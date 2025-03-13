/**
* @file piece.cpp
* @brief Implementation of the Piece class
*
* This file contains the implementation of the Piece class methods.
*
* @see Piece
* @see piece.h
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

//////////////
// #INCLUDE //
//////////////
#include "piece.h"

// Define the size of a block on the chessboard
#define BLOCK_SIZE 0.0835f
// Define the spacing between pieces in the cemetery
#define CIMITERY_SPACING 0.09f
// Define the size of the chessboard
#define CHESSBOARD_SIZE 8

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
Piece::Piece(Eng::Node* node, int color, int row, int col)
	: node(node), color(color), row(row), col(col) {
	tempCol = col;
	tempRow = row;
	startRow = row;
	startCol = col;
	initialTransform = node->getTransform();
	pieceTransform = initialTransform;
}

/**
* @brief Get the node representing the piece
*
* @return The node representing the piece.
*/
Eng::Node* Piece::getNode() const {
	return node;
}

/**
* @brief Set a new position for the piece
*
* Sets the new position of the piece and updates its transform.
*
* @param newRow The new row of the piece.
* @param newCol The new column of the piece.
*/
void Piece::setNewPosition(int newRow, int newCol) {
	this->row = newRow;
	this->col = newCol;
	this->tempRow = newRow;
	this->tempCol = newCol;
	pieceTransform = node->getTransform();
}

/**
* @brief Set a new transformation matrix for the piece
*
* Sets the new transformation matrix of the piece.
*
* @param newTransform The new transform matrix of the piece.
*/
void Piece::setNewTransform(glm::mat4 newTransform) {
	this->pieceTransform = newTransform;
	node->setTransform(newTransform);
}

/**
* @brief Move the piece to the left
*
* Moves the piece one block to the left if possible.
*/
void Piece::moveLeft() {
	if (tempCol == 0 || eliminated) return;
	node->setTransform(glm::translate(node->getTransform(), glm::vec3(-BLOCK_SIZE, 0.0f, 0.0f)));
	tempCol--;
}

/**
* @brief Move the piece to the right
*
* Moves the piece one block to the right if possible.
*/
void Piece::moveRight() {
	if (tempCol == CHESSBOARD_SIZE - 1 || eliminated) return;
	node->setTransform(glm::translate(node->getTransform(), glm::vec3(BLOCK_SIZE, 0.0f, 0.0f)));
	tempCol++;
}

/**
* @brief Move the piece up
*
* Moves the piece one block up if possible.
*/
void Piece::moveUp() {
	if (tempRow == CHESSBOARD_SIZE - 1 || eliminated) return;
	node->setTransform(glm::translate(node->getTransform(), glm::vec3(0.0f, 0.0f, BLOCK_SIZE)));
	tempRow++;
}

/**
* @brief Move the piece down
*
* Moves the piece one block down if possible.
*/
void Piece::moveDown() {
	if (tempRow == 0 || eliminated) return;
	node->setTransform(glm::translate(node->getTransform(), glm::vec3(0.0f, -0.0f, -BLOCK_SIZE)));
	tempRow--;
}

/**
* @brief Remove the piece to the cemetery
*
* Moves the piece to the cemetery and updates its position.
*
* @param cemeteryPosition The position of the cemetery.
* @param eliminatedPieces The number of pieces eliminated.
*/
void Piece::remove(glm::mat4 cemeteryPosition, int eliminatedPieces) {
	int cemeteryRow = eliminatedPieces / 3;
	int cemeteryCol = eliminatedPieces % 3;

	float x = cemeteryRow * CIMITERY_SPACING;
	float z = cemeteryCol * CIMITERY_SPACING;

	glm::mat4 piecePosition = glm::translate(cemeteryPosition, glm::vec3(-x, 0.0f, -z));
	node->setTransform(piecePosition);
	setNewPosition(-1, -1);
	eliminated = true;
}

/**
* @brief Replace the piece on the board
*
* Replaces the piece on the board from the cemetery.
*/
void Piece::replace() {
	eliminated = false;
}

/**
* @brief Reset the piece to its initial state
*
* Resets the piece to its initial position and transform.
*/
void Piece::reset() {
	tempRow = startRow;
	tempCol = startCol;
	row = startRow;
	col = startCol;
	node->setTransform(initialTransform);
	eliminated = false;
}

/**
* @brief Reset the piece to its current position
*
* Resets the piece to its current position and transform.
*/
void Piece::resetPosition() {
	tempRow = row;
	tempCol = col;
	node->setTransform(pieceTransform);
}

/**
* @brief Get the temporary row of the piece
*
* @return The temporary row of the piece.
*/
int Piece::getTempRow() const {
	return tempRow;
}

/**
* @brief Get the temporary column of the piece
*
* @return The temporary column of the piece.
*/
int Piece::getTempCol() const {
	return tempCol;
}

/**
* @brief Get the color of the piece
*
* @return The color of the piece (0 for black, 1 for white).
*/
int Piece::getColor() const {
	return color;
}

/**
* @brief Get the current row of the piece
*
* @return The current row of the piece.
*/
int Piece::getRow() const {
	return row;
}

/**
* @brief Get the current column of the piece
*
* @return The current column of the piece.
*/
int Piece::getCol() const {
	return col;
}

/**
* @brief Get the initial transform of the piece
*
* @return The initial transform matrix of the piece.
*/
glm::mat4 Piece::getInitialTransform() {
	return initialTransform;
}

/**
* @brief Get the starting row of the piece
*
* @return The starting row of the piece.
*/
int Piece::getStartRow() const {
	return startRow;
}

/**
* @brief Get the starting column of the piece
*
* @return The starting column of the piece.
*/
int Piece::getStartCol() const {
	return startCol;
}

/**
* @brief Check if the piece is eliminated
*
* @return True if the piece is eliminated, false otherwise.
*/
bool Piece::isEliminated() const {
	return eliminated;
}

/**
* @brief Get the current transform of the piece
*
* @return The current transform matrix of the piece.
*/
glm::mat4 Piece::getPieceTransform() const {
	return pieceTransform;
}