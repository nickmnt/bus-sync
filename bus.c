// Fill in empty functions and defintions

#include "pintos_headers.h"

struct station {
    struct lock lck;
    struct condition condBoard;
    struct condition condPas;
    int customers;
};

int min(int a, int b) {
    return (a > b) ? b : a;
}

void station_setup(struct station *station) {
    lock_init(&station->lck);
    cond_init(&station->condPas);
    cond_init(&station->condBoard);
    station -> customers = 0;
}

void bus_load_passengers(struct station *station, int count) {
    lock_acquire(&station->lck);
    int remaining = count;
    while(remaining > 0 && station->customers != 0) {
        station->customers-=1;
        remaining -= 1;
        cond_signal(&station->condPas, &station->lck);
        cond_wait(&station->condBoard, &station->lck);
    }
    lock_release(&station->lck);
}

void passenger_waitfor_bus(struct station *station) {
    lock_acquire(&station->lck);
    station->customers += 1;
    cond_wait(&station->condPas, &station->lck);
    lock_release(&station->lck);
}

void passenger_on_board(struct station *station) {
    lock_acquire(&station->lck);
    cond_signal(&station->condBoard, &station->lck);
    lock_release(&station->lck);
}
