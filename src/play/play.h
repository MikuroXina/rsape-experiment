#pragma once

typedef struct CancelViewController_ CancelViewController;
typedef struct Clock_ Clock;
typedef struct Config_ Config;
typedef struct Database_ Database;
typedef struct DoneViewController_ DoneViewController;
typedef struct InputController_ InputController;
typedef struct RegisterViewController_ RegisterViewController;
typedef struct Repository_ Repository;
typedef struct ReserveViewController_ ReserveViewController;

void confirm_reservation(Repository *repo,
    InputController *con,
    ReserveViewController *view);
void register_reservation(Repository *repo,
    Database *db,
    InputController *con,
    RegisterViewController *view,
    Config *config);
void cancel_reservation(Repository *repo,
    Database *db,
    InputController *con,
    CancelViewController *view,
    Clock *clock,
    Config *config);
void done_reservation(Repository *repo,
    Database *db,
    InputController *con,
    DoneViewController *view,
    Clock *clock);
