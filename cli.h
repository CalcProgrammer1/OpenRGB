/*---------------------------------------------------------*\
| cli.h                                                     |
|                                                           |
|   OpenRGB command line interface                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

unsigned int cli_pre_detection(int argc, char* argv[]);
unsigned int cli_post_detection();

/*---------------------------------------------------------*\
| Return flags, a bitfield containing the relevant command  |
| line arguments needed for application startup             |
\*---------------------------------------------------------*/
enum
{
    RET_FLAG_PRINT_HELP             = 1,
    RET_FLAG_START_GUI              = 2,
    RET_FLAG_I2C_TOOLS              = 4,
    RET_FLAG_START_MINIMIZED        = 8,
    RET_FLAG_NO_DETECT              = 16,
    RET_FLAG_CLI_POST_DETECTION     = 32,
    RET_FLAG_START_SERVER           = 64,
    RET_FLAG_NO_AUTO_CONNECT        = 128,
    RET_FLAG_GENERATE_UDEV_RULES    = 256,
};
