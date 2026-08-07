#include "box.hpp"
#include <vector>

class Canvas;

class optionsBox : public Box {
    private:
        std::vector<std::string> options;
    public:
        void setOptions(std::vector<std::string> o);
        void setHeight(int h);

        std::vector<std::string> getOptions();

        void draw(Canvas& c) const override;
};
