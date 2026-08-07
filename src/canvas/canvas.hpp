#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <cstddef>
#include <limits>
#include <stdexcept>

// cell on the grid
struct Cell {
    char symbol = ' ';
    // std::string fg_color = "\x1b[39m";
    // std::string bg_color = "\x1b[49m";
};

class Canvas {
private:
    int width;
    int height;
    std::vector<Cell> buffer;

    static std::size_t computeCellCount(const int w, const int h) {
        if (w <= 0 || h <= 0) {
            throw std::invalid_argument("Canvas dimensions must be positive");
        }
        const std::size_t uw = static_cast<std::size_t>(w);
        const std::size_t uh = static_cast<std::size_t>(h);
        if (uw > std::numeric_limits<std::size_t>::max() / uh) {
            throw std::invalid_argument("Canvas dimensions too large");
        }
        return uw * uh;
    }

public:
    // init with the variables
    Canvas(const int w, const int h) : width(w), height(h), buffer(computeCellCount(w, h)) {}

    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }

    void clear() {
        for (auto& cell : buffer) {
            cell.symbol = ' ';
        }
    }

    void setChar(int x, int y, char c) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            buffer[y * width + x].symbol = c;
        }
    }

    // Render buffer at once instead of every single char
    void render() {
        std::string frame = "";

        // ANSI sequence: Move cursor to home position (Row 1, Col 1)
        frame += "\x1b[H";

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                frame += buffer[y * width + x].symbol;
            }
            // Add newline for all rows except the final row to prevent screen scrolling
            if (y < height - 1) {
                frame += "\n";
            }
        }

        // Write the full frame string in a single flush
        std::cout << frame << std::flush;
    }

};

#endif // CANVAS_HPP
