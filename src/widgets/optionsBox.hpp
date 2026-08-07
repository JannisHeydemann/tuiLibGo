#include "box.hpp"
#include <vector>

class Canvas;

class optionsBox : public Box {
    private:
        std::vector<std::string> options;
        char selectionChar;
        int selection;
    public:
        void setOptions(std::vector<std::string> o);
        void setSelectionChar(char c);
        void setHeight(int h);
        void setSelection(int s);

        std::vector<std::string> getOptions() const;
        char getSelectionChar() const;
        int getSelection() const;
        
        void draw(Canvas& c) const override;

        void changeSelection(bool direction); // TRUE = down, FALSE = up
};
