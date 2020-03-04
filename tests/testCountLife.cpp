#include "gtest/gtest.h"

#include "game_field.h"
#include "mockViewHandler.cpp"

TEST(CountLife, CountCircle) {
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
    game.setCellAt(0,0);
    game.setCellAt(0,1);
    game.setCellAt(0,2);
    game.setCellAt(1,0);
    game.setCellAt(1,1);
    game.setCellAt(1,2);
    game.setCellAt(2,0);
    game.setCellAt(2,1);
    game.setCellAt(2,2);
    game.setCellAt(2,2);
    ASSERT_EQ(game.countLifeAround(1, 1), 8);
}

TEST(CountLife, CountNothing) {
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
    game.setCellAt(4, 4);
    ASSERT_EQ(game.countLifeAround(1, 1), 0);
    ASSERT_EQ(game.countLifeAround(0, 0), 0);
    ASSERT_EQ(game.countLifeAround(3, 3), 0);
    ASSERT_EQ(game.countLifeAround(4, 0), 0);
    ASSERT_EQ(game.countLifeAround(0, 4), 0);
}

TEST(CountLife, CountLooping) {
    //  ....#
    //  .....
    //  .....
    //  .....
    //  ....#
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
    game.setCellAt(-1, 0);
    game.setCellAt(-1, -1); // both of these cells are adjacent due to looping rules
    game.setCellAt(3, 3);
    ASSERT_EQ(game.countLifeAround(0, 0), 2);
    ASSERT_EQ(game.countLifeAround(0, 4), 2);
    ASSERT_EQ(game.countLifeAround(4, 0), 1); // on the top right one, so it doesnt count it
    ASSERT_EQ(game.countLifeAround(1, 1), 0);
}

TEST(CountLife, DoesntCountSelf) {
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
    game.setCellAt(1, 1);
    game.setCellAt(3, 3);
    ASSERT_EQ(game.countLifeAround(1, 1), 0);
}

TEST(CountLife, CountCorners) {
    //  .#..#
    //  ##..#
    //  .....
    //  .....
    //  ##..#
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
    game.setCellAt(0, 1);
    game.setCellAt(1, 0);
    game.setCellAt(4, 0);
    game.setCellAt(0, 4);
    game.setCellAt(1, 1);
    game.setCellAt(4, 4);
    game.setCellAt(1, 4);
    game.setCellAt(4, 1);
    game.setCellAt(3, 3);
    ASSERT_EQ(game.countLifeAround(0, 0), 8);
}
