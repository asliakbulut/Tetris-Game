#ifndef _UTILS_HEADER
#define _UTILS_HEADER
#include "defines.h"
#include <iostream>
#include <stdlib.h>

using namespace std;
class Utils {

public:
  static void drawBoxStart(int width) {
    cout << "╔";
    for (size_t i = 0; i < 2 * width; i++) {
      cout << "═";
    }
    cout << "╗" << endl;
  }

  static void drawBoxEnd(int width) {
    cout << "╚";
    for (size_t i = 0; i < 2 * width; i++) {
      cout << "═";
    }
    cout << "╝" << endl;
  }

  static void drawShape(int startY, int startX, const string &color) {
    std::cout << color << BLANK << BLANK << RESET;
  }

  static void rotate(int **points, int length, int oX, int oY, double beta) {
    for (size_t i = 0; i < length; i++) {
      double X = points[i][0];
      double Y = points[i][1];

      double xo = X - oX;
      double yo = Y - oY;

      double x_new = xo * cos(beta) - yo * sin(beta);
      double y_new = xo * sin(beta) + yo * cos(beta);

      double x_rotated = x_new + oX;
      double y_rotated = y_new + oY;

      points[i][0] = x_rotated;
      points[i][1] = y_rotated;
    }
  }
};

#endif // !_UTILS_HEADER
