//! \file RzChromaSDKTypes.h
//! \brief Data types.

#ifndef _RZCHROMASDKTYPES_H_
#define _RZCHROMASDKTYPES_H_

#pragma once

typedef LONG            RZRESULT;           //!< Return result.
typedef GUID            RZEFFECTID;         //!< Effect Id.
typedef GUID            RZDEVICEID;         //!< Device Id.
typedef unsigned int    RZDURATION;         //!< Milliseconds.
typedef size_t          RZSIZE;             //!< Size.
typedef void*           PRZPARAM;           //!< Context sensitive pointer.
typedef DWORD           RZID;               //!< Generic data type for Identifier.
typedef DWORD           RZCOLOR;            //!< Color data. 1st byte = Red; 2nd byte = Green; 3rd byte = Blue; 4th byte = Alpha (if applicable)

namespace ChromaSDK
{
    //! Event notification Window message
    const UINT WM_CHROMA_EVENT = WM_APP+0x2000;

    //! Application information.
    typedef struct APPINFOTYPE
    {
        TCHAR Title[256];           //!< Title of the application.
        TCHAR Description[1024];    //!< A short description of the application.
        //! Author information
        struct Author
        {
            TCHAR Name[256];        //!< Name of the developer/company.
            TCHAR Contact[256];     //!< Contact info.
        } Author;
        DWORD SupportedDevice;
        DWORD Category;             //!< 0 = Utility; 1 = Game; 2 = Chroma Link Broadcast
    } APPINFOTYPE;

    //! Chroma generic effects. Note: Not all devices supported the listed effects.
    typedef enum EFFECT_TYPE
    {
        CHROMA_NONE = 0,            //!< No effect.
        CHROMA_WAVE,                //!< Wave effect (Deprecated and should not be used).
        CHROMA_SPECTRUMCYCLING,     //!< Spectrum cycling effect (Deprecated and should not be used).
        CHROMA_BREATHING,           //!< Breathing effect (Deprecated and should not be used).
        CHROMA_BLINKING,            //!< Blinking effect (Deprecated and should not be used).
        CHROMA_REACTIVE,            //!< Reactive effect (Deprecated and should not be used).
        CHROMA_STATIC,              //!< Static single color effect.
        CHROMA_CUSTOM,              //!< Custom effect. For mice, please see Mouse::CHROMA_CUSTOM2.
        CHROMA_RESERVED,            //!< Reserved
        CHROMA_INVALID              //!< Invalid effect.
    } EFFECT_TYPE;

    //! Device info.
    typedef struct DEVICE_INFO_TYPE
    {
        //! Device types.
        enum DeviceType
        {
            DEVICE_KEYBOARD    = 1, //!< Keyboard device.
            DEVICE_MOUSE       = 2, //!< Mouse device.
            DEVICE_HEADSET     = 3, //!< Headset device.
            DEVICE_MOUSEPAD    = 4, //!< Mousepad device.
            DEVICE_KEYPAD      = 5, //!< Keypad device.
            DEVICE_SYSTEM      = 6, //!< System device.
            DEVICE_SPEAKERS    = 7, //!< Speakers.
            DEVICE_INVALID          //!< Invalid device.
        } DeviceType;

        DWORD Connected;            //!< Number of devices connected.
    } DEVICE_INFO_TYPE;

    const RZSIZE MAX_ROW = 30;      //!< Maximum rows for custom effects.
    const RZSIZE MAX_COLUMN = 30;   //!< Maximum columns for custom effects.

    //! Blinking effect (Deprecated and should not be used).
    typedef struct BLINKING_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(BLINKING_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.

        COLORREF Color;     //!< Blinking color
    } BLINKING_EFFECT_TYPE;

    //! Breathing effect (Deprecated and should not be used).
    typedef struct BREATHING_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of ths structure. Size = sizeof(BREATHING_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.

        //! Breathing effect types.
        enum _Type
        {
            ONE_COLOR = 1,  //!< 1 color (Only fill Color1).
            TWO_COLORS,     //!< 2 colors.
            RANDOM_COLORS   //!< Random colors
        } Type;

        COLORREF Color1;    //!< First color.
        COLORREF Color2;    //!< Second color.
    } BREATHING_EFFECT_TYPE;

    //! Custom effect (Deprecated and should not be used).
    typedef struct CUSTOM_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(CUSTOM_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.

        RZCOLOR Color[MAX_ROW][MAX_COLUMN];
    } CUSTOM_EFFECT_TYPE;

    //! No effect.
    typedef struct NO_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(NO_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.
    } NO_EFFECT_TYPE;

    //! Reactive effect (Deprecated and should not be used).
    typedef struct REACTIVE_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(REACTIVE_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.

        //! Duration of the effect.
        enum _Duration
        {
            DURATION_SHORT = 1, //!< Short duration.
            DURATION_MEDIUM,    //!< Medium duration.
            DURATION_LONG       //!< Long duration.
        } Duration;             //!< The time taken for the effect to fade away.

        COLORREF Color;         //!< Color of the effect.
    } REACTIVE_EFFECT_TYPE;

    //! Spectrum cycling effect (Deprecated and should not be used).
    typedef struct SPECTRUMCYCLING_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(SPECTRUMCYCLING_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.
    } SPECTRUMCYCLING_EFFECT_TYPE;

    //! Starlight effect (Deprecated and should not be used).
    typedef struct STARLIGHT_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(SPECTRUMCYCLING_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.

        //! Starlight effect types.
        enum _Type
        {
            TWO_COLORS = 1, //!< 2 colors.
            RANDOM_COLORS   //!< Random colors
        } Type;

        COLORREF Color1;    //!< First color.
        COLORREF Color2;    //!< Second color.

        //! Duration of the effect.
        enum _Duration
        {
            DURATION_SHORT = 1, //!< Short duration.
            DURATION_MEDIUM,    //!< Medium duration.
            DURATION_LONG       //!< Long duration.
        } Duration;             //!< The time taken for the effect to fade away.

    } STARLIGHT_EFFECT_TYPE;

    //! Static effect (Deprecated and should not be used).
    typedef struct STATIC_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(STATIC_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.

        COLORREF Color;     //!< Color of the effect.
    } STATIC_EFFECT_TYPE;

    //! Wave effect (Deprecated and should not be used).
    typedef struct WAVE_EFFECT_TYPE
    {
        RZSIZE Size;        //!< Size of the structure. Size = sizeof(WAVE_EFFECT_TYPE)
        DWORD Param;        //!< Extra parameters.

        //! Direction of effect.
        enum _Direction
        {
            DIRECTION_LEFT_TO_RIGHT = 1,    //!< Left to right.
            DIRECTION_RIGHT_TO_LEFT,        //!< Right to left.
            DIRECTION_FRONT_TO_BACK,        //!< Front to back
            DIRECTION_BACK_TO_FRONT         //!< Back top front
        } Direction;
    } WAVE_EFFECT_TYPE;

    //! Keyboards
    namespace Keyboard
    {
        //! Definitions of keys.
        typedef enum RZKEY
        {
            RZKEY_ESC = 0x0001,                 /*!< Esc (VK_ESCAPE) */
            RZKEY_F1 = 0x0003,                  /*!< F1 (VK_F1) */
            RZKEY_F2 = 0x0004,                  /*!< F2 (VK_F2) */
            RZKEY_F3 = 0x0005,                  /*!< F3 (VK_F3) */
            RZKEY_F4 = 0x0006,                  /*!< F4 (VK_F4) */
            RZKEY_F5 = 0x0007,                  /*!< F5 (VK_F5) */
            RZKEY_F6 = 0x0008,                  /*!< F6 (VK_F6) */
            RZKEY_F7 = 0x0009,                  /*!< F7 (VK_F7) */
            RZKEY_F8 = 0x000A,                  /*!< F8 (VK_F8) */
            RZKEY_F9 = 0x000B,                  /*!< F9 (VK_F9) */
            RZKEY_F10 = 0x000C,                 /*!< F10 (VK_F10) */
            RZKEY_F11 = 0x000D,                 /*!< F11 (VK_F11) */
            RZKEY_F12 = 0x000E,                 /*!< F12 (VK_F12) */
            RZKEY_1 = 0x0102,                   /*!< 1 (VK_1) */
            RZKEY_2 = 0x0103,                   /*!< 2 (VK_2) */
            RZKEY_3 = 0x0104,                   /*!< 3 (VK_3) */
            RZKEY_4 = 0x0105,                   /*!< 4 (VK_4) */
            RZKEY_5 = 0x0106,                   /*!< 5 (VK_5) */
            RZKEY_6 = 0x0107,                   /*!< 6 (VK_6) */
            RZKEY_7 = 0x0108,                   /*!< 7 (VK_7) */
            RZKEY_8 = 0x0109,                   /*!< 8 (VK_8) */
            RZKEY_9 = 0x010A,                   /*!< 9 (VK_9) */
            RZKEY_0 = 0x010B,                   /*!< 0 (VK_0) */
            RZKEY_A = 0x0302,                   /*!< A (VK_A) */
            RZKEY_B = 0x0407,                   /*!< B (VK_B) */
            RZKEY_C = 0x0405,                   /*!< C (VK_C) */
            RZKEY_D = 0x0304,                   /*!< D (VK_D) */
            RZKEY_E = 0x0204,                   /*!< E (VK_E) */
            RZKEY_F = 0x0305,                   /*!< F (VK_F) */
            RZKEY_G = 0x0306,                   /*!< G (VK_G) */
            RZKEY_H = 0x0307,                   /*!< H (VK_H) */
            RZKEY_I = 0x0209,                   /*!< I (VK_I) */
            RZKEY_J = 0x0308,                   /*!< J (VK_J) */
            RZKEY_K = 0x0309,                   /*!< K (VK_K) */
            RZKEY_L = 0x030A,                   /*!< L (VK_L) */
            RZKEY_M = 0x0409,                   /*!< M (VK_M) */
            RZKEY_N = 0x0408,                   /*!< N (VK_N) */
            RZKEY_O = 0x020A,                   /*!< O (VK_O) */
            RZKEY_P = 0x020B,                   /*!< P (VK_P) */
            RZKEY_Q = 0x0202,                   /*!< Q (VK_Q) */
            RZKEY_R = 0x0205,                   /*!< R (VK_R) */
            RZKEY_S = 0x0303,                   /*!< S (VK_S) */
            RZKEY_T = 0x0206,                   /*!< T (VK_T) */
            RZKEY_U = 0x0208,                   /*!< U (VK_U) */
            RZKEY_V = 0x0406,                   /*!< V (VK_V) */
            RZKEY_W = 0x0203,                   /*!< W (VK_W) */
            RZKEY_X = 0x0404,                   /*!< X (VK_X) */
            RZKEY_Y = 0x0207,                   /*!< Y (VK_Y) */
            RZKEY_Z = 0x0403,                   /*!< Z (VK_Z) */
            RZKEY_NUMLOCK = 0x0112,             /*!< Numlock (VK_NUMLOCK) */
            RZKEY_NUMPAD0 = 0x0513,             /*!< Numpad 0 (VK_NUMPAD0) */
            RZKEY_NUMPAD1 = 0x0412,             /*!< Numpad 1 (VK_NUMPAD1) */
            RZKEY_NUMPAD2 = 0x0413,             /*!< Numpad 2 (VK_NUMPAD2) */
            RZKEY_NUMPAD3 = 0x0414,             /*!< Numpad 3 (VK_NUMPAD3) */
            RZKEY_NUMPAD4 = 0x0312,             /*!< Numpad 4 (VK_NUMPAD4) */
            RZKEY_NUMPAD5 = 0x0313,             /*!< Numpad 5 (VK_NUMPAD5) */
            RZKEY_NUMPAD6 = 0x0314,             /*!< Numpad 6 (VK_NUMPAD6) */
            RZKEY_NUMPAD7 = 0x0212,             /*!< Numpad 7 (VK_NUMPAD7) */
            RZKEY_NUMPAD8 = 0x0213,             /*!< Numpad 8 (VK_NUMPAD8) */
            RZKEY_NUMPAD9 = 0x0214,             /*!< Numpad 9 (VK_ NUMPAD9*/
            RZKEY_NUMPAD_DIVIDE = 0x0113,       /*!< Divide (VK_DIVIDE) */
            RZKEY_NUMPAD_MULTIPLY = 0x0114,     /*!< Multiply (VK_MULTIPLY) */
            RZKEY_NUMPAD_SUBTRACT = 0x0115,     /*!< Subtract (VK_SUBTRACT) */
            RZKEY_NUMPAD_ADD = 0x0215,          /*!< Add (VK_ADD) */
            RZKEY_NUMPAD_ENTER = 0x0415,        /*!< Enter (VK_RETURN - Extended) */
            RZKEY_NUMPAD_DECIMAL = 0x0514,      /*!< Decimal (VK_DECIMAL) */
            RZKEY_PRINTSCREEN = 0x000F,         /*!< Print Screen (VK_PRINT) */
            RZKEY_SCROLL = 0x0010,              /*!< Scroll Lock (VK_SCROLL) */
            RZKEY_PAUSE = 0x0011,               /*!< Pause (VK_PAUSE) */
            RZKEY_INSERT = 0x010F,              /*!< Insert (VK_INSERT) */
            RZKEY_HOME = 0x0110,                /*!< Home (VK_HOME) */
            RZKEY_PAGEUP = 0x0111,              /*!< Page Up (VK_PRIOR) */
            RZKEY_DELETE = 0x020f,              /*!< Delete (VK_DELETE) */
            RZKEY_END = 0x0210,                 /*!< End (VK_END) */
            RZKEY_PAGEDOWN = 0x0211,            /*!< Page Down (VK_NEXT) */
            RZKEY_UP = 0x0410,                  /*!< Up (VK_UP) */
            RZKEY_LEFT = 0x050F,                /*!< Left (VK_LEFT) */
            RZKEY_DOWN = 0x0510,                /*!< Down (VK_DOWN) */
            RZKEY_RIGHT = 0x0511,               /*!< Right (VK_RIGHT) */
            RZKEY_TAB = 0x0201,                 /*!< Tab (VK_TAB) */
            RZKEY_CAPSLOCK = 0x0301,            /*!< Caps Lock(VK_CAPITAL) */
            RZKEY_BACKSPACE = 0x010E,           /*!< Backspace (VK_BACK) */
            RZKEY_ENTER = 0x030E,               /*!< Enter (VK_RETURN) */
            RZKEY_LCTRL = 0x0501,               /*!< Left Control(VK_LCONTROL) */
            RZKEY_LWIN = 0x0502,                /*!< Left Window (VK_LWIN) */
            RZKEY_LALT = 0x0503,                /*!< Left Alt (VK_LMENU) */
            RZKEY_SPACE = 0x0507,               /*!< Spacebar (VK_SPACE) */
            RZKEY_RALT = 0x050B,                /*!< Right Alt (VK_RMENU) */
            RZKEY_FN = 0x050C,                  /*!< Function key. */
            RZKEY_RMENU = 0x050D,               /*!< Right Menu (VK_APPS) */
            RZKEY_RCTRL = 0x050E,               /*!< Right Control (VK_RCONTROL) */
            RZKEY_LSHIFT = 0x0401,              /*!< Left Shift (VK_LSHIFT) */
            RZKEY_RSHIFT = 0x040E,              /*!< Right Shift (VK_RSHIFT) */
            RZKEY_MACRO1 = 0x0100,              /*!< Macro Key 1 */
            RZKEY_MACRO2 = 0x0200,              /*!< Macro Key 2 */
            RZKEY_MACRO3 = 0x0300,              /*!< Macro Key 3 */
            RZKEY_MACRO4 = 0x0400,              /*!< Macro Key 4 */
            RZKEY_MACRO5 = 0x0500,              /*!< Macro Key 5 */
            RZKEY_OEM_1 = 0x0101,               /*!< ~ (tilde/半角/全角) (VK_OEM_3) */
            RZKEY_OEM_2 = 0x010C,               /*!< -- (minus) (VK_OEM_MINUS) */
            RZKEY_OEM_3 = 0x010D,               /*!< = (equal) (VK_OEM_PLUS) */
            RZKEY_OEM_4 = 0x020C,               /*!< [ (left sqaure bracket) (VK_OEM_4) */
            RZKEY_OEM_5 = 0x020D,               /*!< ] (right square bracket) (VK_OEM_6) */
            RZKEY_OEM_6 = 0x020E,               /*!< \ (backslash) (VK_OEM_5) */
            RZKEY_OEM_7 = 0x030B,               /*!< ; (semi-colon) (VK_OEM_1) */
            RZKEY_OEM_8 = 0x030C,               /*!< ' (apostrophe) (VK_OEM_7) */
            RZKEY_OEM_9 = 0x040A,               /*!< , (comma) (VK_OEM_COMMA) */
            RZKEY_OEM_10 = 0x040B,              /*!< . (period) (VK_OEM_PERIOD) */
            RZKEY_OEM_11 = 0x040C,              /*!< / (forward slash) (VK_OEM_2) */
            RZKEY_EUR_1 = 0x030D,               /*!< "#" (VK_OEM_5) */
            RZKEY_EUR_2 = 0x0402,               /*!< \ (VK_OEM_102) */
            RZKEY_JPN_1 = 0x0015,               /*!< ¥ (0xFF) */
            RZKEY_JPN_2 = 0x040D,               /*!< \ (0xC1) */
            RZKEY_JPN_3 = 0x0504,               /*!< 無変換 (VK_OEM_PA1) */
            RZKEY_JPN_4 = 0x0509,               /*!< 変換 (0xFF) */
            RZKEY_JPN_5 = 0x050A,               /*!< ひらがな/カタカナ (0xFF) */
            RZKEY_KOR_1 = 0x0015,               /*!< | (0xFF) */
            RZKEY_KOR_2 = 0x030D,               /*!< (VK_OEM_5) */
            RZKEY_KOR_3 = 0x0402,               /*!< (VK_OEM_102) */
            RZKEY_KOR_4 = 0x040D,               /*!< (0xC1) */
            RZKEY_KOR_5 = 0x0504,               /*!< (VK_OEM_PA1) */
            RZKEY_KOR_6 = 0x0509,               /*!< 한/영 (0xFF) */
            RZKEY_KOR_7 = 0x050A,               /*!< (0xFF) */
            RZKEY_INVALID = 0xFFFF              /*!< Invalid keys. */
        } RZKEY;

        //! Definition of LEDs.
        typedef enum RZLED
        {
            RZLED_LOGO = 0x0014                 /*!< Razer logo */
        } RZLED;

        //! Maximum number of rows in a keyboard.
        const RZSIZE MAX_ROW = 6;

        //! Maximum number of columns in a keyboard.
        const RZSIZE MAX_COLUMN = 22;

        //! Maximum number of keys.
        const RZSIZE MAX_KEYS = MAX_ROW * MAX_COLUMN;

        //! Maximum number of custom effects.
        const RZSIZE MAX_CUSTOM_EFFECTS = MAX_KEYS;

        //! Keyboard LED layout.
        const COLORREF RZKEY_LAYOUT[MAX_ROW][MAX_COLUMN] = {};

        //! Chroma keyboard effect types
        typedef enum EFFECT_TYPE
        {
            CHROMA_NONE = 0,            //!< No effect.
            CHROMA_BREATHING,           //!< Breathing effect (Deprecated and should not be used.).
            CHROMA_CUSTOM,              //!< Custom effect.
            CHROMA_REACTIVE,            //!< Reactive effect (Deprecated and should not be used).
            CHROMA_STATIC,              //!< Static single color effect.
            CHROMA_SPECTRUMCYCLING,     //!< Spectrum cycling effect (Deprecated and should not be used).
            CHROMA_WAVE,                //!< Wave effect (Deprecated and should not be used).
            CHROMA_RESERVED,            //!< Reserved.
            CHROMA_CUSTOM_KEY,          //!< Custom effects with keys.
            CHROMA_INVALID              //!< Invalid effect.
        } EFFECT_TYPE;

        // Chroma keyboard effects
        //! Breathing effect type (Deprecated and should not be used).
        typedef struct BREATHING_EFFECT_TYPE
        {
            //! Breathing effects.
            enum Type
            {
                TWO_COLORS = 1,     //!< 2 colors
                RANDOM_COLORS,      //!< Random colors
                INVALID             //!< Invalid type
            } Type;
            COLORREF Color1;        //!< First color.
            COLORREF Color2;        //!< Second color.
        } BREATHING_EFFECT_TYPE;

        //! Custom effect using a matrix type.
        typedef struct CUSTOM_EFFECT_TYPE
        {
            COLORREF Color[MAX_ROW][MAX_COLUMN];      //!< Grid layout. 6 rows by 22 columns.
        } CUSTOM_EFFECT_TYPE;

        //! Custom effect with keys.
        typedef struct CUSTOM_KEY_EFFECT_TYPE
        {
            COLORREF Color[MAX_ROW][MAX_COLUMN];        //!< Grid layout. 6 rows by 22 columns.
            COLORREF Key[MAX_ROW][MAX_COLUMN];          //!< Keys information. 6 rows by 22 columns. To indidate there is a key effect, OR with 0x01000000. i.e. Key[0][1] = 0x01000000 | Color;
        } CUSTOM_KEY_EFFECT_TYPE;

        //! Reactive effect type (Deprecated and should not be used).
        typedef struct REACTIVE_EFFECT_TYPE
        {
            //! Duration of the effect.
            enum Duration
            {
                DURATION_NONE=0,    //!< No duration.
                DURATION_SHORT,     //!< Short duration.
                DURATION_MEDIUM,    //!< Medium duration.
                DURATION_LONG,      //!< Long duration.
                DURATION_INVALID    //!< Invalid duration.
            } Duration;             //!< The time taken for the effect to fade away.

            COLORREF Color;         //!< Color of the effect
        } REACTIVE_EFFECT_TYPE;

        //! Starlight effect (Deprecated and should not be used).
        typedef struct STARLIGHT_EFFECT_TYPE
        {
            //! Starlight effect types.
            enum _Type
            {
                TWO_COLORS = 1, //!< 2 colors.
                RANDOM_COLORS   //!< Random colors
            } Type;

            COLORREF Color1;    //!< First color.
            COLORREF Color2;    //!< Second color.

            //! Duration of the effect.
            enum _Duration
            {
                DURATION_SHORT = 1, //!< Short duration.
                DURATION_MEDIUM,    //!< Medium duration.
                DURATION_LONG       //!< Long duration.
            } Duration;             //!< The time taken for the effect to fade away.

        } STARLIGHT_EFFECT_TYPE;

        //! Static effect type
        typedef struct STATIC_EFFECT_TYPE
        {
            COLORREF Color;     //!< Color of the effect
        } STATIC_EFFECT_TYPE;

        //! Wave effect type (Deprecated and should not be used).
        typedef struct WAVE_EFFECT_TYPE
        {
            //! Direction of the wave effect.
            enum Direction
            {
                DIRECTION_NONE=0,           //!< No direction.
                DIRECTION_LEFT_TO_RIGHT,    //!< Left to right.
                DIRECTION_RIGHT_TO_LEFT,    //!< Right to left.
                DIRECTION_INVALID           //!< Invalid direction.
            } Direction;                    //!< Direction of the wave.
        } WAVE_EFFECT_TYPE;
    }

    //! Mice
    namespace Mouse
    {
        //! Maximum number of custom LEDs (old definition to maintain backward compatibility).
        const RZSIZE MAX_LEDS = 30;

        //! Mice LED layout (old definition to maintain backward compatibility).
        const RZCOLOR RZLED_LAYOUT[MAX_LEDS] = {};

        //! Maximum number of rows of the virtual grid.
        const RZSIZE MAX_ROW = 9;

        //! Maximum number of columns of the virtual grid.
        const RZSIZE MAX_COLUMN = 7;

        //! Maximum number of LEDs of the virtual grid.
        const RZSIZE MAX_LEDS2 = MAX_ROW * MAX_COLUMN;

        //! Mice LED virtual grid layout.
        const RZCOLOR RZLED_LAYOUT2[MAX_ROW][MAX_COLUMN] = {};

        //! Mouse LED Id defintion (Deprecated and should not be used).
        typedef enum RZLED
        {
            RZLED_NONE          = 0,    //!< No LED.
            RZLED_SCROLLWHEEL   = 1,    //!< Scroll Wheel LED.
            RZLED_LOGO          = 2,    //!< Logo LED.
            RZLED_BACKLIGHT     = 3,    //!< Backlight or numpad.
            RZLED_SIDE_STRIP1   = 4,    //!< Side strip LED 1. (For Mamba TE, starts from top left hand)
            RZLED_SIDE_STRIP2   = 5,    //!< Side strip LED 2. (For Mamba TE)
            RZLED_SIDE_STRIP3   = 6,    //!< Side strip LED 3. (For Mamba TE)
            RZLED_SIDE_STRIP4   = 7,    //!< Side strip LED 4. (For Mamba TE)
            RZLED_SIDE_STRIP5   = 8,    //!< Side strip LED 5. (For Mamba TE)
            RZLED_SIDE_STRIP6   = 9,    //!< Side strip LED 6. (For Mamba TE)
            RZLED_SIDE_STRIP7   = 10,   //!< Side strip LED 7. (For Mamba TE)
            RZLED_SIDE_STRIP8   = 11,   //!< Side strip LED 8. (For Mamba TE)
            RZLED_SIDE_STRIP9   = 12,   //!< Side strip LED 9. (For Mamba TE)
            RZLED_SIDE_STRIP10  = 13,   //!< Side strip LED 10. (For Mamba TE)
            RZLED_SIDE_STRIP11  = 14,   //!< Side strip LED 11. (For Mamba TE)
            RZLED_SIDE_STRIP12  = 15,   //!< Side strip LED 12. (For Mamba TE)
            RZLED_SIDE_STRIP13  = 16,   //!< Side strip LED 13. (For Mamba TE)
            RZLED_SIDE_STRIP14  = 17,   //!< Side strip LED 14. (For Mamba TE)
            RZLED_ALL           = 0xFFFF
        } RZLED;

        //! Mouse LED Id defintion for the virtual grid.
        typedef enum RZLED2
        {
            RZLED2_SCROLLWHEEL   = 0x0203,  //!< Scroll Wheel LED.
            RZLED2_LOGO          = 0x0703,  //!< Logo LED.
            RZLED2_BACKLIGHT     = 0x0403,  //!< Backlight LED.
            RZLED2_LEFT_SIDE1    = 0x0100,  //!< Left LED 1.
            RZLED2_LEFT_SIDE2    = 0x0200,  //!< Left LED 2.
            RZLED2_LEFT_SIDE3    = 0x0300,  //!< Left LED 3.
            RZLED2_LEFT_SIDE4    = 0x0400,  //!< Left LED 4.
            RZLED2_LEFT_SIDE5    = 0x0500,  //!< Left LED 5.
            RZLED2_LEFT_SIDE6    = 0x0600,  //!< Left LED 6.
            RZLED2_LEFT_SIDE7    = 0x0700,  //!< Left LED 7.
            RZLED2_BOTTOM1       = 0x0801,  //!< Bottom LED 1.
            RZLED2_BOTTOM2       = 0x0802,  //!< Bottom LED 2.
            RZLED2_BOTTOM3       = 0x0803,  //!< Bottom LED 3.
            RZLED2_BOTTOM4       = 0x0804,  //!< Bottom LED 4.
            RZLED2_BOTTOM5       = 0x0805,  //!< Bottom LED 5.
            RZLED2_RIGHT_SIDE1   = 0x0106,  //!< Right LED 1.
            RZLED2_RIGHT_SIDE2   = 0x0206,  //!< Right LED 2.
            RZLED2_RIGHT_SIDE3   = 0x0306,  //!< Right LED 3.
            RZLED2_RIGHT_SIDE4   = 0x0406,  //!< Right LED 4.
            RZLED2_RIGHT_SIDE5   = 0x0506,  //!< Right LED 5.
            RZLED2_RIGHT_SIDE6   = 0x0606,  //!< Right LED 6.
            RZLED2_RIGHT_SIDE7   = 0x0706   //!< Right LED 7.
        } RZLED2;

        //! Chroma mouse effect types
        typedef enum EFFECT_TYPE
        {
            CHROMA_NONE = 0,            //!< No effect.
            CHROMA_BLINKING,            //!< Blinking effect (Deprecated and should not be used).
            CHROMA_BREATHING,           //!< Breathing effect (Deprecated and should not be used).
            CHROMA_CUSTOM,              //!< Custom effect (Deprecated and should not be used).
            CHROMA_REACTIVE,            //!< Reactive effect (Deprecated and should not be used).
            CHROMA_SPECTRUMCYCLING,     //!< Spectrum cycling effect (Deprecated and should not be used).
            CHROMA_STATIC,              //!< Static single color effect.
            CHROMA_WAVE,                //!< Wave effect (Deprecated and should not be used).
            CHROMA_CUSTOM2,             //!< Custom effects using a virtual grid.
            CHROMA_INVALID              //!< Invalid effect.
        } EFFECT_TYPE;

        //! Static effect type
        typedef struct STATIC_EFFECT_TYPE
        {
            RZLED LEDId;        //!< LED Id
            COLORREF Color;     //!< Color of the effect.
        } STATIC_EFFECT_TYPE;

        //! Blinking effect type (Deprecated and should not be used).
        typedef struct BLINKING_EFFECT_TYPE
        {
            RZLED LEDId;        //!< LED Id
            COLORREF Color;     //!< Color.
        } BLINKING_EFFECT_TYPE;

        //! Breathing effect (Deprecated and should not be used).
        typedef struct BREATHING_EFFECT_TYPE
        {
            RZLED LEDId;        //!< LED Id

            //! Breathing type.
            enum Type
            {
                ONE_COLOR = 1,  //!< 1 color (Only fill Color1).
                TWO_COLORS,     //!< 2 colors.
                RANDOM_COLORS,  //!< Random colors
                INVALID         //!< Invalid type
            } Type;

            COLORREF Color1;    //!< First color.
            COLORREF Color2;    //!< Second color.
        } BREATHING_EFFECT_TYPE;

        //! Custom effect (Deprecated and should not be used).
        typedef struct CUSTOM_EFFECT_TYPE
        {
            RZCOLOR Color[MAX_LEDS]; //!< Array of colors.
        } CUSTOM_EFFECT_TYPE;

        //! Custom effect using virtual grid. 
        //! Indexes of the LED are defined in RZLED2.i.e. Row = HIBYTE(RZLED2_SCROLLWHEEL), Column = LOBYTE(RZLED2_SCROLLWHEEL)
        typedef struct CUSTOM_EFFECT_TYPE2
        {
            RZCOLOR Color[MAX_ROW][MAX_COLUMN]; //!< Array of colors.
        } CUSTOM_EFFECT_TYPE2;

        //! Reactive effect (Deprecated and should not be used).
        typedef struct REACTIVE_EFFECT_TYPE
        {
            RZLED LEDId;        //!< LED Id

            //! Duration of the effect.
            enum Duration
            {
                DURATION_NONE=0,    //!< No duration.
                DURATION_SHORT,     //!< Short duration.
                DURATION_MEDIUM,    //!< Medium duration.
                DURATION_LONG       //!< Long duration.
            } Duration;

            RZCOLOR Color;          //!< Color of the effect.
        } REACTIVE_EFFECT_TYPE;

        //! No effect (Deprecated and should not be used).
        typedef struct NO_EFFECT_TYPE
        {
            RZLED LEDId;        //!< LED Id
        } NO_EFFECT_TYPE;

        //! Spectrum cycling (Deprecated and should not be used).
        typedef struct SPECTRUMCYCLING_EFFECT_TYPE
        {
            RZLED LEDId;            //!< LED id.
        } SPECTRUMCYCLING_EFFECT_TYPE;

        //! Wave effect (Deprecated and should not be used).
        typedef struct WAVE_EFFECT_TYPE
        {
            //! Direction of the wave effect.
            enum Direction
            {
                FRONT_TO_BACK,      //!< Front to back
                BACK_TO_FRONT       //!< Back to front
            } Direction;
        } WAVE_EFFECT_TYPE;
    }

    //! Headsets
    namespace Headset
    {
        //! Maximum number of LEDs
        const RZSIZE MAX_LEDS = 5;

        //! Chroma headset effect types
        typedef enum EFFECT_TYPE
        {
            CHROMA_NONE = 0,            //!< No effect.
            CHROMA_STATIC,              //!< Static single color effect.
            CHROMA_BREATHING,           //!< Breathing effect (Deprecated and should not be used).
            CHROMA_SPECTRUMCYCLING,     //!< Spectrum cycling effect (Deprecated and should not be used).
            CHROMA_CUSTOM,              //!< Custom effects.
            CHROMA_INVALID              //!< Invalid effect.
        } EFFECT_TYPE;

        //! Static effect type
        typedef struct STATIC_EFFECT_TYPE
        {
            COLORREF Color;             //!< Color of the effect.
        } STATIC_EFFECT_TYPE;

        //! Breathing effect type (Deprecated and should not be used).
        typedef struct BREATHING_EFFECT_TYPE
        {
            COLORREF Color;             //!< Color.
        } BREATHING_EFFECT_TYPE;

        //! Custom effect type.
        typedef struct CUSTOM_EFFECT_TYPE
        {
            RZCOLOR Color[MAX_LEDS];    //!< Array of colors.
        } CUSTOM_EFFECT_TYPE;
    }

    //! Mousepads
    namespace Mousepad
    {
        //! Maximum number of LEDs
        const RZSIZE MAX_LEDS = 15;

        //! Chroma mousepad effect types
        typedef enum EFFECT_TYPE
        {
            CHROMA_NONE = 0,            //!< No effect.
            CHROMA_BREATHING,           //!< Breathing effect (Deprecated and should not be used).
            CHROMA_CUSTOM,              //!< Custom effect.
            CHROMA_SPECTRUMCYCLING,     //!< Spectrum cycling effect (Deprecated and should not be used).
            CHROMA_STATIC,              //!< Static single color effect.
            CHROMA_WAVE,                //!< Wave effect (Deprecated and should not be used).
            CHROMA_INVALID              //!< Invalid effect.
        } EFFECT_TYPE;

        // Chroma mousepad effects
        //! Breathing effect type (Deprecated and should not be used).
        typedef struct BREATHING_EFFECT_TYPE
        {
            //! Breathing effects.
            enum Type
            {
                TWO_COLORS = 1,     //!< 2 colors
                RANDOM_COLORS,      //!< Random colors
                INVALID
            } Type;
            COLORREF Color1;    //!< First color.
            COLORREF Color2;    //!< Second color.
        } BREATHING_EFFECT_TYPE;

        //! Custom effect type.
        typedef struct CUSTOM_EFFECT_TYPE
        {
            RZCOLOR Color[MAX_LEDS];    //!< An array of colors for all the sides of the mousepad. First LED starts from top-right corner.
                                        //!< LED 0-4 right side, 5-9 bottom side, 10-14 left side.
        } CUSTOM_EFFECT_TYPE;

        //! Static effect type
        typedef struct STATIC_EFFECT_TYPE
        {
            COLORREF Color;     //!< Color of the effect
        } STATIC_EFFECT_TYPE;

        //! Wave effect type
        typedef struct WAVE_EFFECT_TYPE
        {
            //! Direction of the wave effect.
            enum Direction
            {
                DIRECTION_NONE=0,           //!< No direction.
                DIRECTION_LEFT_TO_RIGHT,    //!< Left to right.
                DIRECTION_RIGHT_TO_LEFT,    //!< Right to left.
                DIRECTION_INVALID           //!< Invalid direction.
            } Direction;                    //!< Direction of the wave.
        } WAVE_EFFECT_TYPE;
    }

    //! Keypads
    namespace Keypad
    {
        //! Maximum number of rows.
        const RZSIZE MAX_ROW = 4;

        //! Maximum number of columns.
        const RZSIZE MAX_COLUMN = 5;

        //! Total number of keys.
        const RZSIZE MAX_KEYS = MAX_ROW * MAX_COLUMN;

        //! Chroma keypad effect types
        typedef enum EFFECT_TYPE
        {
            CHROMA_NONE = 0,            //!< No effect.
            CHROMA_BREATHING,           //!< Breathing effect (Deprecated and should not be used).
            CHROMA_CUSTOM,              //!< Custom effect.
            CHROMA_REACTIVE,            //!< Reactive effect (Deprecated and should not be used).
            CHROMA_SPECTRUMCYCLING,     //!< Spectrum cycling effect (Deprecated and should not be used).
            CHROMA_STATIC,              //!< Static single color effect.
            CHROMA_WAVE,                //!< Wave effect (Deprecated and should not be used).
            CHROMA_INVALID              //!< Invalid effect.
        } EFFECT_TYPE;

        // Chroma keypad effects
        //! Breathing effect type (Deprecated and should not be used).
        typedef struct BREATHING_EFFECT_TYPE
        {
            //! Breathing effects.
            enum Type
            {
                TWO_COLORS = 1,     //!< 2 colors
                RANDOM_COLORS,      //!< Random colors
                INVALID             //!< Invalid type
            } Type;
            COLORREF Color1;    //!< First color.
            COLORREF Color2;    //!< Second color.
        } BREATHING_EFFECT_TYPE;

        //! Custom effect type
        typedef struct CUSTOM_EFFECT_TYPE
        {
            RZCOLOR Color[MAX_ROW][MAX_COLUMN]; //!< Custom effect.
                                                //!< For Razer Tartarus Chroma only Color[0] is valid. Use index '0' to change the keypad color.
        } CUSTOM_EFFECT_TYPE;

        //! Reactive effect type (Deprecated and should not be used).
        typedef struct REACTIVE_EFFECT_TYPE
        {
            //! Duration of the effect.
            enum Duration
            {
                DURATION_NONE=0,    //!< No duration.
                DURATION_SHORT,     //!< Short duration.
                DURATION_MEDIUM,    //!< Medium duration.
                DURATION_LONG,      //!< Long duration.
                DURATION_INVALID    //!< Invalid duration.
            } Duration;             //!< The time taken for the effect to fade away.

            COLORREF Color;         //!< Color of the effect
        } REACTIVE_EFFECT_TYPE;

        //! Static effect type
        typedef struct STATIC_EFFECT_TYPE
        {
            RZCOLOR Color;  //!< Color of the effect.
        } STATIC_EFFECT_TYPE;

        //! Wave effect type (Deprecated and should not be used).
        typedef struct WAVE_EFFECT_TYPE
        {
            //! Direction of the wave effect.
            enum Direction
            {
                DIRECTION_NONE=0,           //!< No direction.
                DIRECTION_LEFT_TO_RIGHT,    //!< Left to right.
                DIRECTION_RIGHT_TO_LEFT,    //!< Right to left.
                DIRECTION_INVALID           //!< Invalid direction.
            } Direction;                    //!< Direction of the wave.
        } WAVE_EFFECT_TYPE;
    }

    //! Chroma Link
    namespace ChromaLink
    {
        //! Maximum number of elements/LEDs
        const RZSIZE MAX_LEDS = 5;

        //! Chroma Link effect types
        typedef enum EFFECT_TYPE
        {
            CHROMA_NONE = 0,            //!< No effect.
            CHROMA_CUSTOM,              //!< Custom effect.
            CHROMA_STATIC,              //!< Static single color effect.
            CHROMA_INVALID              //!< Invalid effect.
        } EFFECT_TYPE;

        //! Custom effect type.\n
        //! Use Custom type to create a sequence of animated effects.\n
        //! [ChromaLinkL#1|ChromaLinkL#2|ChromaLinkL#3|ChromaLinkL#4|ChromaLinkL#5].\n
        typedef struct CUSTOM_EFFECT_TYPE
        {
            RZCOLOR Color[MAX_LEDS];    //!< Array of colors. 
        } CUSTOM_EFFECT_TYPE;

        //! Static effect type.\n
        //! Use Static type to create effects for all LEDs (ChromaLinkL#1).
        typedef struct STATIC_EFFECT_TYPE
        {
            RZCOLOR Color;  //!< Color of the effect.
        } STATIC_EFFECT_TYPE;
    }
}

#endif
