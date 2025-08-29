#ifndef RENDER_H_
#define RENDER_H_

#include <stdint.h>
#include <stdbool.h>

// Represents a terminal.
typedef struct Device {
  uint16_t rows;
  uint16_t cols;
  const char* term;
  bool posix;
} Device;

// A single command. These are chained in a 'CommandBuffer'
typedef struct Command {
  uint16_t x;
  uint16_t y;
} Command;

// A chain of commands.
typedef struct CommandBuffer {
  uint16_t len;
  Command commands[];
} CommandBuffer;

// Returns a Device (terminal).
Device get_device(void);

// Switches to alternate screen buffer.
void begin_drawing(Device *device);
// Switches to normal screen buffer.
void end_drawing(Device *device);

// Checks the differences between frame buffers.
void buffer_diff(CommandBuffer *front_buffer, CommandBuffer *back_buffer, bool validation_layers);

#endif
