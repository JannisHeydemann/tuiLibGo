#include "canvas/canvas.hpp"
#include "widgets/optionsBox.hpp"
#include "widgets/box.hpp"
#include <unistd.h>

int main() {
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
    menu.setWidth(25);
    menu.setHeight(10);
    menu.setOptions({"New Game", "load Game", "options", "exit"});

    system("clear");
    canvas.clear();
    box.draw(canvas);
    menu.draw(canvas);
    canvas.render();
    usleep(5000000);
    
    return 0;
}
