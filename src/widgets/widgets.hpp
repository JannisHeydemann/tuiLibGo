#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <string>

class Canvas;

class Widget {
    private:
        struct W_size {
            int width;
            int height;
        };
        int x_;
        int y_;
        W_size size;

    protected:
        void drawBorder(Canvas& c, char b) const;
        void drawText(Canvas& c, int x, int y, std::string text) const;
    public:
        int getXCoords() const;
        int getYCoords() const;
        int getWidth() const;
        int getHeight() const;

        void setXCoords(int x);
        void setYCoords(int y);
        void setWidth(int w);
        void setHeight(int h);

        virtual void draw(Canvas& c) const = 0;
        virtual ~Widget() = default;
};


#endif // WIDGETS_HPP
