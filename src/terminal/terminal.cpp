#include "terminal.hpp"
#include <asm-generic/ioctls.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include <stdlib.h>

Terminal::Terminal() {
    enableRawMode();
    enterAlternateBuffer();
    hideCursor();
}

Terminal::~Terminal() {
    showCursor();
    exitAlternateBuffer();
    disableRawMode();
}

void Terminal::enableRawMode() {
    if (raw_mode_enabled) return;

    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    raw_mode_enabled = true;
}

void Terminal::disableRawMode() {
    if (!raw_mode_enabled) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    raw_mode_enabled = false;
}

void Terminal::getWindowSize(int& rows, int& cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        cols = 80;
        rows = 24;
    } else {
        cols = ws.ws_col;
        rows = ws.ws_row;
    }
}

void Terminal::enterAlternateBuffer() { std::cout << "\x1b[?1049h" << std::flush; }
void Terminal::exitAlternateBuffer()  { std::cout << "\x1b[?1049l" << std::flush; }
void Terminal::hideCursor()           { std::cout << "\x1b[?25l"  << std::flush; }
void Terminal::showCursor()           { std::cout << "\x1b[?25h"  << std::flush; }
void Terminal::clearScreen()          { std::cout << "\x1b[2J\x1b[H" << std::flush; }
