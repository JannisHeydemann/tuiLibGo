#include "input.hpp"
#include <unistd.h>

KeyEvent InputReader::readKey() {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) <= 0) {
        return {Key::None, '\0'};
    }

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) <= 0) return {Key::Escape, '\0'};
        if (read(STDIN_FILENO, &seq[1], 1) <= 0) return {Key::Escape, '\0'};

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return {Key::Up, '\0'};
                case 'B': return {Key::Down, '\0'};
                case 'C': return {Key::Right, '\0'};
                case 'D': return {Key::Left, '\0'};
            }
        }
        return {Key::Escape, '\0'};
    }

    if (c == '\n' || c == '\r') return {Key::Enter, '\0'};
    if (c == 127 || c == 8)     return {Key::Backspace, '\0'};

    return {Key::Char, c};
}
