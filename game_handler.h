//
//  game_handler.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef GAME_HANDLER_H
#define GAME_HANDLER_H

#include <cstdint>
#include <map>
#include <ostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "game_field.h"

static const int MAX_BACKSTEP = 100;

class InputResult {
 public:
  /**
   * Event is thrown by timeout.
   */
  InputResult() : timedout(true) {}

  /**
   * Event is thrown by mouse click.
   */
  InputResult(size_t posX, size_t posY)
      : timedout(false), keyboard(false), posX(posX), posY(posY) {}

  /**
   * Event is thrown by key pressed.
   */
  InputResult(int key) : timedout(false), keyboard(true), key(key) {}

  int getKey() const { return isKeyboard() ? key : 0; }

  size_t getPosX() const { return isKeyboard() ? 0 : posX; }

  size_t getPosY() const { return isKeyboard() ? 0 : posY; }

  bool isTimedOut() const { return timedout; }

  bool isKeyboard() const { return keyboard && !timedout; }

  InputResult& operator=(const InputResult& copy) {
    if (&copy != this) {
      posX = copy.posX;
      posY = copy.posY;
      keyboard = copy.keyboard;
      timedout = copy.timedout;
      key = copy.key;
    }
    return (*this);
  }

 private:
  // Mouse click position
  size_t posX, posY;

  // If this is keyboard event
  bool keyboard;

  // If this is timeout event
  bool timedout;

  // Key code
  int key;
};

class ViewHandler {
 public:
  /**
   * Draws field and prompts.
   */
  virtual void updateField(const GameField& field, size_t stepsCount) = 0;

  /**
   * Draws keyboard cursor on field.
   */
  virtual void updateKeyboardCursor(size_t posX, size_t posY) = 0;

  /**
   * Draws output from commands.
   */
  virtual void updateCommandLine(const std::string& commandOutput) = 0;

  /**
   * Reads command input from user.
   *
   * @return User input.
   */
  virtual std::string readCommandInput() = 0;

  /**
   * Waits for the key press/mouse click/timeout expiration.
   *
   * @param timeout Input timeout in tenths of a second. From 0 to 255.
   *
   * @return Wait result.
   */
  virtual const InputResult waitForInput(uint8_t timeout) = 0;

  /**
   * Checks whether it is possible to create a field with the given dimensions
   * on this terminal.
   */
  virtual bool canCrateFieldWithSizes(size_t width, size_t height) = 0;
};


class GameManager {
 public:
  size_t cursorX = 0;
  size_t cursorY = 0;

  GameManager(size_t width, size_t height, ViewHandler& viewHandler);

  GameManager(const GameField& field, ViewHandler& viewHandler)
      : width(field.getWidth()),
        height(field.getHeight()),
        gameField(field),
        previousStep(GameField()),
        viewHandler(viewHandler) {
          previousStepArr[0] = GameField();
        }

  int runGame();

  void nextStep();

  /**
   * If life existed at that position, it dies.
   * If there was no life, it borns.
   */
  bool setCellAt(int posX, int posY);

   /**
   * Clears the field, resets the steps counter and creates a field with new
   * dimensions.
   */
  void reset(const GameField& field);
  void reset(size_t width = 0, size_t height = 0);

  /**
   * Cancels last step.
   * You can cancel only one step.
   *
   * @return true, if step succesfully cancelled.
   */
  bool stepBack();

  /**
   * Registers the function of the command handler.
   * @param name Command name.
   * @param cmd Command handler.
   */
  void registerCommand(const std::string& name,
                       void (*cmd)(const std::vector<std::string>&,
                                   GameManager&,
                                   std::ostream&));
  
  /**
   * Execute command by name.
   * @param name Command name.
   * @param args Command arguments.
   *
   * @return true, if command successfully executed.
   */
  bool executeCommand(const std::string& name,
                      const std::vector<std::string>& args,
                      std::ostream& output);

  /**
   * Counts the number of living cells around this cell.
   *
   * @return Number of living cells around
   */
  size_t countLifeAround(int posX, int posY) const;

  /**
   * Checks whether it is possible to create a field with the given dimensions
   * on this terminal.
   */
  bool canCreateFieldWithSizes(size_t fieldWidth, size_t fieldHeight) const;

  const GameField getCurrentField() const;

  size_t getWidth() const;

  size_t getHeight() const;

  ViewHandler& getViewHandler();

  /**
   * Catch mouse clicks.
   */
  void onMousePressed(int x, int y);

  /**
   * Catch key presses.
   */
  void onKeyPressed(int key);


 private:
  size_t width;
  size_t height;

  std::map<
      std::string,
      void (*)(const std::vector<std::string>&, GameManager&, std::ostream&)>
      commands;

  GameField gameField;
  GameField previousStep;
  GameField previousStepArr[MAX_BACKSTEP];

  ViewHandler& viewHandler;

  size_t prevCounter = 0;
  size_t stepsCounter = 0;
  size_t minPrevIdx = 0;
  bool hasUndo = false;  // Is it possible to cancel at this step

  // Keyboard cursor on field position
  // size_t cursorX = 0;
  // size_t cursorY = 0;

  /**
   * Forces the update view handler without making any changes to the state of
   * the field.
   */
  void update();

  /**
   * Activates the command mode and waits for one command to be entered.
   * After the execution of the command goes into normal mode.
   */
  void executionInCommandMode();

  /**
   * Catch mouse clicks.
   */
  // void onMousePressed(int x, int y);

  /**
   * Catch key presses.
   */
  // void onKeyPressed(int key);

  /**
   * Manages the cursor to set the cells on the field using the arrows on the
   * keyboard.
   */
  void onKeyboardCursor(int key);
};

// Key codes
static const int KEY_N = 110;
static const int KEY_B = 98;
static const int KEY_R = 114;
static const int KEY_C = 99;
static const int KEY_I = 105;
static const int KEY_Q = 113;

static const std::string DEFAULT_SAVE_FILENAME = "game_of_life.fld";

// Delay in tenths of a second when the next step is updated (for multi steps).
static const size_t STEP_UPDATE_DELAY = 1;

// Number of living cells around the dead for the birth of life.
static const size_t BORN_LIFE = 3;

// Minimum number of living cells around the living to continue life.
static const size_t DEATH_LONELINESS = 2;

// Maximum number of living cells around the living to continue life.
static const size_t DEATH_OVERPOPULATION = 3;

// STATIC METHODS
// ==================== Command handlers ====================

/**
 * Clears the field and resets the steps counter.
 * If you pass width and height arguments, it creates a field with given width
 * and height. Arguments: [width] [height]
 */
static void commandReset(const std::vector<std::string>& args,
                         GameManager& game,
                         std::ostream& out) {
  if (args.size() > 2 || args.size() == 0) {
    if (game.canCreateFieldWithSizes(game.getWidth(), game.getHeight()))
    {
      game.reset(game.getWidth(), game.getHeight());
    }
    else
    {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
  }
  else
  {
    int width;
    int height;
    try {
      if (args.size() == 1) {
        width = stoi(args[0]);
        height = stoi(args[0]);
      }
      else {
        width = stoi(args[0]);
        height = stoi(args[1]);
      }
    } catch(std::invalid_argument& e) {
      out << "Invalid arguments" << std::endl;
      return;
    }

    if (game.canCreateFieldWithSizes(width, height))
      game.reset(width, height);
    else {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
  }
  out << "Field reset to size " << game.getWidth() << "x" << game.getHeight()
      << std::endl;
}

/**
 * Sets or removes life in a cell.
 * Arguments: <pos X> <pos Y>
 */
static void commandSet(const std::vector<std::string>& args,
                       GameManager& game,
                       std::ostream& out) {
  if (args.size() != 2) {
    out << "Need args: <pos X> <pos Y>" << std::endl;
    return;
  }
  try {
    int posX = stoi(args[0]);
    int posY = stoi(args[1]);
    out << "Cell " << (game.setCellAt(posX, posY) ? "spawned." : "killed.") << std::endl;
  } catch(std::invalid_argument& e) {
    out << "Need args: <pos X> <pos Y>" << std::endl;
  }
}

/**
 * Performs the specified number of steps. If there is no argument, it performs
 * 1 step. If the argument is '-', performs an infinite number of steps, until
 * the key 'I' is pressed. Arguments: [steps count or '-']
 */
static void commandStep(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  size_t steps = 1;
  bool isInfinity = false;
  if (args.size() > 0) {
    if (args[0] == "-")
      isInfinity = true;
    else
      try {
        steps = stoi(args[0]);
      } catch(std::invalid_argument& e) {
        out << "Invalid Argument" << std::endl;
        return;
      }
  }

  game.getViewHandler().updateCommandLine(
      "Making steps... Press I for interrupt.");

  size_t counter = 0;
  for (; counter < steps || isInfinity; counter++) {
    game.nextStep();
    InputResult result = game.getViewHandler().waitForInput(STEP_UPDATE_DELAY);
    if (result.isKeyboard() && result.getKey() == KEY_I)
      break;
  }

  out << "Done " << counter << " step(s)." << std::endl;
}

/**
 * Cancels the last step.
 * You can not undo more than one step.
 */
static void commandBack(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  if (game.stepBack())
    out << "Back step completed." << std::endl;
  else
    out << "It's impossible to step back." << std::endl;
}

/**
 * Saves field to file.
 * Arguments: [filename]
 */
static void commandSave(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  std::string filename(DEFAULT_SAVE_FILENAME);
  if (args.size() > 0)
    filename = args[0];

  std::ofstream file(filename);
  if (!file.is_open()) {
    out << "Cannot create file \"" << filename << "\"." << std::endl;
    return;
  }

  file << game.getCurrentField() << std::endl;
  file.close();

  out << "Game field saved to \"" << filename << "\"." << std::endl;
}

/**
 * Loads field from file.
 * Arguments: [filename]
 */
static void commandLoad(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  std::string filename = DEFAULT_SAVE_FILENAME;
  if (args.size() > 0)
    filename = args[0];

  std::ifstream file(filename);
  if (!file.is_open()) {
    out << "Cannot load file \"" << filename << "\"" << std::endl;
    return;
  }

  std::ostringstream fileContent;
  std::string line;
  while (std::getline(file, line))
    fileContent << line << std::endl;
  file.close();

  try {
    GameField field(fileContent.str());
    if (!game.canCreateFieldWithSizes(field.getWidth(), field.getHeight())) {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
    game.reset(field);
  } catch (const BadGameFieldException& e) {
    out << "Cannot parse field: " << e.what() << std::endl;
    return;
  }

  out << "Game \"" << filename << "\" loaded successfully." << std::endl;
}

static std::vector<std::string> splitString(const std::string& str) {
  std::istringstream input(str);
  std::string item;
  std::vector<std::string> items;
  while (std::getline(input, item, ' '))
    items.push_back(item);
  return items;
}

#endif /* GAME_HANDLER_H */
