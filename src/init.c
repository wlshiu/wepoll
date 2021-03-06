#include <stdbool.h>

#include "api.h"
#include "init.h"
#include "nt.h"
#include "reflock.h"
#include "util.h"
#include "ws.h"

static bool _initialized = false;
static INIT_ONCE _once = INIT_ONCE_STATIC_INIT;

static BOOL CALLBACK _init_once_callback(INIT_ONCE* once,
                                         void* parameter,
                                         void** context) {
  unused_var(once);
  unused_var(parameter);
  unused_var(context);

  /* N.b. that initialization order matters here. */
  if (ws_global_init() < 0 || nt_global_init() < 0 ||
      reflock_global_init() < 0 || api_global_init() < 0)
    return FALSE;

  _initialized = true;
  return TRUE;
}

int init(void) {
  if (!_initialized &&
      !InitOnceExecuteOnce(&_once, _init_once_callback, NULL, NULL))
    return -1; /* LastError and errno aren't touched InitOnceExecuteOnce. */

  return 0;
}
