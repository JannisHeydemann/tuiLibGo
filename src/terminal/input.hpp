#ifndef INPUT_HPP
#define INPUT_HPP

enum class Key {
    None,
    Char,
    Up,
    Down,
    Left,
    Right,
    Enter,
    Escape,
    Backspace
};

struct KeyEvent {
    Key key = Key::None;
    char ch = '\0';
};

class InputReader {
    public:
        static KeyEvent readKey();
};

#endif
