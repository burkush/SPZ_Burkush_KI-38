#ifndef COLORED_CONSOLE_H
#define COLORED_CONSOLE_H

#include <windows.h>

const WORD DEFAULT_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const WORD SUCCESS_COLOR = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
const WORD ERROR_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED;
const WORD PATH_COLOR = FOREGROUND_INTENSITY;
const WORD HEADING_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_RED;

/**
 * @class ColoredConsole
 * @brief A class for manipulating the colors of the console output.
 *
 * The ColoredConsole class provides functionality to change the color of the console output.
 */
class ColoredConsole {
public:
    /**
	 * @brief Sets the color of the console output.
	 * @param color The color to be set, defined by a combination of color attributes from the Windows API.
	 *
	 * This function sets the color or the background color of the console output to the specified color. The color is defined by 
	 * a combination of color attributes from the Windows API.
	 */
	static void setConsoleColor(WORD color);
};

#endif