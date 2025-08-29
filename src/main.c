#include <stdlib.h>

#include "render.h"

int main(void) {
  Device device = get_device();

  CommandBuffer *front_buffer = malloc(sizeof(CommandBuffer) + sizeof(Command) * 1);
  front_buffer->len = 1;
  front_buffer->commands[0].x = 1;
  front_buffer->commands[0].y = 1;

  CommandBuffer *back_buffer = malloc(sizeof(CommandBuffer) + sizeof(Command) * 1);
  back_buffer->len = 1;
  back_buffer->commands[0].x = 2;
  back_buffer->commands[0].y = 1;

  begin_drawing(&device);

  buffer_diff(front_buffer, back_buffer, true);

  end_drawing(&device);

  free(front_buffer);
  free(back_buffer);

  return 0;
}
