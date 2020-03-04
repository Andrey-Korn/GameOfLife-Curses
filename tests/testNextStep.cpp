#include "gtest/gtest.h"

#include "mockViewHandler.cpp"

TEST(NextStep, FillField) {
    //  ##.      ###
    //  #..  ->  ###  in one step
    //  ...      ###
    MockViewHandler mockViewHandler;
    GameField field("##.\n#..\n...");
    GameManager game(field, mockViewHandler);
    game.nextStep();
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            ASSERT_TRUE(game.getCurrentField()[i][j].isLife());
        }
    }
}

TEST(NextStep, FullFieldClear) {
    //  ###      ...
    //  ###  ->  ...  in one step
    //  ###      ...
    MockViewHandler mockViewHandler;
    GameField field("###\n###\n###");
    GameManager game(field, mockViewHandler);
    game.nextStep();
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            ASSERT_FALSE(game.getCurrentField()[i][j].isLife());
        }
    }
}

TEST(NextStep, Oscillator) {
    //  .#.      ...     .#. 
    //  .#.  ->  ### ->  .#.  oscillating
    //  .#.      ...     .#. 
    MockViewHandler mockViewHandler;
    GameField field(".....\n..#..\n..#..\n..#..\n.....");
    GameManager game(field, mockViewHandler);
    game.nextStep();
    for(int i = 1; i < 4; i++) {
        ASSERT_FALSE(game.getCurrentField()[1][i].isLife());
        ASSERT_TRUE(game.getCurrentField()[2][i].isLife());
        ASSERT_FALSE(game.getCurrentField()[3][i].isLife());
    }
    game.nextStep();
    for(int i = 1; i < 4; i++) {
        ASSERT_FALSE(game.getCurrentField()[i][1].isLife());
        ASSERT_TRUE(game.getCurrentField()[i][2].isLife());
        ASSERT_FALSE(game.getCurrentField()[i][3].isLife());
    }
}

TEST(NextStep, blankField) {
    MockViewHandler mockViewHandler;
    GameField field("...\n...\n...");
    GameManager game(field, mockViewHandler);
    for(int i = 0; i < 10; i++) {
        game.nextStep();
    }
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            ASSERT_FALSE(game.getCurrentField()[i][j].isLife());
        }
    }
}

TEST(NextStep, PulsarOscillator) {
    MockViewHandler mockViewHandler;
    GameField field(".................\n"
                    ".................\n"
                    "....###...###....\n"
                    ".................\n"
                    "..#....#.#....#..\n"
                    "..#....#.#....#..\n"
                    "..#....#.#....#..\n"
                    "....###...###....\n"
                    ".................\n"
                    "....###...###....\n"
                    "..#....#.#....#..\n"
                    "..#....#.#....#..\n"
                    "..#....#.#....#..\n"
                    ".................\n"
                    "....###...###....\n"
                    ".................\n"
                    ".................\n");
    GameManager game(field, mockViewHandler);
    for(int i = 0; i < 3; i++) {
        game.nextStep();
    }
    GameField checkingField(".................\n"
                            ".................\n"
                            "....###...###....\n"
                            ".................\n"
                            "..#....#.#....#..\n"
                            "..#....#.#....#..\n"
                            "..#....#.#....#..\n"
                            "....###...###....\n"
                            ".................\n"
                            "....###...###....\n"
                            "..#....#.#....#..\n"
                            "..#....#.#....#..\n"
                            "..#....#.#....#..\n"
                            ".................\n"
                            "....###...###....\n"
                            ".................\n"
                            ".................\n");
    ASSERT_EQ(checkingField, game.getCurrentField());


}