#ifndef _GAME_HEADER_
#define _GAME_HEADER_

#include "board.hpp"
#include "defines.h"
#include <SFML/Graphics.hpp>
#include <thread>

class Game {
public:
  Game() : board(BOARD_WIDTH, BOARD_HEIGHT) {
    window = initWindow();
    initGame();
  };

  void initGame();
  void loop();

  Board getBoard() { return board; }
  void selectNewPiece();
  void gameover();
  void switchPiece();
  sf::Keyboard::Key handleEvents();
  std::unique_ptr<sf::RenderWindow> initWindow();

  void printRules() {
    cout << BLUE << "SPACE: Harddrop | " << RESET;
    cout << YELLOW << "Q: Rotate | " << RESET;
    cout << GREEN << "C: Hold | " << RESET;
    cout << RED << "Arrow Keys/AWSD: Movement | " << RESET;
    cout << "X: Quit" << endl;
    cout << RED << "Press ENTER to play" << RESET << endl;
    cout << "Keep the opened window focused to play the game!" << endl;

    while (handleEvents() != sf::Keyboard::Return) {
    }
    std::cout << "\033[2J\033[1;1H";
  }

private:
  Board board;
  int fps = 5;
  Piece movingPiece;
  int pieceX;
  int pieceY;
  int rotation;
  Piece holdedPiece;
  int score;
  std::unique_ptr<sf::RenderWindow> window;
};

#endif // !_GAME_HEADER_
