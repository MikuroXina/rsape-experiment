#include "../abst/clock.h"
#include "../abst/config.h"
#include "../abst/controller.h"
#include "../abst/database.h"
#include "../exp/condition.h"
#include "../exp/reservation.h"
#include "./play.h"

void done_reservation(Repository *repo,
    Database *db,
    InputController *input,
    DoneViewController *view,
    Clock *clock) {
  char const *reservation_id = input_controller_input_reservation_id(input);
  Reservation *r = repository_get_by_id(repo, reservation_id);
  if (r == NULL) {
    view->error_on_done("the reservation not found");
    return;
  }
  if (instant_less_than(clock_get_instant(clock), r->start)) {
    view->error_on_done("too early to be done the reservation");
  } else if (r->state != Active) {
    view->error_on_done("not active reservation cannot be done");
  } else if (!database_done_reservation(db, reservation_id)) {
    view->error_on_done("failed to be done the reservation");
  } else {
    view->success_on_done();
  }
}
