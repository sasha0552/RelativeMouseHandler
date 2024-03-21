#include "ainput.h"

void ainput_mouse_event(UINT64 timestamp, UINT64 flags, INT32 x, INT32 y, INT32 w, INT32 h) {
  printf("W M D R H 1 2 3 4 5 | x y\n%d %d %d %d %d %d %d %d %d %d | %d %d\n\n",
    (flags & AINPUT_FLAGS_WHEEL   ) != 0,
    (flags & AINPUT_FLAGS_MOVE    ) != 0,
    (flags & AINPUT_FLAGS_DOWN    ) != 0,
    (flags & AINPUT_FLAGS_REL     ) != 0,
    (flags & AINPUT_FLAGS_HAVE_REL) != 0,
    (flags & AINPUT_FLAGS_BUTTON1 ) != 0,
    (flags & AINPUT_FLAGS_BUTTON2 ) != 0,
    (flags & AINPUT_FLAGS_BUTTON3 ) != 0,
    (flags & AINPUT_XFLAGS_BUTTON1) != 0,
    (flags & AINPUT_XFLAGS_BUTTON2) != 0,
    x,
    y
  );

  INPUT inputs[1];
  ZeroMemory(inputs, sizeof(inputs));

  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.time = timestamp;
  
  if (flags & AINPUT_FLAGS_WHEEL) {
    if (x != 0) {
      inputs[0].mi.dwFlags = MOUSEEVENTF_HWHEEL;
      inputs[0].mi.mouseData = x;
    } else {
      inputs[0].mi.dwFlags = MOUSEEVENTF_WHEEL;
      inputs[0].mi.mouseData = y;
    }
  } else if (flags & AINPUT_FLAGS_MOVE) {
    if (flags & AINPUT_FLAGS_REL) {
      inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;
      inputs[0].mi.dx = x;
      inputs[0].mi.dy = y;
    } else {
      inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
      inputs[0].mi.dx = MulDiv(x, 65535, w);
      inputs[0].mi.dy = MulDiv(y, 65535, h);
    }
  } else if (flags & AINPUT_FLAGS_BUTTON1) {
    inputs[0].mi.dwFlags = flags & AINPUT_FLAGS_DOWN ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
  } else if (flags & AINPUT_FLAGS_BUTTON2) {
    inputs[0].mi.dwFlags = flags & AINPUT_FLAGS_DOWN ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
  } else if (flags & AINPUT_FLAGS_BUTTON3) {
    inputs[0].mi.dwFlags = flags & AINPUT_FLAGS_DOWN ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
  } else if (flags & AINPUT_XFLAGS_BUTTON1) {
    inputs[0].mi.dwFlags = flags & AINPUT_FLAGS_DOWN ? MOUSEEVENTF_XDOWN : MOUSEEVENTF_XUP;
    inputs[0].mi.mouseData = XBUTTON1;
  } else if (flags & AINPUT_XFLAGS_BUTTON2) {
    inputs[0].mi.dwFlags = flags & AINPUT_FLAGS_DOWN ? MOUSEEVENTF_XDOWN : MOUSEEVENTF_XUP;
    inputs[0].mi.mouseData = XBUTTON2;
  }

  UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  if (uSent != ARRAYSIZE(inputs)) {
    printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
  }
}
