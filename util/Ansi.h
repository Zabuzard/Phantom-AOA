#ifndef PHANTOM_AOA_ANSI_H
#define PHANTOM_AOA_ANSI_H

#include <string>

namespace ansi {
const std::string BLACK = "\u001B[30m";
const std::string WHITE = "\u001B[37m";
const std::string GREEN = "\u001B[32m";
const std::string RED = "\u001B[31m";
const std::string YELLOW = "\u001B[33m";

const std::string WHITE_BACKGROUND = "\u001B[47m";
const std::string BLACK_BACKGROUND = "\u001B[40m";
const std::string RED_BACKGROUND = "\u001B[41m";
const std::string CYAN_BACKGROUND = "\u001B[46m";
const std::string YELLOW_BACKGROUND = "\u001B[43m";

const std::string COLOR_RESET = "\u001B[0m";

const std::string CLEAR_SCREEN = "\033[2J";
} // ansi

#endif //PHANTOM_AOA_ANSI_H
