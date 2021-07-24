#include "./abst/controller.h"
#include "./exp/condition.h"
#include "./exp/instant.h"
#include "./exp/reservation.h"
#include "./play/play.h"
#include "./skin/mock.h"
#include "./skin/stub.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void assert_reseevatins(Reservation **found) {
  assert(strcmp(found[0]->visitor_id, "hoge") == 0);
  assert(strcmp(found[1]->visitor_id, "fuga") == 0);
}

static void assert_not_found(void) __attribute__((noreturn));
static void assert_not_found() {
  assert(false);
}

int main(void) {
  Reservation *reservations[2] = {new_reservation(Done, instant_from_time(200),
                                      instant_from_time(500), "hoge"),
      new_reservation(
          Done, instant_from_time(200), instant_from_time(500), "fuga")};

  Repository *repo = new_stub_repository(reservations, 2);
  Condition *cond = cond_all();
  InputController *con = new_stub_input_controller(
      instant_from_time(300), instant_from_time(400), cond, "", "");
  ReserveViewController view = {assert_reseevatins, assert_not_found};
  confirm_reservation(repo, con, &view);

  drop_stub_input_controller(con);
  drop_condition(cond);
  drop_stub_repository(repo);
  return 0;
}
