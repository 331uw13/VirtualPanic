#ifndef VIRTUAL_PANIC__STATUS_H
#define VIRTUAL_PANIC__STATUS_H


#define TRUE 1
#define FALSE 0

#define VENGINE_OK (1<<0)
#define VENGINE_INIT_OK (1<<1)
#define VENGINE_STARTED (1<<2)
#define VENGINE_QUIT (1<<3)
#define VENGINE_CAMERA_ENABLED (1<<4)
#define VENGINE_DID_RENDER_ANYTHING (1<<5)

#define VENGINE_FOV 0
#define VENGINE_ASPECT_RATIO 1
#define VENGINE_ZNEAR 2
#define VENGINE_ZFAR 3
#define VENGINE_TIME_SCALE 4
#define VENGINE_MAX_VALUES 5

#define VPLAYER_DEFAULT_SPEED 8.5f
#define VPLAYER_DEFAULT_HEIGHT 4.0f


// https://www.glfw.org/docs/latest/group__keys.html
#define VKEY_LSHIFT 0x154
#define VKEY_RSHIFT 0x158
#define VKEY_LCTRL 0x155
#define VKEY_RCTRL 0x159
#define VKEY_LALT 0x156
#define VKEY_RALT 0x15A
#define VKEY_ESC 0x100
#define VKEY_ENTER 0x101
#define VKEY_TAB 0x102
#define VKEY_BACKSPACE 0x103
#define VKEY_INSERT 0x104
#define VKEY_DEL 0x105
#define VKEY_R 0x106
#define VKEY_L 0x107
#define VKEY_D 0x108
#define VKEY_U 0x109
#define VKEY_HOME 0x10c
#define VKEY_END 0x10d
#define VKEY_SPACE 0x20


#endif
