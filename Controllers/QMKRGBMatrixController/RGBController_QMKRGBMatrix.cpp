/*-------------------------------------------------------------------*\
|  RGBController_QMKRGBMatrix.cpp                                     |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "hsv.h"
#include "RGBController_QMKRGBMatrix.h"

RGBController_QMKRGBMatrix::RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr)
{
    qmk_rgb_matrix = qmk_rgb_matrix_ptr;

    name = qmk_rgb_matrix_ptr->GetName();
    description = "QMK RGB Matrix Device";
    type = DEVICE_TYPE_KEYBOARD;
    location = qmk_rgb_matrix_ptr->GetLocation();
    version = qmk_rgb_matrix_ptr->GetVersion();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = QMK_RGBMATRIX_MODE_HID_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = QMK_RGBMATRIX_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    Static.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    Static.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(Static);

    /*mode AlphaMod;
    AlphaMod.name       = "Alpha Mod";
    AlphaMod.value      = QMK_RGBMATRIX_MODE_ALPHA_MOD;
    AlphaMod.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    AlphaMod.color_mode = MODE_COLORS_PER_LED;
    AlphaMod.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    AlphaMod.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    AlphaMod.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(AlphaMod);

    mode StaticHorizontalGradient;
    StaticHorizontalGradient.name       = "Static Horizontal Gradient";
    StaticHorizontalGradient.value      = QMK_RGBMATRIX_MODE_STATIC_GRADIENT_LEFT_RIGHT;
    StaticHorizontalGradient.flags      = 0;
    modes.push_back(StaticHorizontalGradient);

    mode StaticVerticalGradient;
    StaticVerticalGradient.name       = "Static Vertical Gradient";
    StaticVerticalGradient.value      = QMK_RGBMATRIX_MODE_STATIC_GRADIENT_UP_DOWN;
    StaticVerticalGradient.flags      = 0;
    modes.push_back(StaticVerticalGradient);*/

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = QMK_RGBMATRIX_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    Breathing.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(Breathing);

    mode ScrollOnWhite;
    ScrollOnWhite.name       = "Scroll On White";
    ScrollOnWhite.value      = QMK_RGBMATRIX_MODE_BAND_SAT;
    ScrollOnWhite.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ScrollOnWhite.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    ScrollOnWhite.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    ScrollOnWhite.colors_min = 1;
    ScrollOnWhite.colors_max = 1;
    ScrollOnWhite.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    ScrollOnWhite.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ScrollOnWhite.colors.resize(1);
    ScrollOnWhite.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(ScrollOnWhite);

    mode Scroll;
    Scroll.name       = "Scroll";
    Scroll.value      = QMK_RGBMATRIX_MODE_BAND_VAL;
    Scroll.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Scroll.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    Scroll.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    Scroll.colors_min = 1;
    Scroll.colors_max = 1;
    Scroll.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    Scroll.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Scroll.colors.resize(1);
    Scroll.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(Scroll);

    mode PinWheelSpinOnWhite;
    PinWheelSpinOnWhite.name       = "PinWheel Spin On White";
    PinWheelSpinOnWhite.value      = QMK_RGBMATRIX_MODE_BAND_PINWHEEL_SAT;
    PinWheelSpinOnWhite.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    PinWheelSpinOnWhite.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    PinWheelSpinOnWhite.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    PinWheelSpinOnWhite.colors_min = 1;
    PinWheelSpinOnWhite.colors_max = 1;
    PinWheelSpinOnWhite.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    PinWheelSpinOnWhite.color_mode = MODE_COLORS_MODE_SPECIFIC;
    PinWheelSpinOnWhite.colors.resize(1);
    PinWheelSpinOnWhite.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(PinWheelSpinOnWhite);

    mode PinWheelSpin;
    PinWheelSpin.name       = "PinWheel Spin";
    PinWheelSpin.value      = QMK_RGBMATRIX_MODE_BAND_PINWHEEL_VAL;
    PinWheelSpin.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    PinWheelSpin.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    PinWheelSpin.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    PinWheelSpin.colors_min = 1;
    PinWheelSpin.colors_max = 1;
    PinWheelSpin.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    PinWheelSpin.color_mode = MODE_COLORS_MODE_SPECIFIC;
    PinWheelSpin.colors.resize(1);
    PinWheelSpin.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(PinWheelSpin);

    mode SpiralSpinOnWhite;
    SpiralSpinOnWhite.name       = "Spiral Spin On White";
    SpiralSpinOnWhite.value      = QMK_RGBMATRIX_MODE_BAND_SPIRAL_SAT;
    SpiralSpinOnWhite.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SpiralSpinOnWhite.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SpiralSpinOnWhite.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SpiralSpinOnWhite.colors_min = 1;
    SpiralSpinOnWhite.colors_max = 1;
    SpiralSpinOnWhite.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SpiralSpinOnWhite.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SpiralSpinOnWhite.colors.resize(1);
    SpiralSpinOnWhite.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SpiralSpinOnWhite);

    mode SpiralSpin;
    SpiralSpin.name       = "Spiral Spin";
    SpiralSpin.value      = QMK_RGBMATRIX_MODE_BAND_SPIRAL_VAL;
    SpiralSpin.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SpiralSpin.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SpiralSpin.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SpiralSpin.colors_min = 1;
    SpiralSpin.colors_max = 1;
    SpiralSpin.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SpiralSpin.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SpiralSpin.colors.resize(1);
    SpiralSpin.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SpiralSpin);

    mode ColorCycle;
    ColorCycle.name          = "Color Cycle";
    ColorCycle.value         = QMK_RGBMATRIX_MODE_CYCLE_ALL;
    ColorCycle.flags         = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    ColorCycle.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    ColorCycle.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(ColorCycle);

    mode RainbowHorizontal;
    RainbowHorizontal.name          = "Rainbow Horizontal";
    RainbowHorizontal.value         = QMK_RGBMATRIX_MODE_CYCLE_LEFT_RIGHT;
    RainbowHorizontal.flags         = MODE_FLAG_HAS_SPEED;
    RainbowHorizontal.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowHorizontal.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowHorizontal.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowHorizontal);

    mode RainbowVertical;
    RainbowVertical.name          = "Rainbow Vertical";
    RainbowVertical.value         = QMK_RGBMATRIX_MODE_CYCLE_UP_DOWN;
    RainbowVertical.flags         = MODE_FLAG_HAS_SPEED;
    RainbowVertical.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowVertical.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowVertical.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowVertical);

    mode RainbowOutIn;
    RainbowOutIn.name          = "Rainbow Out In";
    RainbowOutIn.value         = QMK_RGBMATRIX_MODE_CYCLE_OUT_IN;
    RainbowOutIn.flags         = MODE_FLAG_HAS_SPEED;
    RainbowOutIn.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowOutIn.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowOutIn.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowOutIn);

    mode RainbowOutInDual;
    RainbowOutInDual.name          = "Rainbow Out In Dual";
    RainbowOutInDual.value         = QMK_RGBMATRIX_MODE_CYCLE_OUT_IN_DUAL;
    RainbowOutInDual.flags         = MODE_FLAG_HAS_SPEED;
    RainbowOutInDual.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowOutInDual.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowOutInDual.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowOutInDual);

    mode RainbowMovingChevron;
    RainbowMovingChevron.name          = "Rainbow Moving Chevron";
    RainbowMovingChevron.value         = QMK_RGBMATRIX_MODE_RAINBOW_MOVING_CHEVRON;
    RainbowMovingChevron.flags         = MODE_FLAG_HAS_SPEED;
    RainbowMovingChevron.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowMovingChevron.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowMovingChevron.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowMovingChevron);

    mode RainbowDualBeacon;
    RainbowDualBeacon.name          = "Rainbow Dual Beacons";
    RainbowDualBeacon.value         = QMK_RGBMATRIX_MODE_DUAL_BEACON;
    RainbowDualBeacon.flags         = MODE_FLAG_HAS_SPEED;
    RainbowDualBeacon.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowDualBeacon.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowDualBeacon.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowDualBeacon);

    mode RainbowPinWheel;
    RainbowPinWheel.name          = "Rainbow Pin Wheel";
    RainbowPinWheel.value         = QMK_RGBMATRIX_MODE_CYCLE_PINWHEEL;
    RainbowPinWheel.flags         = MODE_FLAG_HAS_SPEED;
    RainbowPinWheel.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowPinWheel.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowPinWheel.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowPinWheel);

    mode RainbowCycleSpiral;
    RainbowCycleSpiral.name          = "Rainbow Spiral";
    RainbowCycleSpiral.value         = QMK_RGBMATRIX_MODE_CYCLE_SPIRAL;
    RainbowCycleSpiral.flags         = MODE_FLAG_HAS_SPEED;
    RainbowCycleSpiral.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowCycleSpiral.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowCycleSpiral.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowCycleSpiral);

    mode RainbowBeacon;
    RainbowBeacon.name          = "Rainbow Beacon";
    RainbowBeacon.value         = QMK_RGBMATRIX_MODE_RAINBOW_BEACON;
    RainbowBeacon.flags         = MODE_FLAG_HAS_SPEED;
    RainbowBeacon.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowBeacon.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowBeacon.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowBeacon);

    mode RainbowDualPinWheels;
    RainbowDualPinWheels.name          = "Rainbow Dual Pin Wheels";
    RainbowDualPinWheels.value         = QMK_RGBMATRIX_MODE_RAINBOW_PINWHEELS;
    RainbowDualPinWheels.flags         = MODE_FLAG_HAS_SPEED;
    RainbowDualPinWheels.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowDualPinWheels.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowDualPinWheels.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowDualPinWheels);

    mode Raindrops;
    Raindrops.name          = "Raindrops";
    Raindrops.value         = QMK_RGBMATRIX_MODE_RAINDROPS;
    Raindrops.flags         = 0;
    modes.push_back(Raindrops);

    mode JellybeanRaindrops;
    JellybeanRaindrops.name          = "Jellybean Raindrops";
    JellybeanRaindrops.value         = QMK_RGBMATRIX_MODE_JELLYBEAN_RAINDROPS;
    JellybeanRaindrops.flags         = 0;
    modes.push_back(JellybeanRaindrops);

    mode TypingHeatmap;
    TypingHeatmap.name          = "Typing Heatmap";
    TypingHeatmap.value         = QMK_RGBMATRIX_MODE_TYPING_HEATMAP;
    TypingHeatmap.flags         = 0;
    modes.push_back(TypingHeatmap);

    mode DigitalRain;
    DigitalRain.name          = "Digital Rain";
    DigitalRain.value         = QMK_RGBMATRIX_MODE_DIGITAL_RAIN;
    DigitalRain.flags         = 0;
    modes.push_back(DigitalRain);

    mode SolidReactive;
    SolidReactive.name       = "Solid Reactive";
    SolidReactive.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE;
    SolidReactive.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactive.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactive.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactive.colors_min = 1;
    SolidReactive.colors_max = 1;
    SolidReactive.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactive.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactive.colors.resize(1);
    SolidReactive.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SolidReactive);

    mode SolidReactiveSimple;
    SolidReactiveSimple.name       = "Solid Reactive Simple";
    SolidReactiveSimple.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_SIMPLE;
    SolidReactiveSimple.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveSimple.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveSimple.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveSimple.colors_min = 1;
    SolidReactiveSimple.colors_max = 1;
    SolidReactiveSimple.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveSimple.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveSimple.colors.resize(1);
    SolidReactiveSimple.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SolidReactiveSimple);

    mode SolidReactiveWide;
    SolidReactiveWide.name       = "Solid Reactive Wide";
    SolidReactiveWide.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_WIDE;
    SolidReactiveWide.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveWide.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveWide.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveWide.colors_min = 1;
    SolidReactiveWide.colors_max = 1;
    SolidReactiveWide.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveWide.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveWide.colors.resize(1);
    SolidReactiveWide.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SolidReactiveWide);


    /*mode SolidReactiveMultiWide;
    SolidReactiveMultiWide.name       = "Solid Reactive Multi Wide";
    SolidReactiveMultiWide.value      = QMK_RGBMATRIX_MODE_BREATHING;
    SolidReactiveMultiWide.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveMultiWide.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveMultiWide.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveMultiWide.colors_min = 2;
    SolidReactiveMultiWide.colors_max = 2;
    SolidReactiveMultiWide.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveMultiWide.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveMultiWide.colors.resize(2);
    modes.push_back(SolidReactiveMultiWide);*/

    mode SolidReactiveCross;
    SolidReactiveCross.name       = "Solid Reactive Cross";
    SolidReactiveCross.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_CROSS;
    SolidReactiveCross.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveCross.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveCross.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveCross.colors_min = 1;
    SolidReactiveCross.colors_max = 1;
    SolidReactiveCross.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveCross.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveCross.colors.resize(1);
    SolidReactiveCross.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SolidReactiveCross);

    /*mode SolidReactiveMultiCross;
    SolidReactiveMultiCross.name       = "Solid Reactive Multi Cross";
    SolidReactiveMultiCross.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTICROSS;
    SolidReactiveMultiCross.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveMultiCross.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveMultiCross.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveMultiCross.colors_min = 2;
    SolidReactiveMultiCross.colors_max = 2;
    SolidReactiveMultiCross.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveMultiCross.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveMultiCross.colors.resize(2);
    modes.push_back(SolidReactiveMultiCross)*/

    mode SolidReactiveNexus;
    SolidReactiveNexus.name       = "Solid Reactive Nexus";
    SolidReactiveNexus.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_NEXUS;
    SolidReactiveNexus.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveNexus.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveNexus.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveNexus.colors_min = 1;
    SolidReactiveNexus.colors_max = 1;
    SolidReactiveNexus.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveNexus.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveNexus.colors.resize(1);
    SolidReactiveNexus.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SolidReactiveNexus);

    /*mode SolidReactiveMultiNexus;
    SolidReactiveMultiNexus.name       = "Solid Reactive Multi Nexus";
    SolidReactiveMultiNexus.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTINEXUS;
    SolidReactiveMultiNexus.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveMultiNexus.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveMultiNexus.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveMultiNexus.colors_min = 2;
    SolidReactiveMultiNexus.colors_max = 2;
    SolidReactiveMultiNexus.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveMultiNexus.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveMultiNexus.colors.resize(2);
    modes.push_back(SolidReactiveMultiNexus);*/

    mode Splash;
    Splash.name       = "Rainbow Reactive Splash";
    Splash.value      = QMK_RGBMATRIX_MODE_SPLASH;
    Splash.flags      = MODE_FLAG_HAS_SPEED;
    Splash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    Splash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    Splash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(Splash);

    /*mode MultiSplash;
    MultiSplash.name       = "Rainbow Reactive Multi Splash";
    MultiSplash.value      = QMK_RGBMATRIX_MODE_MULTISPLASH;
    MultiSplash.flags      = MODE_FLAG_HAS_SPEED;
    MultiSplash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    MultiSplash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    MultiSplash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(MultiSplash);*/

    mode SolidSplash;
    SolidSplash.name       = "Solid Reactive Splash";
    SolidSplash.value      = QMK_RGBMATRIX_MODE_SOLID_SPLASH;
    SolidSplash.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidSplash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidSplash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidSplash.colors_min = 1;
    SolidSplash.colors_max = 1;
    SolidSplash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidSplash.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidSplash.colors.resize(1);
    SolidSplash.colors[0] = qmk_rgb_matrix->GetHSV();
    modes.push_back(SolidSplash);

    /*mode SolidMultiSplash;
    SolidMultiSplash.name       = "Solid Reactive Multi Splash";
    SolidMultiSplash.value      = QMK_RGBMATRIX_MODE_SOLID_MULTISPLASH;
    SolidMultiSplash.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidMultiSplash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidMultiSplash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidMultiSplash.colors_min = 2;
    SolidMultiSplash.colors_max = 2;
    SolidMultiSplash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidMultiSplash.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidMultiSplash.colors.resize(2);
    modes.push_back(SolidMultiSplash);*/

    SetupZones();
}

RGBController_QMKRGBMatrix::~RGBController_QMKRGBMatrix()
{
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_QMKRGBMatrix::SetupZones()
{
    int led_index = 0;
    int zone_count = qmk_rgb_matrix->GetZonesCount();

    for(int i = 0; i < zone_count; i++)
    {
        unsigned int zone_led_count = qmk_rgb_matrix->GetZoneSize(i);

        zone keyboard_zone;
        keyboard_zone.name = qmk_rgb_matrix->GetZoneName(i);
        keyboard_zone.type = qmk_rgb_matrix->GetZoneType(i);
        keyboard_zone.leds_min = zone_led_count;
        keyboard_zone.leds_max = zone_led_count;
        keyboard_zone.leds_count = zone_led_count;

        if(keyboard_zone.type == ZONE_TYPE_MATRIX)
        {
            unsigned int led_matrix_columns = qmk_rgb_matrix->GetLEDMatirxColumns();
            unsigned int led_matrix_rows = qmk_rgb_matrix->GetLEDMatirxRows();

            keyboard_zone.matrix_map = new matrix_map_type;
            keyboard_zone.matrix_map->height = led_matrix_columns;
            keyboard_zone.matrix_map->width = led_matrix_rows;

            unsigned int* matrix_map = new unsigned int[led_matrix_rows * led_matrix_columns];
            for(unsigned int y = 0; y < led_matrix_columns; y++)
            {
                for(unsigned int x = 0; x < led_matrix_rows; x++)
                {
                    matrix_map[led_matrix_rows * y + x] = qmk_rgb_matrix->GetLEDValueInMatrix(y, x);
                }
            }

            keyboard_zone.matrix_map->map = matrix_map;
        }
        else
        {
            keyboard_zone.matrix_map = NULL;
        }

        zones.push_back(keyboard_zone);

        for(std::size_t led_idx = 0; led_idx < zone_led_count; led_idx++)
        {
            led keyboard_led;
            keyboard_led.name = qmk_rgb_matrix->GetLEDName(led_index);
            leds.push_back(keyboard_led);
            led_index++;
        }
    }

    SetupColors();

    for(int i = 0; i < led_index; i++)
    {
        colors[i] = qmk_rgb_matrix->GetLEDColor(i);
    }
}

void RGBController_QMKRGBMatrix::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_QMKRGBMatrix::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        qmk_rgb_matrix->SetLEDs(zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_QMKRGBMatrix::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKRGBMatrix::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    qmk_rgb_matrix->SetSingleLED(led, red, grn, blu);
}

void RGBController_QMKRGBMatrix::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_QMKRGBMatrix::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        qmk_rgb_matrix->SetMode(modes[active_mode].value);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
    {
        qmk_rgb_matrix->SetModeAndSpeed(modes[active_mode].value, modes[active_mode].speed);
    } 
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        RGBColor color = modes[active_mode].colors[0];
        hsv_t hsv_color;
        rgb2hsv(color, &hsv_color);
        qmk_rgb_matrix->SetColorModeAndSpeed(hsv_color, modes[active_mode].value, modes[active_mode].speed);
    }
}

