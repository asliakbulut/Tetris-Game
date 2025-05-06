#ifndef _BOARD_HEADER_
#define _BOARD_HEADER_
#include <cmath>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>

#include "defines.h"
#include "piece.hpp"
#include "utils.hpp"

using namespace std;

enum MovementType {
  PERSIST,
  SLIDE,
  UNABLE,
  GAME_OVER,
};

class Board {
private:
  int **matrix;
  int width;
  int height;
  string *colors;
  Piece *pieces;

  void initMatrix() {
    matrix = new int *[height];
    for (size_t i = 0; i < height; i++) {
      matrix[i] = new int[width];
    }
  }

  void disposeMatrix() { delete[] matrix; }

public:
  Board(int w, int h) : width(w), height(h) {
    initMatrix();

    pieces = Piece::createPieces();
    colors = Piece::createColors();
  }
  //  ~Board() { disposeMatrix(); }
  int getWidth() { return width; }
  int getHeight() { return height; }

  void draw(Piece holded) {
    // Draws the game board using unicode characters

    Utils::drawBoxStart(width);

    // draw grid
    for (size_t i = 0; i < height; i++) {
      cout << "║";
      for (size_t j = 0; j < width; j++) {
        int value = matrix[i][j];
        if (value == 0) {
          cout << " ."; // boş hücre çiz
        } else {
          int c = value;
          if (value < 0) {
            c = -value;
          }
          // cout << value;
          Utils::drawShape(i, j, colors[c]); // parçanın bloğunu çiz
        }

        // CLEAR BUFFERED PIECE
        if (matrix[i][j] < 0) {
          matrix[i][j] = 0;
        }
      }
      cout << "║" << endl;
    }

    Utils::drawBoxEnd(width);

    cout << "Hold: " << endl;
    if (holded.getType() != EMPTY_PIECE) {

      Utils::drawBoxStart(holded.getWidth());
      for (size_t i = 0; i < holded.getHeight(); i++) {
        cout << "║";
        for (size_t j = 0; j < holded.getWidth(); j++) {
          int value = holded.getMatrix()[i][j];
          if (value == 0) {
            cout << " ."; // boş hücre çiz
          } else {
            int c = value;
            if (value < 0) {
              c = -value;
            }
            // cout << value;
            Utils::drawShape(i, j, colors[c]); // parçanın bloğunu çiz
          }
        }
        cout << "║" << endl;
      }

      Utils::drawBoxEnd(holded.getWidth());
    } else {
      cout << "empty" << endl;
    }
  }

  MovementType checkCollision(Piece piece, int x, int y, int dx, int dy,
                              int rotation) {
    int **points = piece.calculatePieceAt(x, y);
    if (rotation > 0) {
      Utils::rotate(points, piece.getHeight() * piece.getWidth(), x, y,
                    (rotation % 4) * 90 * (M_PI / 180));
    }

    for (size_t i = 0; i < piece.getWidth() * piece.getHeight(); i++) {
      int cz = points[i][2];
      if (cz == 0)
        continue;
      int cx = points[i][0];
      int cy = points[i][1];

      int nx = cx + dx;
      int ny = cy + dy;

      // if (ny < height && nx >= 0 && nx <)
      //
      if (ny < 0 || cy < 0)
        continue;
      if (ny >= height) {
        return PERSIST;
      }

      if (nx < 0 || nx >= width) { //|| matrix[ny][nx] > 0) {
        return UNABLE;
      }

      if (dx == 1) {
        if (matrix[cy][nx] > 0) {
          return UNABLE;
        } else if (matrix[ny][nx] > 0) {
          if (dy == 1)
            return PERSIST;
          else
            return UNABLE;
        }
      }

      if (matrix[ny][nx] > 0) {
        return PERSIST;
      }
    }
    return SLIDE;
  }

  bool bufferPiece(Piece piece, int x, int y, int rotationCount, bool persist) {
    bool invalid = true;
    int **points = piece.calculatePieceAt(x, y);

    if (rotationCount > 0) {
      Utils::rotate(points, piece.getHeight() * piece.getWidth(), x, y,
                    (rotationCount % 4) * 90 * (M_PI / 180));
    }

    for (size_t i = 0; i < piece.getWidth() * piece.getHeight(); i++) {
      int cx = points[i][0];
      int cy = points[i][1];
      int cz = points[i][2];
      if (cz == 0)
        continue;

      if (cy >= 0 && cx >= 0 && cx < width && cy < height) {
        if (persist)
          matrix[cy][cx] = cz;
        else
          matrix[cy][cx] = -cz;
      }
      invalid = invalid && cy >= 0;
    }

    return invalid;
  }

  Piece *getPieces() { return pieces; }
  int getActivePieceCount() {
    int x = 0;
    for (size_t i = 0; i < PIECE_COUNT; i++) {
      if (pieces[i].getType() != EMPTY_PIECE) {
        x++;
      }
    }
    return x;
  }
  int clearCompleteLines() {
    int count = 0;
    int *toClear = new int[height];

    // Step 1: Find complete lines
    for (size_t i = 0; i < height; i++) {
      int c = 0;
      for (size_t j = 0; j < width; j++) { // Fix the loop condition
        if (matrix[i][j] > 0) {
          c++;
        }
      }
      if (c == width) {
        toClear[count] = i;
        count++;
      }
    }

    // Step 2: Remove complete lines and shift remaining lines down
    for (int i = count - 1; i >= 0; i--) {
      // Remove the complete line 'toClear[i]'
      for (int j = toClear[i]; j > 0; j--) {
        for (size_t k = 0; k < width; k++) {
          matrix[j][k] = matrix[j - 1][k];
        }
      }
      // Set the top line to be empty
      for (size_t k = 0; k < width; k++) {
        matrix[0][k] = 0;
      }
    }

    delete[] toClear; // Free the memory allocated for toClear

    return count; // Return the number of lines cleared
  }

  void hardDrop(Piece piece, int fromX, int fromY, int rotation) {
    int dy = 0;
    while (checkCollision(piece, fromX, fromY, 0, dy, rotation) != PERSIST) {
      if (dy > height) {
        return;
      }
      dy++;
    }
    bufferPiece(piece, fromX, fromY + dy - 1, rotation, true);
  }
};
#endif // !_BOARD_HEADER_
