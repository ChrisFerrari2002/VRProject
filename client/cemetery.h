/**
* @file cemetery.h
* @brief Cemetery class header file
*
* This file contains the definition of the Cemetery class that represents the cemetery for captured chess pieces.
*
* @details The Cemetery class manages the captured pieces, keeping track of their positions and the number of pieces eliminated for each color.
*
* @date 2025
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
* @brief Cemetery class
*
* The Cemetery class manages the captured pieces, keeping track of their positions and the number of pieces eliminated for each color.
*/
class Cemetery {
public:
    /**
    * @brief Constructor
    *
    * Initializes the cemetery with the positions for black and white pieces cemeteries.
    *
    * @param blackCemeteryPos The position of the black cemetery.
    * @param whiteCemeteryPos The position of the white cemetery.
    */
    Cemetery(glm::mat4 blackCemeteryPos, glm::mat4 whiteCemeteryPos)
        : blackCemeteryPos(blackCemeteryPos), whiteCemeteryPos(whiteCemeteryPos) {
    }


    /**
    * @brief Add a piece to the cemetery
    *
    * Adds a captured piece to the cemetery.
    *
    * @param piece The piece to be added to the cemetery.
    */
    void addPieceToDeleted(Piece* piece);

    /**
    * @brief Remove a piece from the cemetery
    *
    * Removes a piece from the cemetery.
    *
    * @param piece The piece to be removed from the cemetery.
    */
    void removePieceFromDeleted(Piece* piece);

    /**
    * @brief Get the number of black pieces eliminated
    *
    * @return The number of black pieces eliminated.
    */
    int getBlackPiecesEliminated() const;

    /**
    * @brief Get the number of white pieces eliminated
    *
    * @return The number of white pieces eliminated.
    */
    int getWhitePiecesEliminated() const;

    /**
    * @brief Get the position of the black cemetery
    *
    * @return The position of the black cemetery.
    */
    glm::mat4 getBlackCemeteryPos() const;

    /**
    * @brief Get the position of the white cemetery
    *
    * @return The position of the white cemetery.
    */
    glm::mat4 getWhiteCemeteryPos() const;

    /**
    * @brief Get the list of deleted pieces
    *
    * @return A vector containing the deleted pieces.
    */
    std::vector<Piece*> getDeletedPieces() const;

    /**
    * @brief Set the number of black pieces eliminated
    *
    * @param newValue The new number of black pieces eliminated.
    */
    void setBlackPiecesEliminated(int newValue);

    /**
    * @brief Set the number of white pieces eliminated
    *
    * @param newValue The new number of white pieces eliminated.
    */
    void setWhitePiecesEliminated(int newValue);

private:
    int blackPiecesEliminated = 0; /**< Number of black pieces eliminated */
    int whitePiecesEliminated = 0; /**< Number of white pieces eliminated */

    glm::mat4 whiteCemeteryPos; /**< Position of the white cemetery */
    glm::mat4 blackCemeteryPos; /**< Position of the black cemetery */

    std::vector<Piece*> deletedPieces; /**< List of all deleted pieces */
};

