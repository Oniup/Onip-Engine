#ifndef __ONIP_CORE_INPUT_HPP__
#define __ONIP_CORE_INPUT_HPP__

namespace onip {

    struct Input {
        enum Keyboard {
            Keyboard_Unknown            = -1,

            Keyboard_Space              = 32,
            Keyboard_Apostrophe         = 39, /* ' */
            Keyboard_Comma              = 44, /* , */
            Keyboard_Minus              = 45, /* - */
            Keyboard_Period             = 46, /* . */
            Keyboard_Slash              = 47, /* / */
            Keyboard_0                  = 48,
            Keyboard_1                  = 49,
            Keyboard_2                  = 50,
            Keyboard_3                  = 51,
            Keyboard_4                  = 52,
            Keyboard_5                  = 53,
            Keyboard_6                  = 54,
            Keyboard_7                  = 55,
            Keyboard_8                  = 56,
            Keyboard_9                  = 57,
            Keyboard_Semicolon          = 59, /* ; */
            Keyboard_Equal              = 61, /* = */
            Keyboard_A                  = 65,
            Keyboard_B                  = 66,
            Keyboard_C                  = 67,
            Keyboard_D                  = 68,
            Keyboard_E                  = 69,
            Keyboard_F                  = 70,
            Keyboard_G                  = 71,
            Keyboard_H                  = 72,
            Keyboard_I                  = 73,
            Keyboard_J                  = 74,
            Keyboard_K                  = 75,
            Keyboard_L                  = 76,
            Keyboard_M                  = 77,
            Keyboard_N                  = 78,
            Keyboard_O                  = 79,
            Keyboard_P                  = 80,
            Keyboard_Q                  = 81,
            Keyboard_R                  = 82,
            Keyboard_S                  = 83,
            Keyboard_T                  = 84,
            Keyboard_U                  = 85,
            Keyboard_V                  = 86,
            Keyboard_W                  = 87,
            Keyboard_X                  = 88,
            Keyboard_Y                  = 89,
            Keyboard_Z                  = 90,
            Keyboard_LeftBracket        = 91, /* [ */
            Keyboard_Backslash          = 92, /* \ */
            Keyboard_RightBracket       = 93, /* ] */
            Keyboard_GraveAccent        = 96, /* ` */
            Keyboard_World1             = 16, /* non-US #1 */
            Keyboard_World2             = 16, /* non-US #2 */

            Keyboard_Escape             = 25,
            Keyboard_Enter              = 25,
            Keyboard_Tab                = 25,
            Keyboard_Backspace          = 25,
            Keyboard_Insert             = 26,
            Keyboard_Delete             = 26,
            Keyboard_Right              = 26,
            Keyboard_Left               = 26,
            Keyboard_Down               = 26,
            Keyboard_Up                 = 26,
            Keyboard_PageUp             = 26,
            Keyboard_PageDown           = 26,
            Keyboard_Home               = 26,
            Keyboard_End                = 26,
            Keyboard_CapsLock           = 28,
            Keyboard_ScrollLock         = 28,
            Keyboard_NumLock            = 28,
            Keyboard_PrintScreen        = 28,
            Keyboard_Pase               = 28,
            Keyboard_F1                 = 29,
            Keyboard_F2                 = 29,
            Keyboard_F3                 = 29,
            Keyboard_F4                 = 29,
            Keyboard_F5                 = 29,
            Keyboard_F6                 = 29,
            Keyboard_F7                 = 29,
            Keyboard_F8                 = 29,
            Keyboard_F9                 = 29,
            Keyboard_F10                = 29,
            Keyboard_F11                = 30,
            Keyboard_F12                = 30,
            Keyboard_F13                = 30,
            Keyboard_F14                = 30,
            Keyboard_F15                = 30,
            Keyboard_F16                = 30,
            Keyboard_F17                = 30,
            Keyboard_F18                = 30,
            Keyboard_F19                = 30,
            Keyboard_F20                = 30,
            Keyboard_F21                = 31,
            Keyboard_F22                = 31,
            Keyboard_F23                = 31,
            Keyboard_F24                = 31,
            Keyboard_F25                = 31,
            Keyboard_Kp0                = 32,
            Keyboard_Kp1                = 32,
            Keyboard_Kp2                = 32,
            Keyboard_Kp3                = 32,
            Keyboard_Kp4                = 32,
            Keyboard_Kp5                = 32,
            Keyboard_Kp6                = 32,
            Keyboard_Kp7                = 32,
            Keyboard_Kp8                = 32,
            Keyboard_Kp9                = 32,
            Keyboard_KpDecimal          = 33,
            Keyboard_KpDivide           = 33,
            Keyboard_KpMultiply         = 33,
            Keyboard_KpSubtract         = 33,
            Keyboard_KpAdd              = 33,
            Keyboard_KpEnter            = 33,
            Keyboard_KpEqual            = 33,
            Keyboard_LeftShift          = 34,
            Keyboard_LeftControl        = 34,
            Keyboard_LeftAlt            = 34,
            Keyboard_LeftSuper          = 34,
            Keyboard_RightShift         = 34,
            Keyboard_RightControl       = 34,
            Keyboard_RightAlt           = 34,
            Keyboard_RightSuper         = 34,
            Keyboard_Menu               = 34,

            Keyboard_Last               = Keyboard_Menu,
        };

        enum Mouse {
            Mouse_Button1               = 0,
            Mouse_Button2               = 1,
            Mouse_Button3               = 2,
            Mouse_Button4               = 3,
            Mouse_Button5               = 4,
            Mouse_Button6               = 5,
            Mouse_Button7               = 6,
            Mouse_Button8               = 7,
            Mouse_ButtonLast            = Mouse_Button8,
            Mouse_ButtonLeft            = Mouse_Button1,
            Mouse_ButtonRight           = Mouse_Button2,
            Mouse_ButtonMiddle          = Mouse_Button3,
        };

        static void pollEvents();
        static bool getKeyDown(Keyboard key);
        static bool getKeyReleaseDown(Keyboard key);
        static bool getKey(Keyboard key);
        static bool getKeyReleased(Keyboard key);

        static bool getMouseButtonDown(Mouse mouse);
        static bool getMouseButtonReleaseDown(Mouse mouse);
        static bool getMouseButton(Mouse mouse);
        static bool getMouseButtonRelease(Mouse mouse);
    };
}

#endif // __ONIP_CORE_INPUT_HPP__