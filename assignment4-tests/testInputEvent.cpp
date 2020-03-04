
#include "gtest/gtest.h"

#include "mockViewHandler.cpp"

static const int KEY_UP = 259;
static const int KEY_DOWN = 258;
static const int KEY_LEFT = 260;
static const int KEY_RIGHT = 261;
static const int KEY_ENTER = 10;


TEST(InputEvent, onMouseInput){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    // mouse press at (1,1) 
    game.onMousePressed(1, 1);

    // check cell and cursor state
    ASSERT_TRUE(game.getCurrentField()[1][1].isLife());
    ASSERT_EQ(game.cursorX, 1);
    ASSERT_EQ(game.cursorY, 1);

    // mouse press at (1,1) 
    game.onMousePressed(1, 1);

    // check cell again
    ASSERT_FALSE(game.getCurrentField()[1][1].isLife());
    ASSERT_EQ(game.cursorX, 1);
    ASSERT_EQ(game.cursorY, 1);
}

TEST(InputEvent, keyEnter){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    // place cursor at (2,2)
    game.cursorX = 2;
    game.cursorY = 2;

    // hit enter twice, making sure the cell gets set and reset
    game.onKeyPressed(KEY_ENTER);
    ASSERT_TRUE(game.getCurrentField()[2][2].isLife());

    game.onKeyPressed(KEY_ENTER);
    ASSERT_FALSE(game.getCurrentField()[2][2].isLife());
}

TEST(InputEvent, keyUp){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 0;
    game.cursorY = 4;

    // cursor should stop once it gets to the top edge of the field
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
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 0;
    game.cursorY = 0;

    // cursor should stop once it gets to the bottom edge of the field
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
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 4;
    game.cursorY = 0;

    // cursor should stop once it gets to the left edge of the field
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
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);

    game.cursorX = 0;
    game.cursorY = 0;

    // cursor should stop once it gets to the right edge of the field
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
