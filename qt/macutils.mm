#import <Cocoa/Cocoa.h>
#include "macutils.h"

void MacUtils::ToggleApplicationDocklessState(bool showDock)
{
    ProcessApplicationTransformState transformState = showDock ? ProcessApplicationTransformState(kProcessTransformToForegroundApplication) : ProcessApplicationTransformState(kProcessTransformToUIElementApplication);
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType(&psn, transformState);
}
