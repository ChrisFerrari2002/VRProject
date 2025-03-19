/**
* @file board.cpp
* @brief Implementation of the Board class
*
* This file contains the implementation of the Board class methods.
*
* @see Board
* @see board.h
*
* @date 2025
*
* @details The Board class represents the chessboard, managing the state of the pieces, the cemetery for captured pieces, and the history of moves for undo/redo functionality.
*
* @see Piece, Move, Cemetery
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

//////////////
// #INCLUDE //
//////////////
#include "board.h"
#include "move.h"
#include <iostream>
#include <regex>

// define the size of the chessboard (8x8)
#define CHESSBOARD_SIZE 8
// define the size of a block
#define BLOCK_SIZE 0.0835f

/**
* @brief Constructor
*
* Initializes an empty chessboard and sets up the cemetery.
*/
Board::Board() : table(8, std::vector<Piece*>(8, nullptr)) {}

/**
* @brief Get the position of a node
*
* @param nodeName The name of the node.
* @return A pair representing the row and column of the node.
*/
std::pair<int, int> Board::getPosition(const std::string& nodeName) {
    // Regex per identificare i pezzi (p_<color>_<type><number>)
    std::regex pieceRegex("p_(white|black)_(rook|horse|bishop|queen|king|pawn)([0-9]*)");
    std::smatch match;

    if (std::regex_match(nodeName, match, pieceRegex)) {
        std::string color = match[1];
        std::string type = match[2];
        int number = match[3].length() > 0 ? std::stoi(match[3]) : 0;

        if (type == "rook") {
            return (color == "white") ? std::make_pair(0, number == 1 ? 0 : 7) : std::make_pair(7, number == 1 ? 7 : 0);
        }
        else if (type == "horse") {
            return (color == "white") ? std::make_pair(0, number == 1 ? 1 : 6) : std::make_pair(7, number == 1 ? 1 : 6);
        }
        else if (type == "bishop") {
            return (color == "white") ? std::make_pair(0, number == 1 ? 2 : 5) : std::make_pair(7, number == 1 ? 5 : 2);
        }
        else if (type == "queen") {
            return (color == "white") ? std::make_pair(0, 4) : std::make_pair(7, 4);
        }
        else if (type == "king") {
            return (color == "white") ? std::make_pair(0, 3) : std::make_pair(7, 3);
        }
        else if (type == "pawn") {
            return (color == "white") ? std::make_pair(1, number - 1) : std::make_pair(6, number - 1);
        }
    }

    return std::make_pair(-1, -1); // Posizione invalida
}

/**
* @brief Print the entire chessboard in the current state
*
* Prints the current state of the chessboard to the console.
*/
void Board::printChessboard() {
    for (int row = 0; row < CHESSBOARD_SIZE; ++row) {
        for (int col = 0; col < CHESSBOARD_SIZE; ++col) {
            if (table[row][col] != nullptr) {
                std::cout << table[row][col]->getNode()->getName() << "\t";
            }
            else {
                std::cout << "empty\t";
            }
        }
        std::cout << std::endl;
    }
}

/**
* @brief Set the selected piece
*
* Sets the piece that the player wants to move.
*
* @param node The node representing the piece to be selected.
*/
void Board::setSelectedPiece(Eng::Node* node) {
    Piece* pieceFound;
    for (int row = 0; row < CHESSBOARD_SIZE; ++row) {
        for (int col = 0; col < CHESSBOARD_SIZE; ++col) {
            if (table[row][col] == nullptr) continue;
            pieceFound = table[row][col];

            if (pieceFound->getNode()->getName() == node->getName()) {
                selectedPiece = table[row][col];
                return;
            }
        }
    }

    std::vector<Piece*> deleted = cemetery->getDeletedPieces();
    for (int i = 0; i < deleted.size(); ++i) {
        if (deleted[i]->getNode()->getName() == node->getName()) {
            selectedPiece = deleted[i];
            return;
        }
    }
}

/**
* @brief Get the selected piece
*
* @return The currently selected piece.
*/
Piece* Board::getSelectedPiece() const {
    return selectedPiece;
}

/**
* @brief Save the current position of the selected piece
*
* Confirms the movement of the selected piece and updates its position on the board.
*/
void Board::confirmMovement() {
    if (selectedPiece == nullptr) return;
    if (selectedPiece->isEliminated()) return;

    int pieceRow = selectedPiece->getTempRow();
    int pieceCol = selectedPiece->getTempCol();
    int initialPieceRow = selectedPiece->getRow();
    int initialPieceCol = selectedPiece->getCol();

    // Destination empty
    if (table[pieceRow][pieceCol] == nullptr) {
        table[initialPieceRow][initialPieceCol] = nullptr;
        table[pieceRow][pieceCol] = selectedPiece;
        Move* move = new Move(selectedPiece);
        addMove(move);
        selectedPiece->setNewPosition(pieceRow, pieceCol);
        return;
    }

    Move* move = new Move(selectedPiece);
    addMove(move);

    // Destination same color
    if (table[pieceRow][pieceCol]->getColor() == selectedPiece->getColor()) {
        selectedPiece->resetPosition();
        return;
    }

    Piece* capturedPiece = table[selectedPiece->getTempRow()][selectedPiece->getTempCol()];
    Move* capturedMove = new Move(capturedPiece);
    capturePiece(capturedPiece);
    capturedMove->setEliminated(true);
    capturedMove->setEndPosition(-1, -1);
    addMove(capturedMove);

    // Move piece to new position
    table[initialPieceRow][initialPieceCol] = nullptr;
    table[pieceRow][pieceCol] = selectedPiece;
    selectedPiece->setNewPosition(pieceRow, pieceCol);
}

/**
* @brief Capture a piece sending it to the cemetery
*
* Moves a captured piece to the cemetery.
*
* @param capturedPiece The piece to be captured.
*/
void Board::capturePiece(Piece* capturedPiece) {
    // Place captured piece in cemetery
    if (capturedPiece->getColor() == 0) {
        capturedPiece->remove(cemetery->getBlackCemeteryPos(), cemetery->getBlackPiecesEliminated());
        int piecesEliminated = cemetery->getBlackPiecesEliminated() + 1;
        cemetery->setBlackPiecesEliminated(piecesEliminated);
    }
    else {
        capturedPiece->remove(cemetery->getWhiteCemeteryPos(), cemetery->getWhitePiecesEliminated());
        int piecesEliminated = cemetery->getWhitePiecesEliminated() + 1;
        cemetery->setWhitePiecesEliminated(piecesEliminated);
    }
    cemetery->addPieceToDeleted(capturedPiece);
}

/**
* @brief Reset the board to the initial state
*
* Resets the chessboard to its initial state, with all pieces in their starting positions.
*/
void Board::resetBoard() {
    for (int row = 0; row < CHESSBOARD_SIZE; ++row) {
        for (int col = 0; col < CHESSBOARD_SIZE; ++col) {
            table[row][col] = nullptr;
        }
    }

    for (Piece* piece : pieces) {
        int pieceRow = piece->getStartRow();
        int pieceCol = piece->getStartCol();

        table[pieceRow][pieceCol] = piece;
        piece->reset();
    }

    cemetery->getDeletedPieces().clear();
    cemetery->setBlackPiecesEliminated(0);
    cemetery->setWhitePiecesEliminated(0);

    moves.clear();
    currentMove = -1;
}

/**
* @brief Add the move to the list of moves
*
* Adds a move to the list of moves for undo/redo functionality.
*
* @param move The move to be added.
*/
void Board::addMove(Move* move) {
    if (currentMove < moves.size() - 1) {
        moves.erase(moves.begin() + currentMove + 1, moves.end());
    }

    // TODO: check whyyyy
    if (currentMove == -1 && moves.size() > 0) {
        moves.clear();
    }

    moves.push_back(move);
    currentMove++;
}

/**
* @brief Undo the last move
*
* Reverts the last move made on the chessboard.
*/
void Board::undo() {
    if (currentMove < 0) return;

    Move* move = moves[currentMove];
    Piece* movedPiece = move->getMovedPiece();

    if (move->getIsEliminated()) {
        table[move->getStartRow()][move->getStartCol()] = movedPiece;
        movedPiece->setNewPosition(move->getStartRow(), move->getStartCol());
        movedPiece->setNewTransform(move->getStartTransform());
        movedPiece->replace();

        if (movedPiece->getColor() == 0) {
            int piecesEliminated = cemetery->getBlackPiecesEliminated() - 1;
            cemetery->setBlackPiecesEliminated(piecesEliminated);
        }
        else {
            int piecesEliminated = cemetery->getWhitePiecesEliminated() - 1;
            cemetery->setWhitePiecesEliminated(piecesEliminated);
        }

        cemetery->removePieceFromDeleted(movedPiece);

        currentMove--;
        undo();

        return;
    }
    else
    {
        if (table[move->getEndRow()][move->getEndCol()]->getNode()->getId() == movedPiece->getNode()->getId()) {
            table[move->getEndRow()][move->getEndCol()] = nullptr;
        }
        table[move->getStartRow()][move->getStartCol()] = movedPiece;
        movedPiece->setNewPosition(move->getStartRow(), move->getStartCol());
        movedPiece->setNewTransform(move->getStartTransform());

        currentMove--;
    }
}

/**
* @brief Redo the undone move
*
* Reapplies the last move that was undone.
*/
void Board::redo() {
    if (currentMove == moves.size() - 1) return;

    currentMove++;

    Move* move = moves[currentMove];
    Piece* movedPiece = move->getMovedPiece();
    Piece* capturedPiece = nullptr;

    if (table[move->getEndRow()][move->getEndCol()] != nullptr) {
        currentMove++;
        capturedPiece = table[move->getEndRow()][move->getEndCol()];
        capturePiece(capturedPiece);
    }

    // Move piece to new position
    table[move->getEndRow()][move->getEndCol()] = movedPiece;
    table[move->getStartRow()][move->getStartCol()] = nullptr;
    movedPiece->setNewPosition(move->getEndRow(), move->getEndCol());
    movedPiece->setNewTransform(move->getEndTransform());
}