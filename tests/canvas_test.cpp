#include <gtest/gtest.h>
#include "../src/canvas/canvas.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>

TEST(CanvasTest, ConstructorThrowsOnNonPositiveWidth) {
    EXPECT_THROW(Canvas(0, 5), std::invalid_argument);
    EXPECT_THROW(Canvas(-3, 5), std::invalid_argument);
}

TEST(CanvasTest, ConstructorThrowsOnNonPositiveHeight) {
    EXPECT_THROW(Canvas(5, 0), std::invalid_argument);
    EXPECT_THROW(Canvas(5, -3), std::invalid_argument);
}

TEST(CanvasTest, GetWidthAndHeightReflectConstructorArgs) {
    Canvas c(7, 3);
    EXPECT_EQ(c.getWidth(), 7);
    EXPECT_EQ(c.getHeight(), 3);
}

TEST(CanvasTest, NewCanvasRendersAsAllBlanks) {
    Canvas c(3, 2);
    testing::internal::CaptureStdout();
    c.render();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\x1b[H   \n   ");
}

TEST(CanvasTest, SetCharWritesAtGivenCoordinates) {
    Canvas c(3, 2);
    c.setChar(1, 0, 'X');
    c.setChar(0, 1, 'Y');
    testing::internal::CaptureStdout();
    c.render();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\x1b[H X \nY  ");
}

TEST(CanvasTest, SetCharIgnoresNegativeCoordinates) {
    Canvas c(2, 2);
    c.setChar(-1, 0, 'X');
    c.setChar(0, -1, 'X');
    testing::internal::CaptureStdout();
    c.render();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\x1b[H  \n  ");
}

TEST(CanvasTest, SetCharIgnoresCoordinatesAtOrPastEdge) {
    Canvas c(2, 2);
    c.setChar(2, 0, 'X'); // x == width
    c.setChar(0, 2, 'X'); // y == height
    testing::internal::CaptureStdout();
    c.render();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\x1b[H  \n  ");
}

TEST(CanvasTest, ClearResetsEveryCellToSpace) {
    Canvas c(2, 2);
    c.setChar(0, 0, 'A');
    c.setChar(1, 1, 'B');
    c.clear();
    testing::internal::CaptureStdout();
    c.render();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\x1b[H  \n  ");
}

TEST(CanvasTest, RenderJoinsRowsWithNewlinesButNoTrailingNewline) {
    Canvas c(2, 3);
    testing::internal::CaptureStdout();
    c.render();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(std::count(out.begin(), out.end(), '\n'), 2); // 3 rows -> 2 separators
    EXPECT_NE(out.back(), '\n');
}
