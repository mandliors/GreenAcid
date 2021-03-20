#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

#define GLFW_KEY_COUNT 120

/* Printable keys */
#define OX_KEY_SPACE              0
#define OX_KEY_APOSTROPHE         1  /* ' */
#define OX_KEY_COMMA              2  /* , */
#define OX_KEY_MINUS              3  /* - */
#define OX_KEY_PERIOD             4  /* . */
#define OX_KEY_SLASH              5  /* / */
#define OX_KEY_0                  6
#define OX_KEY_1                  7
#define OX_KEY_2                  8
#define OX_KEY_3                  9
#define OX_KEY_4                  10
#define OX_KEY_5                  11
#define OX_KEY_6                  12
#define OX_KEY_7                  13
#define OX_KEY_8                  14
#define OX_KEY_9                  15
#define OX_KEY_SEMICOLON          16  /* ; */
#define OX_KEY_EQUAL              17 /* = */
#define OX_KEY_A                  18
#define OX_KEY_B                  19
#define OX_KEY_C                  20
#define OX_KEY_D                  21
#define OX_KEY_E                  22
#define OX_KEY_F                  23
#define OX_KEY_G                  24
#define OX_KEY_H                  25
#define OX_KEY_I                  26
#define OX_KEY_J                  27
#define OX_KEY_K                  28
#define OX_KEY_L                  29
#define OX_KEY_M                  30
#define OX_KEY_N                  31
#define OX_KEY_O                  32
#define OX_KEY_P                  33
#define OX_KEY_Q                  34
#define OX_KEY_R                  35
#define OX_KEY_S                  36
#define OX_KEY_T                  37
#define OX_KEY_U                  38
#define OX_KEY_V                  39
#define OX_KEY_W                  40
#define OX_KEY_X                  41
#define OX_KEY_Y                  42
#define OX_KEY_Z                  43
#define OX_KEY_LEFT_BRACKET       44  /* [ */
#define OX_KEY_BACKSLASH          45  /* \ */
#define OX_KEY_RIGHT_BRACKET      46  /* ] */
#define OX_KEY_GRAVE_ACCENT       47  /* ` */
#define OX_KEY_WORLD_1            48 /* non-US #1 */
#define OX_KEY_WORLD_2            49 /* non-US #2 */

/* Function keys */
#define OX_KEY_ESCAPE             50
#define OX_KEY_ENTER              51
#define OX_KEY_TAB                52
#define OX_KEY_BACKSPACE          53
#define OX_KEY_INSERT             54
#define OX_KEY_DELETE             55
#define OX_KEY_RIGHT              56
#define OX_KEY_LEFT               57
#define OX_KEY_DOWN               58
#define OX_KEY_UP                 59
#define OX_KEY_PAGE_UP            60
#define OX_KEY_PAGE_DOWN          61
#define OX_KEY_HOME               62
#define OX_KEY_END                63
#define OX_KEY_CAPS_LOCK          64
#define OX_KEY_SCROLL_LOCK        65
#define OX_KEY_NUM_LOCK           66
#define OX_KEY_PRINT_SCREEN       67
#define OX_KEY_PAUSE              68
#define OX_KEY_F1                 69
#define OX_KEY_F2                 70
#define OX_KEY_F3                 71
#define OX_KEY_F4                 72
#define OX_KEY_F5                 73
#define OX_KEY_F6                 74
#define OX_KEY_F7                 75
#define OX_KEY_F8                 76
#define OX_KEY_F9                 77
#define OX_KEY_F10                78
#define OX_KEY_F11                79
#define OX_KEY_F12                80
#define OX_KEY_F13                81
#define OX_KEY_F14                82
#define OX_KEY_F15                83
#define OX_KEY_F16                84
#define OX_KEY_F17                85
#define OX_KEY_F18                86
#define OX_KEY_F19                87
#define OX_KEY_F20                88
#define OX_KEY_F21                89
#define OX_KEY_F22                90
#define OX_KEY_F23                91
#define OX_KEY_F24                92
#define OX_KEY_F25                93
#define OX_KEY_KP_0               94
#define OX_KEY_KP_1               95
#define OX_KEY_KP_2               96
#define OX_KEY_KP_3               97
#define OX_KEY_KP_4               98
#define OX_KEY_KP_5               99
#define OX_KEY_KP_6               100
#define OX_KEY_KP_7               101
#define OX_KEY_KP_8               102
#define OX_KEY_KP_9               103
#define OX_KEY_KP_DECIMAL         104
#define OX_KEY_KP_DIVIDE          105
#define OX_KEY_KP_MULTIPLY        106
#define OX_KEY_KP_SUBTRACT        107
#define OX_KEY_KP_ADD             108
#define OX_KEY_KP_ENTER           109
#define OX_KEY_KP_EQUAL           110
#define OX_KEY_LEFT_SHIFT         111
#define OX_KEY_LEFT_CONTROL       112
#define OX_KEY_LEFT_ALT           113
#define OX_KEY_LEFT_SUPER         114
#define OX_KEY_RIGHT_SHIFT        115
#define OX_KEY_RIGHT_CONTROL      116
#define OX_KEY_RIGHT_ALT          117
#define OX_KEY_RIGHT_SUPER        118
#define OX_KEY_MENU               119

const std::map<int, int> KEYCODE_DEFINITIONS
{
	{ GLFW_KEY_SPACE,			OX_KEY_SPACE },
	{ GLFW_KEY_APOSTROPHE,		OX_KEY_APOSTROPHE },
	{ GLFW_KEY_COMMA,			OX_KEY_COMMA },
	{ GLFW_KEY_MINUS,			OX_KEY_MINUS },
	{ GLFW_KEY_PERIOD,			OX_KEY_PERIOD },
	{ GLFW_KEY_SLASH,			OX_KEY_SLASH },
	{ GLFW_KEY_0,				OX_KEY_0 },
	{ GLFW_KEY_1,				OX_KEY_1 },
	{ GLFW_KEY_2,				OX_KEY_2 },
	{ GLFW_KEY_3,				OX_KEY_3 },
	{ GLFW_KEY_4,				OX_KEY_4 },
	{ GLFW_KEY_5,				OX_KEY_5 },
	{ GLFW_KEY_6,				OX_KEY_6 },
	{ GLFW_KEY_7,				OX_KEY_7 },
	{ GLFW_KEY_8,				OX_KEY_8 },
	{ GLFW_KEY_9,				OX_KEY_9 },
	{ GLFW_KEY_SEMICOLON,		OX_KEY_SEMICOLON },
	{ GLFW_KEY_EQUAL,			OX_KEY_EQUAL },
	{ GLFW_KEY_A,				OX_KEY_A },
	{ GLFW_KEY_B,				OX_KEY_B },
	{ GLFW_KEY_C,				OX_KEY_C },
	{ GLFW_KEY_D,				OX_KEY_D },
	{ GLFW_KEY_E,				OX_KEY_E },
	{ GLFW_KEY_F,				OX_KEY_F },
	{ GLFW_KEY_G,				OX_KEY_G },
	{ GLFW_KEY_H,				OX_KEY_H },
	{ GLFW_KEY_I,				OX_KEY_I },
	{ GLFW_KEY_J,				OX_KEY_J },
	{ GLFW_KEY_K,				OX_KEY_K },
	{ GLFW_KEY_L,				OX_KEY_L },
	{ GLFW_KEY_M,				OX_KEY_M },
	{ GLFW_KEY_N,				OX_KEY_N },
	{ GLFW_KEY_O,				OX_KEY_O },
	{ GLFW_KEY_P,				OX_KEY_P },
	{ GLFW_KEY_Q,				OX_KEY_Q },
	{ GLFW_KEY_R,				OX_KEY_R },
	{ GLFW_KEY_S,				OX_KEY_S },
	{ GLFW_KEY_T,				OX_KEY_T },
	{ GLFW_KEY_U,				OX_KEY_U },
	{ GLFW_KEY_V,				OX_KEY_V },
	{ GLFW_KEY_W,				OX_KEY_W },
	{ GLFW_KEY_X,				OX_KEY_X },
	{ GLFW_KEY_Y,				OX_KEY_Y },
	{ GLFW_KEY_Z,				OX_KEY_Z },
	{ GLFW_KEY_LEFT_BRACKET,	OX_KEY_LEFT_BRACKET },
	{ GLFW_KEY_BACKSLASH,		OX_KEY_BACKSLASH },
	{ GLFW_KEY_RIGHT_BRACKET,	OX_KEY_RIGHT_BRACKET },
	{ GLFW_KEY_GRAVE_ACCENT,	OX_KEY_GRAVE_ACCENT },
	{ GLFW_KEY_WORLD_1,			OX_KEY_WORLD_1 },
	{ GLFW_KEY_WORLD_2,			OX_KEY_WORLD_2 },
	{ GLFW_KEY_ESCAPE,			OX_KEY_ESCAPE },
	{ GLFW_KEY_ENTER,			OX_KEY_ENTER },
	{ GLFW_KEY_TAB,				OX_KEY_TAB },
	{ GLFW_KEY_BACKSPACE,		OX_KEY_BACKSPACE },
	{ GLFW_KEY_INSERT,			OX_KEY_INSERT },
	{ GLFW_KEY_DELETE,			OX_KEY_DELETE },
	{ GLFW_KEY_RIGHT,			OX_KEY_RIGHT },
	{ GLFW_KEY_LEFT,			OX_KEY_LEFT },
	{ GLFW_KEY_DOWN,			OX_KEY_DOWN },
	{ GLFW_KEY_UP,				OX_KEY_UP },
	{ GLFW_KEY_PAGE_UP,			OX_KEY_PAGE_UP },
	{ GLFW_KEY_PAGE_DOWN,		OX_KEY_PAGE_DOWN },
	{ GLFW_KEY_HOME,			OX_KEY_HOME },
	{ GLFW_KEY_END,				OX_KEY_END },
	{ GLFW_KEY_CAPS_LOCK,		OX_KEY_CAPS_LOCK },
	{ GLFW_KEY_SCROLL_LOCK,		OX_KEY_SCROLL_LOCK },
	{ GLFW_KEY_NUM_LOCK,		OX_KEY_NUM_LOCK },
	{ GLFW_KEY_PRINT_SCREEN,	OX_KEY_PRINT_SCREEN },
	{ GLFW_KEY_PAUSE,			OX_KEY_PAUSE },
	{ GLFW_KEY_F1,				OX_KEY_F1 },
	{ GLFW_KEY_F2,				OX_KEY_F2 },
	{ GLFW_KEY_F3,				OX_KEY_F3 },
	{ GLFW_KEY_F4,				OX_KEY_F4 },
	{ GLFW_KEY_F5,				OX_KEY_F5 },
	{ GLFW_KEY_F6,				OX_KEY_F6 },
	{ GLFW_KEY_F7,				OX_KEY_F7 },
	{ GLFW_KEY_F8,				OX_KEY_F8 },
	{ GLFW_KEY_F9,				OX_KEY_F9 },
	{ GLFW_KEY_F10,				OX_KEY_F10 },
	{ GLFW_KEY_F11,				OX_KEY_F11 },
	{ GLFW_KEY_F12,				OX_KEY_F12 },
	{ GLFW_KEY_F13,				OX_KEY_F13 },
	{ GLFW_KEY_F14,				OX_KEY_F14 },
	{ GLFW_KEY_F15,				OX_KEY_F15 },
	{ GLFW_KEY_F16,				OX_KEY_F16 },
	{ GLFW_KEY_F17,				OX_KEY_F17 },
	{ GLFW_KEY_F18,				OX_KEY_F18 },
	{ GLFW_KEY_F19,				OX_KEY_F19 },
	{ GLFW_KEY_F20,				OX_KEY_F20 },
	{ GLFW_KEY_F21,				OX_KEY_F21 },
	{ GLFW_KEY_F22,				OX_KEY_F22 },
	{ GLFW_KEY_F23,				OX_KEY_F23 },
	{ GLFW_KEY_F24,				OX_KEY_F24 },
	{ GLFW_KEY_F25,				OX_KEY_F25 },
	{ GLFW_KEY_KP_0,			OX_KEY_KP_0 },
	{ GLFW_KEY_KP_1,			OX_KEY_KP_1 },
	{ GLFW_KEY_KP_2,			OX_KEY_KP_2 },
	{ GLFW_KEY_KP_3,			OX_KEY_KP_3 },
	{ GLFW_KEY_KP_4,			OX_KEY_KP_4 },
	{ GLFW_KEY_KP_5,			OX_KEY_KP_5 },
	{ GLFW_KEY_KP_6,			OX_KEY_KP_6 },
	{ GLFW_KEY_KP_7,			OX_KEY_KP_7 },
	{ GLFW_KEY_KP_8,			OX_KEY_KP_8 },
	{ GLFW_KEY_KP_9,			OX_KEY_KP_9 },
	{ GLFW_KEY_KP_DECIMAL,		OX_KEY_KP_DECIMAL },
	{ GLFW_KEY_KP_DIVIDE,		OX_KEY_KP_DIVIDE },
	{ GLFW_KEY_KP_MULTIPLY,		OX_KEY_KP_MULTIPLY },
	{ GLFW_KEY_KP_SUBTRACT,		OX_KEY_KP_SUBTRACT },
	{ GLFW_KEY_KP_ADD,			OX_KEY_KP_ADD },
	{ GLFW_KEY_KP_ENTER,		OX_KEY_KP_ENTER },
	{ GLFW_KEY_KP_EQUAL,		OX_KEY_KP_EQUAL },
	{ GLFW_KEY_LEFT_SHIFT,		OX_KEY_LEFT_SHIFT },
	{ GLFW_KEY_LEFT_CONTROL,	OX_KEY_LEFT_CONTROL },
	{ GLFW_KEY_LEFT_ALT,		OX_KEY_LEFT_ALT },
	{ GLFW_KEY_LEFT_SUPER,		OX_KEY_LEFT_SUPER },
	{ GLFW_KEY_RIGHT_SHIFT,		OX_KEY_RIGHT_SHIFT },
	{ GLFW_KEY_RIGHT_CONTROL,	OX_KEY_RIGHT_CONTROL },
	{ GLFW_KEY_RIGHT_ALT,		OX_KEY_RIGHT_ALT },
	{ GLFW_KEY_RIGHT_SUPER,		OX_KEY_RIGHT_SUPER },
	{ GLFW_KEY_MENU,			OX_KEY_MENU }
};