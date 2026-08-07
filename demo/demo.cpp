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

    int hide = 0;
    
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
    menu.setOptions({"New Game", "load Game", "options", "exit", "haha"});
    menu.setWidth(25);
    menu.setHeight(10);
    menu.setBorder('B');
    menu.setTitle("Fortnite");
    menu.setSelectionChar('>');
    menu.setSelection(0);

    while (running == 1) {
        canvas.clear();
        if (hide == 1) box.draw(canvas);
        if (hide == 1) menu.draw(canvas);
        curr = InRea.readKey();
        canvas.render();
        switch (curr.key) {
            case Key::Down:
                menu.changeSelection(true);
                break;
            case Key::Up:
                menu.changeSelection(false);
                break;
            case Key::Escape:
                running = 0;
                break;
            case Key::Left:
                if (hide == 1) {
                    hide = 0;
                    break;
                } else {
                    hide = 1;
                    break;
                }
            default:
                break;
            
        }
    }


    
    return 0;
}
