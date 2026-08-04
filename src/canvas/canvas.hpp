#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <string>
#include <iostream>

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

public:
    // Initialize the canvas grid with dimensions (width x height)
    Canvas(int w, int h) : width(w), height(h), buffer(w * h) {}

    int getWidth() const { return width; }
    int getHeight() const { return height; }

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

    // Draw a horizontal string starting at coordinates (x, y)
    void drawText(int x, int y, const std::string& text) {
        for (size_t i = 0; i < text.length(); ++i) {
            setChar(x + static_cast<int>(i), y, text[i]);
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
