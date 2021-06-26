#include "AutoStart.h"

/*-----------------------------------------------------*\
| Common AutoStart Implementation                       |
| Public Methods                                        |
\*-----------------------------------------------------*/

std::string AutoStartInterface::GetAutoStartFile()
{
    return(autostart_file);
}

std::string AutoStartInterface::GetAutoStartName()
{
    return(autostart_name);
}
