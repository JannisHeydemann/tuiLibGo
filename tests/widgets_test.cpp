#include <gtest/gtest.h>
#include "../src/canvas/canvas.hpp"
#include "../src/widgets/widgets.hpp"
#include "../src/widgets/box.hpp"
#include <string>
#include <vector>

namespace {

// Widget is abstract and drawBorder()/drawText() are protected; this
// subclass gives the tests direct access so the border/text drawing logic
// can be verified independently of any concrete widget (e.g. Box).
class ExposedWidget : public Widget {
public:
    void draw(Canvas&) const override {}
    void callDrawBorder(Canvas& c, char b) const { drawBorder(c, b); }
    void callDrawText(Canvas& c, int x, int y, std::string text) const { drawText(c, x, y, text); }
};

std::string renderToString(Canvas& c) {
    testing::internal::CaptureStdout();
    c.render();
    return testing::internal::GetCapturedStdout();
}

} // namespace

// ---------------------------------------------------------------------
// Widget getters/setters
// ---------------------------------------------------------------------

TEST(WidgetTest, XAndYCoordsRoundTrip) {
    ExposedWidget w;
    w.setXCoords(5);
    w.setYCoords(-2); // no validation on coordinates, negative is allowed
    EXPECT_EQ(w.getXCoords(), 5);
    EXPECT_EQ(w.getYCoords(), -2);
}

TEST(WidgetTest, SetWidthAcceptsValuesGreaterThanTwo) {
    ExposedWidget w;
    w.setWidth(3);
    EXPECT_EQ(w.getWidth(), 3);
    w.setWidth(100);
    EXPECT_EQ(w.getWidth(), 100);
}

TEST(WidgetTest, SetWidthRejectsValuesOfTwoOrLess) {
    ExposedWidget w;
    w.setWidth(10);
    w.setWidth(2);
    EXPECT_EQ(w.getWidth(), 10) << "width must stay at the last valid value";
    w.setWidth(0);
    EXPECT_EQ(w.getWidth(), 10);
    w.setWidth(-5);
    EXPECT_EQ(w.getWidth(), 10);
}

TEST(WidgetTest, SetHeightAcceptsValuesGreaterThanTwo) {
    ExposedWidget w;
    w.setHeight(3);
    EXPECT_EQ(w.getHeight(), 3);
    w.setHeight(50);
    EXPECT_EQ(w.getHeight(), 50);
}

TEST(WidgetTest, SetHeightRejectsValuesOfTwoOrLess) {
    ExposedWidget w;
    w.setHeight(10);
    w.setHeight(2);
    EXPECT_EQ(w.getHeight(), 10) << "height must stay at the last valid value";
    w.setHeight(0);
    EXPECT_EQ(w.getHeight(), 10);
    w.setHeight(-5);
    EXPECT_EQ(w.getHeight(), 10);
}

// ---------------------------------------------------------------------
// Widget::drawBorder
// ---------------------------------------------------------------------

TEST(WidgetDrawBorderTest, DrawsFullOutlineWhenWidgetFillsCanvas) {
    Canvas c(5, 4);
    ExposedWidget w;
    w.setXCoords(0);
    w.setYCoords(0);
    w.setWidth(5);
    w.setHeight(4);
    w.callDrawBorder(c, '*');
    EXPECT_EQ(renderToString(c), "\x1b[H*****\n*   *\n*   *\n*****");
}

TEST(WidgetDrawBorderTest, RespectsOffsetPositionWithinLargerCanvas) {
    Canvas c(6, 5);
    ExposedWidget w;
    w.setXCoords(1);
    w.setYCoords(1);
    w.setWidth(4);
    w.setHeight(3);
    w.callDrawBorder(c, '#');

    std::vector<std::string> rows(5, std::string(6, ' '));
    rows[1] = " #### ";
    rows[2] = " #  # ";
    rows[3] = " #### ";
    std::string expected = "\x1b[H";
    for (size_t i = 0; i < rows.size(); ++i) {
        expected += rows[i];
        if (i + 1 < rows.size()) expected += "\n";
    }

    EXPECT_EQ(renderToString(c), expected);
}

// ---------------------------------------------------------------------
// Widget::drawText
// ---------------------------------------------------------------------

TEST(WidgetDrawTextTest, DrawsStringLeftToRightAtGivenPosition) {
    Canvas c(6, 1);
    ExposedWidget w;
    w.callDrawText(c, 1, 0, "Hi!");
    EXPECT_EQ(renderToString(c), "\x1b[H Hi!  ");
}

TEST(WidgetDrawTextTest, ClipsSilentlyWhenTextExtendsPastCanvasEdge) {
    Canvas c(3, 1);
    ExposedWidget w;
    w.callDrawText(c, 0, 0, "Hello");
    EXPECT_EQ(renderToString(c), "\x1b[HHel");
}

TEST(WidgetDrawTextTest, EmptyStringDrawsNothing) {
    Canvas c(3, 1);
    ExposedWidget w;
    w.callDrawText(c, 0, 0, "");
    EXPECT_EQ(renderToString(c), "\x1b[H   ");
}

// ---------------------------------------------------------------------
// Box
// ---------------------------------------------------------------------

TEST(BoxTest, BorderRoundTrips) {
    Box b;
    b.setBorder('%');
    EXPECT_EQ(b.getBorder(), '%');
}

TEST(BoxTest, TitleIsEmptyByDefault) {
    Box b;
    EXPECT_EQ(b.getTitle(), "");
}

TEST(BoxTest, SetTitleAcceptsStringsThatFitTheWidth) {
    Box b;
    b.setWidth(8); // "Hi" (len 2) needs width > 2 + 2 = 4
    b.setTitle("Hi");
    EXPECT_EQ(b.getTitle(), "Hi");
}

TEST(BoxTest, SetTitleRejectsStringsThatDoNotLeaveRoomForBorder) {
    Box b;
    b.setWidth(7); // "Hello" (len 5) needs width > 5 + 2 = 7; 7 is not > 7
    b.setTitle("Hello");
    EXPECT_EQ(b.getTitle(), "") << "title must be rejected, not truncated";

    b.setWidth(8); // now width (8) > 7, so it fits
    b.setTitle("Hello");
    EXPECT_EQ(b.getTitle(), "Hello");
}

TEST(BoxTest, DrawRendersBorderAndCenteredTitle) {
    Box b;
    b.setXCoords(0);
    b.setYCoords(0);
    b.setWidth(8);
    b.setHeight(4);
    b.setBorder('#');
    b.setTitle("Hi");

    Canvas c(8, 4);
    b.draw(c);

    EXPECT_EQ(renderToString(c), "\x1b[H########\n#  Hi  #\n#      #\n########");
}
