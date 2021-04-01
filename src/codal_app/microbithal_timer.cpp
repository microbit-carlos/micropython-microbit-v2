#include "main.h"
#include "MicroBitDevice.h"

extern "C" {

#include "microbithal.h"

// Last ID I found was MICROBIT_ID_MBED_TICKER -> 43
#define MICROBIT_ID_TIMER_INTERVAL    46


void run_me(Event e, void* callback) {
    uBit.serial.printf("%d: ", e.value);
    ((void (*)(uint16_t))callback)(e.value);
    // uBit.serial.putc(';');
}

void microbit_hal_timer_run_every_ms(uint16_t id, void (*callback)(uint16_t), uint32_t interval_ms) {
    uBit.messageBus.listen(MICROBIT_ID_TIMER_INTERVAL, id, &run_me, (void*)callback, MESSAGE_BUS_LISTENER_QUEUE_IF_BUSY);
    uBit.timer.eventEvery((CODAL_TIMESTAMP)interval_ms, MICROBIT_ID_TIMER_INTERVAL, id);
}

void microbit_hal_timer_release(uint16_t id) {
    uBit.messageBus.ignore(MICROBIT_ID_TIMER_INTERVAL, id, &run_me);
    uBit.timer.cancel(MICROBIT_ID_TIMER_INTERVAL, id);
}

}
