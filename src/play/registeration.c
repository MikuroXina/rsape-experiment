#include "../abst/config.h"
#include "../abst/controller.h"
#include "../abst/database.h"
#include "../exp/condition.h"
#include "../exp/reservation.h"
#include "./play.h"

void register_reservation(Repository *repo,
    Database *db,
    InputController *input,
    RegisterViewController *view,
    Config *config) {
  Instant from = input_controller_input_instant_from(input);
  Instant to = input_controller_input_instant_to(input);
  Condition *cond = cond_and(cond_later_than(from), cond_earlier_than(to));

  Reservation **list = repository_query_reservations(repo, cond);
  int list_len = 0;
  for (Reservation **it = list; *it != NULL; ++it) {
    ++list_len;
  }

  if (config_reservation_limit(config) < list_len) {
    view->error_on_register("reached reservation limit in the duration");
  } else {
    char const *visitor_id = input_controller_input_visitor_id(input);
    Reservation *r = new_reservation(Active, from, to, visitor_id);
    if (database_register_reservation(db, r)) {
      view->success_on_register();
    } else {
      view->error_on_register("failed to register th reservation");
    }
  }

  drop_condition(cond);
}
