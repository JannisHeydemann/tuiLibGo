#ifndef BOX_HPP
#define BOX_HPP

#include <string>
#include "canvas/canvas.hpp"
#include "widgets.hpp"

class Box: public Widget {
private:
    char border;
    std::string title;
public:
    void setBorder(char b);
    void setTitle(std::string t);

    char getBorder() const;
    std::string getTitle() const;

    void draw(Canvas& c) const override;
};

#endif
