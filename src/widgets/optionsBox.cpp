#include "optionsBox.hpp"
#include <vector>

void optionsBox::setOptions(std::vector<std::string> o) { options = o; }
void optionsBox::setHeight(int h) { 
    if (h >= static_cast<int>(2 + options.size()))
    Widget::setHeight(h);
}

std::vector<std::string> optionsBox::getOptions() { return options; }

void optionsBox::draw(Canvas& c) const {
    // vars
    int width = getWidth();
    // int height = getHeight();
    int x = getXCoords();
    int y = getYCoords();

    std::string title = getTitle();

    drawBorder(c, getBorder());

    // HACK: just draw the text for now. do special positioning later.
    // render the title in the second row (first row is border)
    drawText(c, x + 1 + ((width - 2 - title.length()) / 2), y + 1, title);

    return;
}
