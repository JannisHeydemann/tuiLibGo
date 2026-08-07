#include "widgets.hpp"
#include "canvas/canvas.hpp"

int Widget::getXCoords() const { return x_; }
int Widget::getYCoords() const { return y_; }
int Widget::getWidth() const { return size.width; }
int Widget::getHeight() const { return size.height; }

void Widget::setXCoords(int x) {
            x_ = x;
        }
void Widget::setYCoords(int y) {
            y_ = y;
        }
void Widget::setWidth(int w) {
            if (w > 2) size.width = w;
        }
void Widget::setHeight(int h) {
            if (h > 2) size.height = h;
        }

void Widget::drawBorder(Canvas& c, char border) const {
    int x = getXCoords();
    int y = getYCoords();
    int width = getWidth();
    int height = getHeight();
    
    // top and bottom border
    for (int i = x; i < x + width; ++i) {
        c.setChar(i, y, border);
        c.setChar(i, y + height - 1, border);
    }

    // left and right border
    for (int i = y; i < y + height; ++i) {
        c.setChar(x, i, border);
        c.setChar(x+width-1, i, border);
    }
}
void Widget::drawText(Canvas& c, int x, int y, std::string text) const {
    for (size_t i = 0; i < text.length(); ++i) {
        c.setChar(x + static_cast<int>(i), y, text[i]);
    }
}
