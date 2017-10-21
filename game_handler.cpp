//
//  game_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <cstdlib>
#include <ncurses.h>
#include <sstream>
#include <chrono>
#include <thread>

#include "game_handler.h"

// Коды клавиш
const int KEY_Q = 113;
const int KEY_N = 110;
const int KEY_B = 98;
const int KEY_R = 114;
const int KEY_C = 99;

// Задержка при обновлении следующего шага(для множественных шагов)
const size_t STEP_UPDATE_DELAY = 100;

// Максимальная длина команды в командном режиме
const size_t MAX_COMMAND_LEN = 50;

// Количество живых клеток вокруг мертвой для зарождения жизни.
const size_t BORN_LIFE = 3;

// Минимальное количество живых клеток вокруг живой для продолжения жизни.
const size_t DEATH_LONELINESS = 2;

// Максимальное количесвто живых клеток вокруг живой для продолжения жизни.
const size_t DEATH_OVERPOPULATION = 3;

size_t GameManager::getMaxPromptWidth() const {
    size_t max = 0;
    for (std::string prompt : PROMPTS)
        if (prompt.size() > max)
            max = prompt.size();
    return max + 2; // К длине добавляются 2 пробела, обрамляющие символ горячей клавиши.
}

bool GameManager::canCreateFieldWithSizes(size_t fieldWidth, size_t fieldHeight) const {
    size_t maxWidth, maxHeight;
    getmaxyx(stdscr, maxHeight, maxWidth);
    size_t width = fieldWidth + getMaxPromptWidth();
    size_t promptsHeight = PROMPTS.size() + 2; // Смещение для отображения количества шагов
    size_t height = fieldHeight > promptsHeight ? fieldHeight : promptsHeight;
    height += 2; // Смещение для командного режима
    return maxWidth >= width && maxHeight >= height;
}

// ==================== Обработчики команд ====================

/**
 * Очищает поле и сбрасывает счетчик команд.
 * */
static void commandReset(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    if (args.size() != 2) {
        if (game.canCreateFieldWithSizes(game.getCurrentField().getWidth(),
                                    game.getCurrentField().getHeight()))
            game.reset(game.getCurrentField().getWidth(), game.getCurrentField().getHeight());
        else {
            out << "Cannot place game field on this terminal size." << std::endl;
            return;
        }
    }
    else {
        int width = atoi(args[0].c_str());
        int height = atoi(args[1].c_str());
        if (game.canCreateFieldWithSizes(width, height))
            game.reset(width, height);
        else {
            out << "Cannot place game field on this terminal size." << std::endl;
            return;
        }
    }
    out << "Field reseted to size (" << game.getCurrentField().getWidth() << ", " <<
    game.getCurrentField().getHeight() << ")." << std::endl;
}

/**
 * Устанавливает или удаляет организм в клетку.
 * Аргументы: <позиция X> <позиция Y>
 * */
static void commandSet(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    if (args.size() != 2) {
        out << "Need args: <pos X> <pos Y>" << std::endl;
        return;
    }
    int posX = atoi(args[0].c_str());
    int posY = atoi(args[1].c_str());
    
    out << "Cell " << (game.setCellAt(posX, posY) ? "killed." : "spawned.") << std::endl;
}

/**
 * Выполняет переданное кол-во шагов. Если аргумент отсутствует, выполняет 1 шаг.
 * Аргументы: [кол-во шагов]
 * */
static void commandStep(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    size_t steps = 1;
    if (args.size() > 0)
        steps = atoi(args[0].c_str());
    
    for (size_t i = 0; i < steps; i++) {
        game.nextStep();
        std::this_thread::sleep_for(std::chrono::milliseconds(STEP_UPDATE_DELAY));
    }
    
    out << "Steps done." << std::endl;
}

/**
 * Отменяет последний шаг.
 * Невозможно отменить более одного шага.
 * */
static void commandBack(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    game.stepBack();
    out << "Back step completed." << std::endl;
}

/**
 * Сохраняет состояние поля в файл.
 * Аргументы: <имя файла>
 * */
static void commandSave(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    // TODO - сохранение в файл
    out << "Not yet implemented." << std::endl;
}

/**
 * Загружает состояние поля из файла.
 * Аргументы: <имя файла>
 * */
static void commandLoad(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    // TODO - загрузка из файла
    out << "Not yet implemented." << std::endl;
}

GameManager::GameManager(size_t width, size_t height, FieldUpdateListener& listener) :
    width(width), height(height),
    gameField(GameField(width, height)),
    updateListener(listener),
    previousStep(GameField(0, 0))
{
    registerCommand("reset", &commandReset);
    registerCommand("set", &commandSet);
    registerCommand("step", &commandStep);
    registerCommand("back", &commandBack);
    registerCommand("save", &commandSave);
    registerCommand("load", &commandLoad);
}

size_t GameManager::countLifeAround(int posX, int posY) const {
    //   1 2 3
    // 1 # # #  }
    // 2 # . #  }- Точка это проверяемая ячейка
    // 3 # # #  }
    
    size_t lifes = 0; // Количество живых клеток вокруг
    
    lifes += previousStep[posX - 1][posY].isLife() ? 1 : 0; // 2-1
    lifes += previousStep[posX + 1][posY].isLife() ? 1 : 0; // 2-3
    
    // 1-1 -> 1-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += previousStep[i][posY - 1].isLife() ? 1 : 0;
    
    // 3-1 -> 3-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += previousStep[i][posY + 1].isLife() ? 1 : 0;
    
    return lifes;
}

void GameManager::nextStep() {
    previousStep = GameField(gameField);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            size_t life = countLifeAround(i, j);
            bool hasLife = previousStep[i][j].isLife();
            if (hasLife && (life < DEATH_LONELINESS || life > DEATH_OVERPOPULATION))
                gameField[i][j].kill();
            else if (!hasLife && life == BORN_LIFE)
                gameField[i][j].bornLife();
        }
    }
    stepsCounter++;
    hasUndo = true;
    update();
}

bool GameManager::setCellAt(int posX, int posY) {
    previousStep = GameField(gameField);
    if (gameField[posX][posY].isLife())
        gameField[posX][posY].kill();
    else
        gameField[posX][posY].bornLife();
    hasUndo = true;
    update();
    return gameField[posX][posY].isLife();
}

void GameManager::reset(size_t width, size_t height) {
    this->width = width;
    this->height = height;
    gameField = GameField(width, height);
    stepsCounter = 0;
    update();
}

bool GameManager::stepBack() {
    if (!hasUndo)
        return false;
    
    gameField = GameField(previousStep);
    hasUndo = false;
    stepsCounter--;
    update();
    
    return true;
}

void GameManager::update() const {
    updateListener.onUpdate(gameField);
}

void GameManager::registerCommand(std::string name, void (*cmd)(const std::vector<std::string>&, GameManager&, std::ostream&)) {
    commands[name] = cmd;
}

bool GameManager::executeCommand(std::string name, std::vector<std::string>& args, std::ostream& output) {
    auto executor = commands.find(name);
    if (executor == commands.end())
        return false;
    executor->second(args, (*this), output);
    return true;
}

/**
 * Разбивает строку на пробелы.
 *
 * @param str Строка для разбития
 *
 * @return Вектор разбитых строк
 * */
static std::vector<std::string> splitString(std::string str) {
    std::istringstream input(str);
    std::string item;
    std::vector<std::string> items;
    while(std::getline(input, item, ' '))
        items.push_back(item);
    return items;
}

void GameManager::executionInCommandMode() {
    move((int) gameField.getHeight() + 1, 0);
    clrtobot();
    printw(">> ");
    keypad(stdscr, FALSE);
    curs_set(1);
    echo();
    
    char str[MAX_COMMAND_LEN];
    getnstr(str, MAX_COMMAND_LEN);
    
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    
    std::vector<std::string> split = splitString(std::string(str));
    
    std::string command = split[0];
    std::vector<std::string> args(split.begin() + 1, split.begin() + split.size());
    std::ostringstream out;
    
    if (!executeCommand(command, args, out))
        out << "Command \"" << command << "\" not found.";
    
    move((int) gameField.getHeight() + 1, 0);
    clrtobot();
    std::string outStr = out.str();
    printw(outStr.c_str());
}

void GameManager::onMousePressed(int x, int y) {
    x /= 2;
    if (x < 0 ||
        y < 0 ||
        x >= width ||
        y >= height)
        return;
    setCellAt(x, y);
}

bool GameManager::onKeyPressed(int key) {
    switch (key) {
        case KEY_N:
            nextStep();
            break;
        case KEY_Q:
            return true;
        case KEY_B:
            stepBack();
            break;
        case KEY_R:
            reset(width, height);
            break;
        case KEY_C:
            executionInCommandMode();
            break;
        default:
            break;
    }
    return false;
}

const GameField GameManager::getCurrentField() const {
    return gameField;
}

size_t GameManager::getStepsCount() const {
    return stepsCounter;
}
