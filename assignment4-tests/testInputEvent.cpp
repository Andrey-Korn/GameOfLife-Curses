
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

    ASSERT_FALSE(game.getCurrentField()[1][1].isLife());
    ASSERT_EQ(game.cursorX, 1);
    ASSERT_EQ(game.cursorY, 1);
}

TEST(InputEvent, keyUp){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
}

TEST(InputEvent, keyDown){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
}

TEST(InputEvent, keyLeft){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
}

TEST(InputEvent, keyRight){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
}

TEST(InputEvent, keyEnter){
    // use MockViewHandler and GameManager to start a game instance
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
}