from microbit import *
import timer_
import data_logging as dlog

# Creates a new "log" file with the given "headers"
dlog.create_log('time (ms)', 'temperature (°C)')


# A decorator might be a bit "magical" as we have never used them before in micro:bit
@timer_.run_every(ms=1000)
def log_temp_from_timer_mod_decorator():
    """Records the temperature with a time stamp every second."""
    dlog.add_row(running_time(), temperature())


@dlog.log_every(3000)
def log_temp_from_dlog_mod_decorator():
    """Records the temperature (+100) with a time stamp every 2s."""
    return running_time(), temperature() + 100


# Schedule via function call by creating a lambda (an "inline" function)
timer_.call_every(lambda: dlog.add_row(running_time(), temperature() + 1000), 5000)


hear_small = False
while True:
    display.show(Image.HEART_SMALL if hear_small else Image.HEART)
    hear_small = not hear_small
    if button_a.is_pressed():
        # On Button A send all the data via serial
        print(dlog.dump())
        sleep(1000)
    if button_b.is_pressed():
        # On Button B add another entry to the log
        # this shows the function with the decorator can still be called
        log_temp_from_timer_mod_decorator()
        sleep(300)

