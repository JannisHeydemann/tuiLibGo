#include "canvas/canvas.hpp"
#include "terminal/terminal.hpp"
#include "widgets/optionsBox.hpp"
#include "widgets/box.hpp"
#include "terminal/input.hpp"

int main() {
    Terminal term;
    InputReader InRea;
    KeyEvent curr;
    int running = 1;
    Canvas canvas(80, 24);

    Box box;
    box.setXCoords(5);
    box.setYCoords(3);
    box.setWidth(20);
    box.setHeight(8);
    box.setTitle("Hallo!");
    box.setBorder('#');
    
    optionsBox menu;
    menu.setXCoords(30);
    menu.setYCoords(3);
    menu.setOptions({"New Game", "load Game", "options", "exit"});
    menu.setWidth(25);
    menu.setHeight(10);
    menu.setBorder('B');

    while (running == 1) {
        canvas.clear();
        box.draw(canvas);
        menu.draw(canvas);
        canvas.render();
        curr = InRea.readKey();
        if (curr.key == Key::Escape) {
            running = 0;
        }
    }


    
    return 0;
}
