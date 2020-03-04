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
