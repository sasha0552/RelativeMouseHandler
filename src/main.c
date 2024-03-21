#include <stdio.h>
#include <Windows.h>
#include <WtsApi32.h>

#include "ainput.h"
#include "buffers.h"

static void handle_event(PCHAR Buffer, ULONG BufferSize, ULONG BytesRead, INT32 w, INT32 h) {
  int i = 0;

  if (BytesRead != 34) {
    return;
  }

  UINT32 a = get_uint32(Buffer, i); i += 4; // message length
  UINT32 b = get_uint32(Buffer, i); i += 4; // ???
  UINT16 c = get_uint16(Buffer, i); i += 2; // message id
  UINT64 d = get_uint64(Buffer, i); i += 8; // time
  UINT64 e = get_uint64(Buffer, i); i += 8; // flags
  INT32  f =  get_int32(Buffer, i); i += 4; // x
  INT32  g =  get_int32(Buffer, i); i += 4; // y

  if (a != 26) {
    return;
  }

  if (b != 3) {
    return;
  }

  if (c != MSG_AINPUT_MOUSE) {
    return;
  }

  ainput_mouse_event(d, e, f, g, w, h);
}

// Message: AINPUT_VERSION
char MESSAGE_AINPUT_VERSION[] = {
  // UINT16: MSG_AINPUT_VERSION
  0x01, 0x00,

  // UINT32: AINPUT_VERSION_MAJOR
  0x01, 0x00, 0x00, 0x00,

  // UINT32: AINPUT_VERSION_MINOR
  0x00, 0x00, 0x00, 0x00,
};

int main() {
  //
  POINT cursorPos;
  GetCursorPos(&cursorPos);
  HMONITOR hMonitor = MonitorFromPoint(cursorPos, MONITOR_DEFAULTTONEAREST);
  MONITORINFO monitorInfo;
  monitorInfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(hMonitor, &monitorInfo);
  int w = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
  int h = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
  //

  //
  char buf[512];

  //
  ULONG out = 0;

  //
  void* virtualChannel = WTSVirtualChannelOpenEx(WTS_CURRENT_SESSION, AINPUT_DVC_CHANNEL_NAME, WTS_CHANNEL_OPTION_DYNAMIC);

  //
  if (virtualChannel == NULL) {
    printf("WTSVirtualChannelOpenEx failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    return 1;
  }

  //
  if (!WTSVirtualChannelWrite(virtualChannel, MESSAGE_AINPUT_VERSION, sizeof(MESSAGE_AINPUT_VERSION), &out)) {
    printf("WTSVirtualChannelWrite failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    return 1;
  }

  //
  while (1) {
    //
    if (!WTSVirtualChannelRead(virtualChannel, INFINITE, buf, 512, &out)) {
      printf("WTSVirtualChannelRead failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
      return 1;
    }

    //
    handle_event(buf, 512, out, w, h);
  }
}
