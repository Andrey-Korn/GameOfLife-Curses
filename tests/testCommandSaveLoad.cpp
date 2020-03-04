#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "mockViewHandler.cpp"

TEST(SaveLoad, loadBad) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {"doesntExist.fld"};
  
  std::ostringstream stream;
  commandLoad(args, game, stream);
  
  ASSERT_EQ(stream.str(), "Cannot load file \"doesntExist.fld\"\n");
}

TEST(SaveLoad, createTest) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {"testFile1.fld"};
  
  std::ostringstream stream;
  commandSave(args, game, stream);
  
  ASSERT_EQ(stream.str(), "Game field saved to \"testFile1.fld\".\n");
}

TEST(SaveLoad, loadTest) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {"testFile1.fld"};
  
  std::ostringstream stream;
  commandLoad(args, game, stream);
  
  ASSERT_EQ(stream.str(), "Game \"testFile1.fld\" loaded successfully.\n");
}

TEST(SaveLoad, createEmptyFile) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {};             //array empty?
  
  std::ostringstream stream;
  commandSave(args, game, stream);
  
  ASSERT_EQ(stream.str(), "Game field saved to \"game_of_life.fld\".\n");
}

TEST(SaveLoad, loadEmptyFile) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {};             //array empty?
  
  std::ostringstream stream;
  commandLoad(args, game, stream);
  
  ASSERT_EQ(stream.str(), "Game \"game_of_life.fld\" loaded successfully.\n");
}
