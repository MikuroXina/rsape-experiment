#include "../abst/clock.h"
#include "../abst/config.h"
#include "../abst/controller.h"
#include "../abst/database.h"
#include "../exp/condition.h"
#include "../exp/reservation.h"
#include "./play.h"

void cancel_reservation(Repository *repo,
    Database *db,
    InputController *input,
    CancelViewController *view,
    Clock *clock,
    Config *config) {
  char const *reservation_id = input_controller_input_reservation_id(input);
  Reservation *r = repository_get_by_id(repo, reservation_id);
  if (r == NULL) {
    view->error_on_cancel("the reservation not found");
    return;
  }
  if (instant_less_than(clock_get_instant(clock),
          instant_add(r->start, config_cancel_limit(config)))) {
    view->error_on_cancel("too late to cancel the reservation");
  } else if (r->state != Active) {
    view->error_on_cancel("not active reservation cannot cancel");
  } else if (!database_stale_reservation(db, reservation_id)) {
    view->error_on_cancel("failed to cancel the reservation");
  } else {
    view->success_on_cancel();
  }
}
