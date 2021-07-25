#include "../abst/controller.h"
#include "../abst/database.h"
#include "../exp/condition.h"
#include "../exp/reservation.h"
#include "./play.h"

void confirm_reservation(Repository *repo,
    InputController *input,
    ReserveViewController *view) {
  Condition *cond = input_controller_input_condition(input);
  Reservation **reservations = repository_query_reservations(repo, cond);
  view->show_reservations(reservations);
}
