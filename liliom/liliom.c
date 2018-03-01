
#include "liliom.h"
#include <stdio.h>
#include <stdlib.h>

HHOOK kb_hook;

HWND GetWindowHandle(void);
void RegisterRawInputForKeyboards(const HWND my_window);
void InitializeWindow(HINSTANCE hinstance);
LRESULT CALLBACK KeyboardHookEvent(int ncode, WPARAM wparam, LPARAM lparam);

int main(void) {
  printf("-- lilio: manager --");
  const HWND my_window = GetWindowHandle();
  RegisterRawInputForKeyboards(my_window);
  const HINSTANCE hinstance = (HINSTANCE) GetWindowLong(my_window, GWL_HINSTANCE);
  InitializeWindow(hinstance);
  kb_hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardHookEvent, NULL, 0);
  MSG msg;
  PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
  while (msg.message != WM_QUIT) {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return 0;
}

LRESULT CALLBACK KeyboardHookEvent(int ncode, WPARAM wparam, LPARAM lparam) {
  return CallNextHookEx(kb_hook, ncode, wparam, lparam);
}

HWND GetWindowHandle(void) {
  char title[256];
  GetConsoleTitleA(title, 256);
  return FindWindowA(NULL, title);
}

void RegisterRawInputForKeyboards(const HWND my_window) {
  RAWINPUTDEVICE rid[1];
  rid[0].usUsagePage = 1;
  rid[0].usUsage = 6;
  rid[0].dwFlags = RIDEV_INPUTSINK;
  rid[0].hwndTarget = my_window;
  RegisterRawInputDevices(rid, 1, sizeof rid[0]);
}

void InitializeWindow(HINSTANCE hinstance) {
}
