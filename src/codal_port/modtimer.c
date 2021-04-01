#include "py/runtime.h"
#include "py/mphal.h"
// #include "py/gc.h"
#include "modmicrobit.h"
#include "microbithal.h"


#define MAX_TIMER_FUNCS     10

STATIC mp_obj_t _timer_callback[MAX_TIMER_FUNCS] = { mp_const_none };
STATIC uint16_t _timer_callback_count = 0;

void timer_deactivate(uint16_t id) {
    _timer_callback[id] = mp_const_none;
    microbit_hal_timer_release(id);
}

void timer_callback(uint16_t id) {
    if (id >= MAX_TIMER_FUNCS) {
        mp_raise_ValueError(MP_ERROR_TEXT("timer callback ID invalid"));
    }
    if (_timer_callback[id] != mp_const_none) {
        // gc_lock();
        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0) {
            mp_call_function_0(_timer_callback[id]);
            nlr_pop();
        } else {
            // Uncaught exception; disable the callback so it doesn't run again.
            timer_deactivate(id);
            mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
        }
        // gc_unlock();
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("BAD!!!"));
    }
}

mp_obj_t timer_call_at_ms_interval(mp_obj_t callback_obj, mp_obj_t ms_obj) {
    mp_int_t interval_ms = mp_obj_get_int(ms_obj);

    if (_timer_callback_count >= MAX_TIMER_FUNCS) {
        mp_raise_ValueError(MP_ERROR_TEXT("cannot register more timer callbacks"));
    }

    if (mp_obj_is_callable(callback_obj)) {
        _timer_callback_count++;
        _timer_callback[_timer_callback_count] = callback_obj;
        microbit_hal_timer_run_every_ms(
            _timer_callback_count, &timer_callback, (uint32_t)interval_ms
        );
    } else if (callback_obj == mp_const_none) {
        // TODO: Need to define a way to unregister timer events
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("callback must be a function or None"));
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(timer_call_at_ms_interval_obj, timer_call_at_ms_interval);


STATIC const mp_map_elem_t timer_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_timer) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_call_at_ms_interval), (mp_obj_t)&timer_call_at_ms_interval_obj },
};
STATIC MP_DEFINE_CONST_DICT(timer_module_globals, timer_module_globals_table);

const mp_obj_module_t timer_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&timer_module_globals,
};
