
#pragma comment(lib, "interception.lib")

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "interception.h"

int main(void) {
  SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
  const InterceptionContext context = interception_create_context();
  interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN);

  InterceptionDevice p1dev;
  InterceptionStroke p1stroke;
  BOOL ready = FALSE;
  printf("liliom:setup\n");
  printf("press card-in key on p1 device to continue\n");

  InterceptionDevice device;
  InterceptionStroke stroke;
  while (interception_receive(context,
    device = interception_wait(context), &stroke, 1) > 0) {
    if (interception_is_keyboard(device)) {
      if (!ready) {
        p1dev = device;
        memcpy(p1stroke, stroke, sizeof stroke);
        ready = TRUE;
        printf("p1 device had been configured :)");
      }
      else {
        const InterceptionKeyStroke keystroke = *(InterceptionKeyStroke *)&stroke;
        if (keystroke.code == 0x01) {
          interception_send(context, p1dev, &p1stroke, 1);
        }
      }
    }
    interception_send(context, device, &stroke, 1);
  }

  interception_destroy_context(context);
  return 0;
}
