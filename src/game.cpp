#include "game.hpp"
#include "board.hpp"
#include "defines.h"
#include "piece.hpp"
#include <cstdlib>
#include <thread>

#define GAME_SPEED 500

std::atomic<int> dy(0);
std::atomic<bool> quit(false);

void updateDy() {
  while (true && !quit.load(std::memory_order_relaxed)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(GAME_SPEED));
    dy = 1;
  }
}

void Game::initGame() {
  score = 0;
  selectNewPiece();
}

void Game::loop() {

  printRules();

  std::thread dyThread(updateDy);

  // bool quit = false;
  while (!quit.load(std::memory_order_relaxed)) {
    int dx = 0;
    int dr = 0;
    // Update dy
    int currentDy = dy.exchange(0);
    bool hardDrop = false;
    sf::Keyboard::Key key = handleEvents();

    switch (key) {
    case sf::Keyboard::A:
    case sf::Keyboard::Left:
      dx = -1;
      break;
    case sf::Keyboard::D:
    case sf::Keyboard::Right:
      dx = 1;
      break;
    case sf::Keyboard::Q:
      dr++;
      break;
    case sf::Keyboard::X:
      quit.exchange(true);
      break;
    case sf::Keyboard::C:
      switchPiece();
      break;
    case sf::Keyboard::Space:
      board.hardDrop(movingPiece, pieceX, pieceY, rotation);
      hardDrop = true;
      break;
    case sf::Keyboard::Escape:
      quit.exchange(true);
      break;
    default:
      break;
    }

    MovementType t = board.checkCollision(movingPiece, pieceX, pieceY, dx,
                                          currentDy, rotation + dr);
    if (t != SLIDE && dr != 0) {
      dr = 0;
      t = board.checkCollision(movingPiece, pieceX, pieceY, dx, currentDy,
                               rotation);
    }

    if (!hardDrop)
      switch (t) {
      case PERSIST:
        if (!board.bufferPiece(movingPiece, pieceX, pieceY, rotation, true)) {
          gameover();
          quit.exchange(true);
          break;
        }

        selectNewPiece();
        break;
      case SLIDE:
        pieceX += dx;
        pieceY += currentDy;
        board.bufferPiece(movingPiece, pieceX, pieceY, rotation, false);
        break;
      case UNABLE:
        pieceY += currentDy;
        board.bufferPiece(movingPiece, pieceX, pieceY, rotation, false);
        break;
      case GAME_OVER:
        quit.exchange(true);
        gameover();
        break;
      }

    rotation += dr;
    // Fill the board with some random blocks
    // board.draw(holdedPiece);

    int l = board.clearCompleteLines();
    score += l * l * 10;
    board.draw(holdedPiece);

    // draw score
    std::cout << "Score: " << score << std::endl;

    // Delay to achieve 30 FPS
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));

    std::cout << "\033[2J\033[1;1H";

    if (hardDrop)
      selectNewPiece();
  }

  quit.exchange(true);
  dyThread.join();
}

void Game::selectNewPiece() {
  movingPiece =
      getBoard().getPieces()[(rand() % getBoard().getActivePieceCount()) + 1];
  pieceX = 3;
  pieceY = -movingPiece.getHeight();
  rotation = 0;
}

void Game::gameover() { cout << RED << "GAME OVER" << RESET << endl; }

void Game::switchPiece() {
  Piece tmp = movingPiece;
  selectNewPiece();
  if (holdedPiece.getType() != EMPTY_PIECE) {
    movingPiece = holdedPiece;
  }
  holdedPiece = tmp;
}

sf::Keyboard::Key Game::handleEvents() {
  sf::Event event;
  while (window->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      // Handle window close event
      window->close();
    } else if (event.type == sf::Event::KeyPressed) {
      // Handle key press event
      return event.key.code;
    }
  }

  // Return a special value to indicate no key press
  return sf::Keyboard::Unknown;
}

std::unique_ptr<sf::RenderWindow> Game::initWindow() {
  int width = 100;
  int height = 100;
  std::string title = "Tetris";
  auto window = std::unique_ptr<sf::RenderWindow>(
      new sf::RenderWindow(sf::VideoMode(width, height), title));
  window->setFramerateLimit(60); // Set a frame rate limit (optional)

  return window;
}
