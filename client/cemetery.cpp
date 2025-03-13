/**
* @file cemetery.cpp
* @brief Implementation of the Cemetery class
*
* This file contains the implementation of the Cemetery class methods.
*
* @see Cemetery
* @see cemetery.h
*
* @date 2025
*
* @details The Cemetery class manages the captured pieces, keeping track of their positions and the number of pieces eliminated for each color.
*
* @see Piece
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/
#include "cemetery.h"

/**
* @brief Add a piece to the cemetery
*
* Adds a captured piece to the cemetery.
*
* @param piece The piece to be added to the cemetery.
*/
void Cemetery::addPieceToDeleted(Piece* piece) {
    deletedPieces.push_back(piece);
}

/**
* @brief Remove a piece from the cemetery
*
* Removes a piece from the cemetery.
*
* @param piece The piece to be removed from the cemetery.
*/
void Cemetery::removePieceFromDeleted(Piece* piece) {
    for (int i = 0; i < deletedPieces.size(); i++) {
        if (deletedPieces[i] == piece) {
            deletedPieces.erase(deletedPieces.begin() + i);
            return;
        }
    }
}

/**
* @brief Get the number of black pieces eliminated
*
* @return The number of black pieces eliminated.
*/
int Cemetery::getBlackPiecesEliminated() const {
    return blackPiecesEliminated;
}

/**
* @brief Get the number of white pieces eliminated
*
* @return The number of white pieces eliminated.
*/
int Cemetery::getWhitePiecesEliminated() const {
    return whitePiecesEliminated;
}

/**
* @brief Get the position of the black cemetery
*
* @return The position of the black cemetery.
*/
glm::mat4 Cemetery::getBlackCemeteryPos() const {
    return blackCemeteryPos;
}

/**
* @brief Get the position of the white cemetery
*
* @return The position of the white cemetery.
*/
glm::mat4 Cemetery::getWhiteCemeteryPos() const {
    return whiteCemeteryPos;
}

/**
* @brief Get the list of deleted pieces
*
* @return A vector containing the deleted pieces.
*/
std::vector<Piece*> Cemetery::getDeletedPieces() const {
    return deletedPieces;
}

/**
* @brief Set the number of the eliminated black pieces
*
* @param newValue The new number of the eliminated black pieces.
*/
void Cemetery::setBlackPiecesEliminated(int newValue) {
    this->blackPiecesEliminated = newValue;
}

/**
* @brief Set the number of the eliminated white pieces 
*
* @param newValue The new number of the eliminated white pieces.
*/
void Cemetery::setWhitePiecesEliminated(int newValue) {
    this->whitePiecesEliminated = newValue;
}