#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <cstddef>
#include <limits>
#include <stdexcept>

// Represents an individual character on the terminal grid
struct Cell {
    char symbol = ' ';
    // Note: You can easily add ANSI color codes here later!
    // std::string fg_color = "\x1b[39m";
    // std::string bg_color = "\x1b[49m";
};

class Canvas {
private:
    int width;
    int height;
    std::vector<Cell> buffer;

    // Rejects nonpositive dimensions and overflowing products before
    // the vector allocation below ever sees them.
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
    // Initialize the canvas grid with dimensions (width x height)
    Canvas(const int w, const int h) : width(w), height(h), buffer(computeCellCount(w, h)) {}

    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }

    // Clear the back-buffer grid to blank spaces
    void clear() {
        for (auto& cell : buffer) {
            cell.symbol = ' ';
        }
    }

    // Set a single character at grid coordinates (x, y) with bounds checking
    void setChar(int x, int y, char c) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            buffer[y * width + x].symbol = c;
        }
    }

    // Render the entire grid to stdout in one atomic write call to prevent screen flickering
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
