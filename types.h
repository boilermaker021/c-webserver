#ifndef TYPES_H
#define TYPES_H

enum message_type {
  MESSAGE
};

typedef struct message {
  char buf[512];
} message_t;

#endif