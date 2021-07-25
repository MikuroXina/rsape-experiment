#pragma once

#include "../exp/instant.h"

typedef struct Condition_ Condition;
typedef struct InputController_ InputController;

InputController *new_console_input_controller(void);
void drop_console_input_controller(InputController *);

typedef struct ReserveViewController_ ReserveViewController;
typedef struct RegisterViewController_ RegisterViewController;
typedef struct CancelViewController_ CancelViewController;
typedef struct DoneViewController_ DoneViewController;

typedef struct ConsoleViewController_ {
  ReserveViewController *reserve_view;
  RegisterViewController *register_view;
  CancelViewController *cancel_view;
  DoneViewController *done_view;
} ConsoleViewController;

ConsoleViewController *new_console_view_controller(void);
void drop_console_view_controller(ConsoleViewController *);
