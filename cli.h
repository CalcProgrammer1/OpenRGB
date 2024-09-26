#ifndef CLI_H
#define CLI_H

unsigned int cli_pre_detection(int argc, char* argv[]);
unsigned int cli_post_detection();

enum
{
    RET_FLAG_PRINT_HELP         = 1,
    RET_FLAG_START_GUI          = 2,
    RET_FLAG_I2C_TOOLS          = 4,
    RET_FLAG_START_MINIMIZED    = 8,
    RET_FLAG_NO_DETECT          = 16,
    RET_FLAG_CLI_POST_DETECTION = 32,
    RET_FLAG_START_SERVER       = 64,
    RET_FLAG_NO_AUTO_CONNECT    = 128,
};

#endif
