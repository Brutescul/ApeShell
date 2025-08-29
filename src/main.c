#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Command {
  uint16_t x;
  uint16_t y;
} Command;

typedef struct CommandBuffer {
  uint16_t len;
  Command commands[];
} CommandBuffer;

void begin_drawing(void) {
  const char *env_name = "TERM";
  const char *term = getenv(env_name);

  printf("[BUFFER][LOG] switching to alternate screen buffer to begin drawing for terminal \'%s\'\n", term);
  printf("\x1B[?1049h");
}

void end_drawing(void) {
  const char *env_name = "TERM";
  const char *term = getenv(env_name);

  printf("\x1B[?1049l");
  printf("[BUFFER][LOG] switching to normal frame buffer to end drawing for terminal \'%s\'\n", term);
}

bool commands_equal(Command *command1, Command *command2) {
  if (&command1->x != &command2->x) {
    return false;
  }

  if (&command1->y != &command2->y) {
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
      if (validation_layers == true) {
	printf("[BUFFER][LOG] Command %d differs: front_buffer=%p (x=%u, y=%u), back_buffer=%p (x=%u, y=%u)\n",
		i,
		(void*)f_cmd, f_cmd->x, f_cmd->y,
		(void*)b_cmd, b_cmd->x, b_cmd->y
	);
      }
    }
  }
}

int main(void) {
    CommandBuffer *front_buffer = malloc(sizeof(CommandBuffer) + sizeof(Command) * 1);
    front_buffer->len = 1;
    front_buffer->commands[0].x = 1;
    front_buffer->commands[0].y = 1;

    CommandBuffer *back_buffer = malloc(sizeof(CommandBuffer) + sizeof(Command) * 1);
    back_buffer->len = 1;
    back_buffer->commands[0].x = 2;
    back_buffer->commands[0].y = 1;

    begin_drawing();

    buffer_diff(front_buffer, back_buffer, true);

    end_drawing();

    free(front_buffer);
    free(back_buffer);

    return 0;
}
