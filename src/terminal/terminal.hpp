#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <termios.h>

class Terminal {
    private:
        struct termios orig_termios{};
        bool raw_mode_enabled = false;

    public:
        Terminal();
        ~Terminal();

        void enableRawMode();
        void disableRawMode();

        void getWindowSize(int& rows, int& cols);

        // ANSI helpers
        void hideCursor();
        void showCursor();
        void clearScreen();
        void enterAlternateBuffer(); // clean full-screen buffer
        void exitAlternateBuffer(); // restore original shell
};

#endif
