#include "gtest/gtest.h"

#include "game_field.h"
#include "mockViewHandler.cpp"

TEST(SplitArgsExecuteCommand, NoArgs) {
  MockViewHandler viewhandler;
  GameManager game(25, 25, viewhandler);

  std::string cmd = "reset";
  std::vector<std::string> cmdSplit = splitString(cmd);
  std::ostringstream stream;
  game.executeCommand(cmdSplit[0], std::vector<std::string>(cmdSplit.begin() + 1, cmdSplit.end()), stream);
//   ASSERT_EQ(stream.str(), "test");
  ASSERT_EQ(game.getWidth(), 25);
  ASSERT_EQ(game.getHeight(), 25);
}

TEST(SplitArgsExecuteCommand, ResetArgs) {
  MockViewHandler viewhandler;
  GameManager game(25, 25, viewhandler);

  std::string cmd = "reset 10 10";
  std::vector<std::string> cmdSplit = splitString(cmd);
  std::ostringstream stream;
  game.executeCommand(cmdSplit[0], std::vector<std::string>(cmdSplit.begin() + 1, cmdSplit.end()), stream);
  ASSERT_EQ(stream.str(), "Field reset to size 10x10\n");
  ASSERT_EQ(game.getWidth(), 10);
  ASSERT_EQ(game.getHeight(), 10);
}

TEST(SplitArgsExecuteCommand, StepArg) {
  MockViewHandler viewhandler;
  GameManager game(25, 25, viewhandler);

  std::string cmd = "step 5";
  std::vector<std::string> cmdSplit = splitString(cmd);
  std::ostringstream stream;
  game.executeCommand(cmdSplit[0], std::vector<std::string>(cmdSplit.begin() + 1, cmdSplit.end()), stream);
  ASSERT_EQ(stream.str(), "Done 5 step(s).\n");
}

TEST(SplitArgsExecuteCommand, ResetWrongArg) {
  MockViewHandler viewhandler;
  GameManager game(10, 10, viewhandler);

  std::string cmd = "reset j";
  std::vector<std::string> cmdSplit = splitString(cmd);
  std::ostringstream stream;
  game.executeCommand(cmdSplit[0], std::vector<std::string>(cmdSplit.begin() + 1, cmdSplit.end()), stream);
  ASSERT_EQ(stream.str(), "Invalid arguments\n");
}

TEST(SplitArgsExecuteCommand, SetWrongArgs) {
  MockViewHandler viewhandler;
  GameManager game(25, 25, viewhandler);
  std::string cmds[4] = {"set j 5", "set 5 j", "set j j", "set 5 5"};

  for(int i = 0; i < 4; i++) {
    std::vector<std::string> cmdSplit = splitString(cmds[i]);
    std::ostringstream stream;
    game.executeCommand(cmdSplit[0], std::vector<std::string>(cmdSplit.begin() + 1, cmdSplit.end()), stream);
  }

  ASSERT_FALSE(game.getCurrentField()[0][0].isLife());
  ASSERT_FALSE(game.getCurrentField()[0][5].isLife());
  ASSERT_FALSE(game.getCurrentField()[5][0].isLife());
  ASSERT_TRUE(game.getCurrentField()[5][5].isLife());
}

TEST(SplitArgsExecuteCommand, InvalidCmd) {
  MockViewHandler viewhandler;
  GameManager game(25, 25, viewhandler);

  std::string cmd = "invalidCmd test";
  std::vector<std::string> cmdSplit = splitString(cmd);
  std::ostringstream stream;
  ASSERT_FALSE(game.executeCommand(cmdSplit[0], std::vector<std::string>(cmdSplit.begin() + 1, cmdSplit.end()), stream));
}

TEST(SplitArgsExecuteCommand, StepWrongArgsNoThrow) {
  MockViewHandler viewhandler;
  GameManager game(25, 25, viewhandler);
  std::string cmd = "step j";

  std::vector<std::string> cmdSplit = splitString(cmd);
  std::ostringstream stream;

  ASSERT_NO_THROW(game.executeCommand(cmdSplit[0], std::vector<std::string>(cmdSplit.begin() + 1, cmdSplit.end()), stream));
}
