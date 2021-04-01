from timer import *

def run_every(ms):
    def decorator(function):
        print('(dbg) Run {} every {} ms.'.format(function, ms))
        call_at_ms_interval(function, ms)
        def wrapper(*args, **kwargs):
            # print('(db) b4 func:', args, kwargs)
            result = function(*args, **kwargs)
            return result
        return wrapper
    return decorator


def call_every(callback, ms):
    print('(dbg) Run {} every {} ms.'.format(callback, ms))
    call_at_ms_interval(callback, ms)
