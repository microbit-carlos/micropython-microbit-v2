import timer
# import gc
import micropython

micropython.alloc_emergency_exception_buf(200)


DEFAULT_LOG_FILENAME = 'log.csv'
SEPARATOR = ','


def _is_file(filename):
    try:
        f = open(filename, "r")
        f.close()
        return True
    except:
        return False


def _append(filename, data):
    f_contents = ''
    try:
        with open(filename, "r") as f:
            f_contents += f.read()
    except:
        pass
    with open(filename, "w") as f:
            f.write(f_contents)
            f.write(data)
    # gc.collect()


def create_log(*args, filename=DEFAULT_LOG_FILENAME):
    data = SEPARATOR.join(map(str, args)) + SEPARATOR + "\n"
    if not _is_file(filename):
        print("(logger) creating csv: ", *args)
        with open(filename, "w") as f:
            f.write(data)
    else:
        print('(logger) csv file already exists: ', filename)


def add_row(*args, filename=DEFAULT_LOG_FILENAME,):
    data = SEPARATOR.join(map(str, args)) + SEPARATOR + "\n"
    print("(logger) adding row:", *args)
    _append(filename, data)


def dump(filename=DEFAULT_LOG_FILENAME):
    try:
        with open(filename, "r") as f:
            return f.read()
    except:
        return ''


def set_separator(new_separator):
    if not isinstance(new_separator, str):
        raise Exception('New separator must be a string')
    global SEPARATOR
    SEPARATOR = new_separator


def log_every(ms, filename=DEFAULT_LOG_FILENAME):
    def decorator(function):
        print("(dbg) Log {} every {} ms.".format(function, ms))
        timer.call_at_ms_interval(
            lambda: add_row(*function(), filename=DEFAULT_LOG_FILENAME),
            ms
        )
        def wrapper(*args, **kwargs):
            return function(*args, **kwargs)
        return wrapper
    return decorator
