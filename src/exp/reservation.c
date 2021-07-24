#include "./reservation.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static char *gen_uuid_v4() {
  srand((unsigned)time(NULL));
  unsigned char rands[16] = {0};
  for (int i = 0; i < 16; ++i) {
    rands[i] = (unsigned char)rand();
  }
  rands[6] &= 0x0f;
  rands[6] |= 0x40;
  rands[8] &= 0x3f;
  rands[8] |= 0x80;

  char *str = malloc(37 * sizeof(char));
  sprintf(str,
      "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
      rands[0], rands[1], rands[2], rands[3], rands[4], rands[5], rands[6],
      rands[7], rands[8], rands[9], rands[10], rands[11], rands[12], rands[13],
      rands[14], rands[15]);
  return str;
}

Reservation *new_reservation(ReservationState state,
    Instant start,
    Instant end,
    char const *visitor_id) {
  Reservation *r = malloc(sizeof(Reservation));
  r->id = gen_uuid_v4();
  r->state = state;
  r->start = start;
  r->end = end;
  r->visitor_id = visitor_id;
  return r;
}

void drop_reservation(Reservation *r) {
  free(r->id);
  free(r);
}
