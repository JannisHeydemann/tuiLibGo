#include <gtest/gtest.h>
#include "../src/canvas/canvas.hpp"
#include "../src/widgets/optionsBox.hpp"
#include <string>
#include <vector>

TEST(OptionsBoxTest, OptionsAreEmptyByDefault) {
    optionsBox ob;
    EXPECT_TRUE(ob.getOptions().empty());
}

TEST(OptionsBoxTest, SetOptionsGetOptionsRoundTrip) {
    optionsBox ob;
    std::vector<std::string> options = {"Alpha", "Beta", "Gamma"};
    ob.setOptions(options);
    EXPECT_EQ(ob.getOptions(), options);
}

TEST(OptionsBoxTest, SetHeightRequiresRoomForBorderPlusEveryOption) {
    optionsBox ob;
    ob.setOptions({"Alpha", "Beta"}); // 2 options -> needs height >= 2 + 2 = 4

    ob.setHeight(4); // exactly enough
    EXPECT_EQ(ob.getHeight(), 4);

    ob.setHeight(3); // one line too short -> rejected, unchanged
    EXPECT_EQ(ob.getHeight(), 4);

    ob.setHeight(10); // plenty of room
    EXPECT_EQ(ob.getHeight(), 10);
}

TEST(OptionsBoxTest, SetHeightStillEnforcesWidgetMinimumOfThree) {
    optionsBox ob;
    // With no options, optionsBox's own guard only requires height >= 2,
    // but it still delegates to Widget::setHeight(), which requires > 2.
    ob.setHeight(5);
    ASSERT_EQ(ob.getHeight(), 5);

    ob.setHeight(2); // passes optionsBox's check (2 >= 2 + 0) but fails Widget's (2 > 2 is false)
    EXPECT_EQ(ob.getHeight(), 5) << "Widget's minimum-height guard should still apply";
}

// KNOWN BUG: optionsBox::draw() (src/widgets/optionsBox.cpp) is a copy of
// Box::draw() — it never reads the `options` member, so the option list is
// never rendered. This test documents the intended behavior and is expected
// to fail until optionsBox::draw() is fixed to actually draw its options.
TEST(OptionsBoxTest, DrawRendersEachOption) {
    optionsBox ob;
    ob.setXCoords(0);
    ob.setYCoords(0);
    ob.setWidth(10);
    ob.setOptions({"One", "Two"});
    ob.setHeight(5);
    ob.setBorder('#');

    Canvas c(10, 5);
    ob.draw(c);

    testing::internal::CaptureStdout();
    c.render();
    std::string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("One"), std::string::npos)
        << "BUG: optionsBox::draw() never renders its options list";
    EXPECT_NE(out.find("Two"), std::string::npos)
        << "BUG: optionsBox::draw() never renders its options list";
}
