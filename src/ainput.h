#pragma once

#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>

// channel names
#define AINPUT_DVC_CHANNEL_NAME "FreeRDP::Advanced::Input"

// message types
#define MSG_AINPUT_VERSION 0x01
#define MSG_AINPUT_MOUSE   0x02

// MSG_AINPUT_VERSION
#if 1
  #define AINPUT_VERSION_MAJOR 1
  #define AINPUT_VERSION_MINOR 0
#endif

// MSG_AINPUT_MOUSE
#if 1
  #define AINPUT_FLAGS_WHEEL    0x0001
  #define AINPUT_FLAGS_MOVE     0x0004
  #define AINPUT_FLAGS_DOWN     0x0008

  #define AINPUT_FLAGS_REL      0x0010
  #define AINPUT_FLAGS_HAVE_REL 0x0020

  // Pointer flags
  #define AINPUT_FLAGS_BUTTON1  0x1000 // left
  #define AINPUT_FLAGS_BUTTON2  0x2000 // right
  #define AINPUT_FLAGS_BUTTON3  0x4000 // middle

  // Extended pointer flags
  #define AINPUT_XFLAGS_BUTTON1 0x0100
  #define AINPUT_XFLAGS_BUTTON2 0x0200
#endif

// Event Handler: AINPUT_MOUSE
void ainput_mouse_event(UINT64 timestamp, UINT64 flags, INT32 x, INT32 y);
