#ifndef __ATOMICS_H
#define __ATOMICS_H

#include <pthread.h>
#include <stdbool.h>

#include "debugging.h"

#define LOAD(p)    __atomic_load_n((p), __ATOMIC_SEQ_CST)
#define STORE(p,v) __atomic_store_n((p), (v), __ATOMIC_SEQ_CST)
#define CAS(p,e,d) __atomic_compare_exchange_n((p), (e), (d), false, \
                        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

extern pthread_mutex_t big_lock;

#define STEP_START(sc,td) { \
    pthread_mutex_lock(&big_lock); \
}
#define STEP_END(sc,td) { \
    (sc)->line_number= __LINE__; \
    pthread_mutex_unlock(&big_lock); \
}
#define STEP_HERE(sc,td) { \
    STEP_START(sc,td); \
    (sc)->function= __func__; \
    STEP_END(sc,td) \
}

extern bool do_check_invariants;

#define CHECK_ALWAYS(sc,td) { \
    pthread_mutex_lock(&big_lock); \
    (sc)->line_number= __LINE__; \
    check_invariants((td), __func__, __LINE__); \
    pthread_mutex_unlock(&big_lock); \
}
#define CHECK(sc,td) { \
    if(do_check_invariants) { \
        CHECK_ALWAYS(sc,td); \
    } \
}

#endif /* __ATOMICS_H */
