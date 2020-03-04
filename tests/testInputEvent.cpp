
#include "gtest/gtest.h"

#include "mockViewHandler.cpp"

static const int KEY_UP = 259;
static const int KEY_DOWN = 258;
static const int KEY_LEFT = 260;
static const int KEY_RIGHT = 261;
static const int KEY_ENTER = 10;


TEST(InputEvent, onMouseInput){
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
 
    game.onMousePressed(1, 1);
 
    ASSERT_TRUE(game.getCurrentField()[1][1].isLife());
    ASSERT_EQ(game.cursorX, 1);
    ASSERT_EQ(game.cursorY, 1);

    game.onMousePressed(1, 1);

    ASSERT_FALSE(game.getCurrentField()[1][1].isLife());
    ASSERT_EQ(game.cursorX, 1);
    ASSERT_EQ(game.cursorY, 1);
}

TEST(InputEvent, keyEnter){
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 2;
    game.cursorY = 2;

    game.onKeyPressed(KEY_ENTER);
    ASSERT_TRUE(game.getCurrentField()[2][2].isLife());

    game.onKeyPressed(KEY_ENTER);
    ASSERT_FALSE(game.getCurrentField()[2][2].isLife());
}

TEST(InputEvent, keyUp){
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 0;
    game.cursorY = 4;

    game.onKeyPressed(KEY_UP);
    ASSERT_EQ(game.cursorY, 3);
    game.onKeyPressed(KEY_UP);
    ASSERT_EQ(game.cursorY, 2);
    game.onKeyPressed(KEY_UP);
    ASSERT_EQ(game.cursorY, 1);
    game.onKeyPressed(KEY_UP);
    ASSERT_EQ(game.cursorY, 0);
    game.onKeyPressed(KEY_UP);
    ASSERT_EQ(game.cursorY, 0);
}

TEST(InputEvent, keyDown){
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 0;
    game.cursorY = 0;

    game.onKeyPressed(KEY_DOWN);
    ASSERT_EQ(game.cursorY, 1);
    game.onKeyPressed(KEY_DOWN);
    ASSERT_EQ(game.cursorY, 2);
    game.onKeyPressed(KEY_DOWN);
    ASSERT_EQ(game.cursorY, 3);
    game.onKeyPressed(KEY_DOWN);
    ASSERT_EQ(game.cursorY, 4);
    game.onKeyPressed(KEY_DOWN);
    ASSERT_EQ(game.cursorY, 4);
}

TEST(InputEvent, keyLeft){
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 4;
    game.cursorY = 0;

    game.onKeyPressed(KEY_LEFT);
    ASSERT_EQ(game.cursorX, 3);
    game.onKeyPressed(KEY_LEFT);
    ASSERT_EQ(game.cursorX, 2);
    game.onKeyPressed(KEY_LEFT);
    ASSERT_EQ(game.cursorX, 1);
    game.onKeyPressed(KEY_LEFT);
    ASSERT_EQ(game.cursorX, 0);
    game.onKeyPressed(KEY_LEFT);
    ASSERT_EQ(game.cursorX, 0);
}

TEST(InputEvent, keyRight){
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 0;
    game.cursorY = 0;

    game.onKeyPressed(KEY_RIGHT);
    ASSERT_EQ(game.cursorX, 1);
    game.onKeyPressed(KEY_RIGHT);
    ASSERT_EQ(game.cursorX, 2);
    game.onKeyPressed(KEY_RIGHT);
    ASSERT_EQ(game.cursorX, 3);
    game.onKeyPressed(KEY_RIGHT);
    ASSERT_EQ(game.cursorX, 4);
    game.onKeyPressed(KEY_RIGHT);
    ASSERT_EQ(game.cursorX, 4);
}
