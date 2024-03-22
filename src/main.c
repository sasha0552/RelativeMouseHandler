#include <stdio.h>

#include <Windows.h>
#include <WinUser.h>
#include <WtsApi32.h>

#include "ainput.h"
#include "buffers.h"

///// ///// ///// ///// /////

// Message: AINPUT_VERSION
static char MESSAGE_AINPUT_VERSION[] = {
  // UINT16: MSG_AINPUT_VERSION
  0x01, 0x00,

  // UINT32: AINPUT_VERSION_MAJOR
  0x01, 0x00, 0x00, 0x00,

  // UINT32: AINPUT_VERSION_MINOR
  0x00, 0x00, 0x00, 0x00,
};

///// ///// ///// ///// /////

static void handle_event(PCHAR Buffer, ULONG BufferSize, ULONG BytesRead) {
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

  ainput_mouse_event(d, e, f, g);
}

static BOOL IsCaptured() {
  GUITHREADINFO guiThreadInfo;

  /////

  guiThreadInfo.cbSize = sizeof(GUITHREADINFO);

  /////

  if (!GetGUIThreadInfo(NULL, &guiThreadInfo)) {
    printf("GetGUIThreadInfo failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    ExitProcess(1);
  }

  /////

  return guiThreadInfo.hwndCapture != NULL;
}

///// ///// ///// ///// /////

static HANDLE virtualChannel = NULL;

static DWORD WINAPI WTSVirtualChannelReadLoop(LPVOID lpParam) {
  char buf[512];

  /////

  ULONG out = 0;

  /////

  while (TRUE) {
    if (virtualChannel != NULL) {
      if (!WTSVirtualChannelRead(virtualChannel, 0, buf, sizeof(buf), &out)) {
        printf("WTSVirtualChannelRead failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
        ExitProcess(1);
      }

      /////

      handle_event(buf, sizeof(buf), out);
    }
  }
}

int main() {
  HANDLE readLoop = CreateThread(NULL, 0, WTSVirtualChannelReadLoop, NULL, 0, NULL);

  /////

  ULONG out = 0;

  /////

  if (readLoop == NULL) {
    printf("CreateThread failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    ExitProcess(1);
  }

  /////

  while (TRUE) {
    const BOOL mouseCaptured = IsCaptured();

    /////

    printf("mouseCaptured: %d\n", mouseCaptured);

    /////

    if (mouseCaptured) {
      if (virtualChannel == NULL) {
        HANDLE newVirtualChannel = WTSVirtualChannelOpenEx(WTS_CURRENT_SESSION, AINPUT_DVC_CHANNEL_NAME, WTS_CHANNEL_OPTION_DYNAMIC);

        /////

        if (newVirtualChannel == NULL) {
          printf("WTSVirtualChannelOpenEx failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
          ExitProcess(1);
        }

        /////

        if (!WTSVirtualChannelWrite(newVirtualChannel, MESSAGE_AINPUT_VERSION, sizeof(MESSAGE_AINPUT_VERSION), &out)) {
          printf("WTSVirtualChannelWrite failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
          ExitProcess(1);
        }

        /////

        virtualChannel = newVirtualChannel;
      }
    } else {
      if (virtualChannel != NULL) {
        HANDLE oldVirtualChannel = virtualChannel;

        /////

        virtualChannel = NULL;

        /////

        if (!WTSVirtualChannelClose(oldVirtualChannel)) {
          printf("WTSVirtualChannelClose failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
          ExitProcess(1);
        }
      }
    }

    /////

    Sleep(50);
  }

  /////

  if (!TerminateThread(readLoop, 0)) {
    printf("TerminateThread failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    ExitProcess(1);
  }
}
