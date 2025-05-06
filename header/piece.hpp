#ifndef _PIECE_HEADER_
#define _PIECE_HEADER_

#include <string>

#define PIECE_COUNT 10

enum PieceType {
  EMPTY_PIECE,
  // standard
  I_PIECE,
  J_PIECE,
  L_PIECE,
  O_PIECE,
  S_PIECE,
  T_PIECE,
  Z_PIECE,
  // custom pieces for future use
  C1_PIECE,
  C2_PIECE,
  C3_PIECE,
};

class Piece {
protected:
  PieceType type;
  int **matrix;
  int width;
  int height;
  int origin[2];
  int **calculatedPoints;

public:
  Piece(int **matrix, int width, int height, int oX, int oY, PieceType type)
      : matrix(matrix), width(width), height(height), type(type) {
    origin[0] = oX;
    origin[1] = oY;
  }

  Piece() { type = EMPTY_PIECE; }

  int **getMatrix() { return matrix; }
  int getWidth() { return width; }
  int getHeight() { return height; }
  PieceType getType() { return type; }
  int **getCalculatedPoints() { return calculatedPoints; }
  int getOriginX() { return origin[0]; }
  int getOriginY() { return origin[1]; }

  static Piece *createPieces();
  static std::string *createColors();
  static int **createMatrix(int rawPiece[], int width, int height);

  int **calculatePieceAt(int x, int y);
};

#endif // !_PIECE_HEADER_
