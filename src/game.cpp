// Definition of the game class.
// Tianyun Zhang 2020 all rights reserved.

#include <game.h>
#include <ncurses.h>

#define DEBUG true
#include <debug.h>

Game::Game(WINDOW *screen, int fps /* 60 */) {
  fps = fps;
  status = STATUS_INIT;
  screen = screen;
}

// Run the game process.
void Game::run() {
  while (true) {
    menu();
    if (status == STATUS_EXIT) break;
    play();
    if (status == STATUS_EXIT) break;
  }
}

// Show the game menu.
void Game::menu() {
  Panic("please implement me");
}

// Play the tank game.
void Game::play() {
  Panic("please implement me");
}

// Handle the game logic.
void Game::tick() {
  Panic("please implement me");
}

// Show game over and options.
void Game::over() {
  Panic("please implement me");
}
