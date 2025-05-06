#include "piece.hpp"
#include "defines.h"
#include <stdlib.h>
#include <string>

int **Piece::createMatrix(int *rawPiece, int width, int height) {
  int **matrix = new int *[height];
  for (size_t i = 0; i < height; i++) {
    matrix[i] = new int[width];
    for (size_t j = 0; j < width; j++) {
      matrix[i][j] = rawPiece[width * i + j];
    }
  }

  return matrix;
}

Piece *Piece::createPieces() {
  Piece *pieces = new Piece[PIECE_COUNT];

  pieces[I_PIECE] = Piece(Piece::createMatrix(new int[]{1, 1, 1, 1}, 1, 4), 1,
                          4, 0, 1, I_PIECE);

  pieces[J_PIECE] =
      Piece(Piece::createMatrix(new int[]{1, 0, 0, 1, 1, 1}, 3, 2), 3, 2, 1, 1,
            J_PIECE);

  pieces[L_PIECE] =
      Piece(Piece::createMatrix(new int[]{0, 0, 1, 1, 1, 1}, 3, 2), 3, 2, 1, 1,
            L_PIECE);

  pieces[O_PIECE] = Piece(Piece::createMatrix(new int[]{1, 1, 1, 1}, 2, 2), 2,
                          2, 0, 0, O_PIECE);

  pieces[S_PIECE] =
      Piece(Piece::createMatrix(new int[]{0, 1, 1, 1, 1, 0}, 3, 2), 3, 2, 1, 1,
            S_PIECE);

  pieces[T_PIECE] =
      Piece(Piece::createMatrix(new int[]{0, 1, 0, 1, 1, 1}, 3, 2), 3, 2, 1, 1,
            T_PIECE);

  pieces[Z_PIECE] =
      Piece(Piece::createMatrix(new int[]{1, 1, 0, 0, 1, 1}, 3, 2), 3, 2, 1, 1,
            Z_PIECE);

  return pieces;
}

std::string *Piece::createColors() {
  std::string *colors = new std::string[PIECE_COUNT];
  colors[I_PIECE] = CYAN;
  colors[J_PIECE] = BLUE;
  colors[L_PIECE] = WHITE;
  colors[O_PIECE] = YELLOW;
  colors[S_PIECE] = GREEN;
  colors[T_PIECE] = MAGENTA;
  colors[Z_PIECE] = RED;

  return colors;
}

int **Piece::calculatePieceAt(int ox, int oy) {
  int **points = new int *[height * width];

  int z = 0;
  for (int y = 0; y < height; y++) {
    points[z] = new int[3];
    for (int x = 0; x < width; x++) {
      int m = matrix[y][x];
      points[z] = new int[]{x - origin[0] + ox, y - origin[1] + oy,
                            static_cast<int>(type) * m};
      z++;
    }
  }
  calculatedPoints = points;
  return calculatedPoints;
}
