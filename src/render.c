#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/ioctl.h>
#include <unistd.h>

#include "render.h"

#ifdef NDEBUG
const bool debug = false;
#else
const bool debug = true;
#endif

Device get_device(void) {
  Device device;

  const char *env_name = "TERM";
  const char *term = getenv(env_name);

  bool posix_compliant = true;

  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
    posix_compliant = false;

    device.rows = 24;
    device.cols = 80;
  } else {
    device.rows = ws.ws_row;
    device.cols = ws.ws_col;
  }

  device.term = term;
  device.posix = posix_compliant;

  if (debug == true) {
    printf("[BUFFER][LOG] found device    : \'%s\'\n", device.term);
    printf("              POSIX compliant : %s\n", device.posix ? "true" : "false");
    printf("              columns         : %d\n", device.cols);
    printf("              rows            : %d\n", device.rows);
    printf("\n");
  }

  if (!posix_compliant) {
    printf("[BUFFER][WARNING] device \'%s\' is not POSIX compliant, defaulting to 24:80 size\n", term);
  }

  return device;
}

void begin_drawing(Device *device) {
  if (debug == true) {
    printf("[BUFFER][LOG] switching to alternate screen buffer to begin drawing for terminal \'%s\'\n", device->term);
  }
  printf("\x1B[?1049h");
}

void end_drawing(Device *device) {
  printf("\x1B[?1049l");
  if (debug == true) {
    printf("[BUFFER][LOG] switching to normal frame buffer to end drawing for terminal \'%s\'\n", device->term);
  }
}

bool commands_equal(Command *command1, Command *command2) {
  if (command1->x != command2->x) {
    return false;
  }

  if (command1->y != command2->y) {
    return false;
  }

  return true;
}

void buffer_diff(CommandBuffer *front_buffer, CommandBuffer *back_buffer, bool validation_layers) {
  if (front_buffer->len != back_buffer->len) {
    fprintf(stderr, "[BUFFER][ERROR] front_buffer (%p) and back_buffer (%p) length mismatch: %u vs %u\n",
	    (void*)front_buffer, (void*)back_buffer,
	    front_buffer->len, back_buffer->len);
    return;
  }

  for (int i = 0; i < front_buffer->len; i++) {
    Command *f_cmd = &front_buffer->commands[i];
    Command *b_cmd = &back_buffer->commands[i];

    if (!commands_equal(f_cmd, b_cmd)) {
      if (debug == true) {
	printf("[BUFFER][LOG] Command %d differs: front_buffer=%p (x=%u, y=%u), back_buffer=%p (x=%u, y=%u)\n",
		i,
		(void*)f_cmd, f_cmd->x, f_cmd->y,
		(void*)b_cmd, b_cmd->x, b_cmd->y
	);
      }
    }
  }
}
