#include "box.hpp"
#include "canvas/canvas.hpp"

void Box::setBorder(char b)  { border = b; }
void Box::setTitle(std::string t)  {
    if (getWidth() > static_cast<int>(t.length() + 2)) title = t;
}

char Box::getBorder() const { return border; }
std::string Box::getTitle() const { return title; }

void Box::draw(Canvas& c) const {
    // vars
    int width = getWidth();
    // int height = getHeight();
    int x = getXCoords();
    int y = getYCoords();

    drawBorder(c, getBorder());

    // HACK: just draw the text for now. do special positioning later.
    // render the title in the second row (first row is border)
    drawText(c, x + 1 + ((width - 2 - title.length()) / 2), y + 1, title);

    return;
}
