#include "optionsBox.hpp"
#include <vector>

void optionsBox::setOptions(std::vector<std::string> o) { options = o; }
void optionsBox::setSelectionChar(char c) { selectionChar = c; }
void optionsBox::setSelection(int s) { selection = s; }
void optionsBox::setHeight(int h) { 
    if (h >= static_cast<int>(2 + options.size()))
    Widget::setHeight(h);
}

std::vector<std::string> optionsBox::getOptions() const { return options; }
char optionsBox::getSelectionChar() const { return selectionChar; }
int optionsBox::getSelection() const { return selection; }

void optionsBox::draw(Canvas& c) const {
    if (options.size() == 0) {
        return; // if options are 0, just use a normal box.
    }
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

    
    for (int i = 0; i < static_cast<int>(options.size()); ++i) {
        // offset the x axis by 2 so there is space for the selection character and spacing
        if (getSelection() == i) {
            drawText(c, x + 2, y + 2 + i, std::string(1,selectionChar) + " " + options[i]);
        } else {
            // two spaces for the missing selection char
            drawText(c, x + 2, y + 2 + i, "  " + options[i]);
        }
    }

    return;
}

// TRUE is down, FALSE is up
void optionsBox::changeSelection(bool direction) {
    int sel = getSelection();
    int last = static_cast<int>(options.size()) - 1;
    if (direction) {
        // move down
        if (sel == last) {
            sel = 0;
        } else {
            sel++;
        }
    } else {
        // move up
        if (sel == 0) {
            sel = last;
        } else {
            sel--;
        }
    }
    setSelection(sel);
}
