CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(pthread)

SET(LIBPTHREAD_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(${LIBPTHREAD_SRC_DIR})

IF(tscpdk_BINARY_DIR)
    SET(LIBRARY_OUTPUT_PATH ${tscpdk_BINARY_DIR})
ENDIF(tscpdk_BINARY_DIR)

SET(LIBPTHREAD_DLL_INLINED_SRC
    ${LIBPTHREAD_SRC_DIR}/pthread.c
)

SET(LIBPTHREAD_DLL_SRC
    ${LIBPTHREAD_SRC_DIR}/attr.c
    ${LIBPTHREAD_SRC_DIR}/barrier.c
    ${LIBPTHREAD_SRC_DIR}/cancel.c
    ${LIBPTHREAD_SRC_DIR}/cleanup.c
    ${LIBPTHREAD_SRC_DIR}/condvar.c
    ${LIBPTHREAD_SRC_DIR}/create.c
    ${LIBPTHREAD_SRC_DIR}/dll.c
    ${LIBPTHREAD_SRC_DIR}/errno.c
    ${LIBPTHREAD_SRC_DIR}/exit.c
    ${LIBPTHREAD_SRC_DIR}/fork.c
    ${LIBPTHREAD_SRC_DIR}/global.c
    ${LIBPTHREAD_SRC_DIR}/misc.c
    ${LIBPTHREAD_SRC_DIR}/mutex.c
    ${LIBPTHREAD_SRC_DIR}/nonportable.c
    ${LIBPTHREAD_SRC_DIR}/private.c
    ${LIBPTHREAD_SRC_DIR}/rwlock.c
    ${LIBPTHREAD_SRC_DIR}/sched.c
    ${LIBPTHREAD_SRC_DIR}/semaphore.c
    ${LIBPTHREAD_SRC_DIR}/signal.c
    ${LIBPTHREAD_SRC_DIR}/spin.c
    ${LIBPTHREAD_SRC_DIR}/sync.c
    ${LIBPTHREAD_SRC_DIR}/tsd.c
)

SET(LIBPTHREAD_STATIC_SRC
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_getdetachstate.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_setdetachstate.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_getstackaddr.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_setstackaddr.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_getstacksize.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_setstacksize.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_getscope.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_setscope.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_setschedpolicy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_getschedpolicy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_setschedparam.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_getschedparam.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_setinheritsched.c
    ${LIBPTHREAD_SRC_DIR}/pthread_attr_getinheritsched.c
    ${LIBPTHREAD_SRC_DIR}/pthread_barrier_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_barrier_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_barrier_wait.c
    ${LIBPTHREAD_SRC_DIR}/pthread_barrierattr_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_barrierattr_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_barrierattr_setpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_barrierattr_getpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_setcancelstate.c
    ${LIBPTHREAD_SRC_DIR}/pthread_setcanceltype.c
    ${LIBPTHREAD_SRC_DIR}/pthread_testcancel.c
    ${LIBPTHREAD_SRC_DIR}/pthread_cancel.c
    ${LIBPTHREAD_SRC_DIR}/cleanup.c
    ${LIBPTHREAD_SRC_DIR}/pthread_condattr_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_condattr_getpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_condattr_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_condattr_setpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_cond_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_cond_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_cond_signal.c
    ${LIBPTHREAD_SRC_DIR}/pthread_cond_wait.c
    ${LIBPTHREAD_SRC_DIR}/create.c
    ${LIBPTHREAD_SRC_DIR}/dll.c
    ${LIBPTHREAD_SRC_DIR}/autostatic.c
    ${LIBPTHREAD_SRC_DIR}/errno.c
    ${LIBPTHREAD_SRC_DIR}/pthread_exit.c
    ${LIBPTHREAD_SRC_DIR}/fork.c
    ${LIBPTHREAD_SRC_DIR}/global.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutex_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutex_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_getpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_setpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_settype.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_gettype.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_setrobust.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_getrobust.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutex_lock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutex_timedlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutex_unlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutex_trylock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutex_consistent.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_setkind_np.c
    ${LIBPTHREAD_SRC_DIR}/pthread_mutexattr_getkind_np.c
    ${LIBPTHREAD_SRC_DIR}/pthread_getw32threadhandle_np.c
    ${LIBPTHREAD_SRC_DIR}/pthread_getunique_np.c
    ${LIBPTHREAD_SRC_DIR}/pthread_delay_np.c
    ${LIBPTHREAD_SRC_DIR}/pthread_num_processors_np.c
    ${LIBPTHREAD_SRC_DIR}/pthread_win32_attach_detach_np.c
    ${LIBPTHREAD_SRC_DIR}/pthread_equal.c
    ${LIBPTHREAD_SRC_DIR}/pthread_getconcurrency.c
    ${LIBPTHREAD_SRC_DIR}/pthread_once.c
    ${LIBPTHREAD_SRC_DIR}/pthread_self.c
    ${LIBPTHREAD_SRC_DIR}/pthread_setconcurrency.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlock_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlock_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlockattr_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlockattr_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlockattr_getpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlockattr_setpshared.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlock_rdlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlock_wrlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlock_unlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlock_tryrdlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_rwlock_trywrlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_setschedparam.c
    ${LIBPTHREAD_SRC_DIR}/pthread_getschedparam.c
    ${LIBPTHREAD_SRC_DIR}/pthread_timechange_handler_np.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_is_attr.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_processInitialize.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_processTerminate.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_threadStart.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_threadDestroy.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_tkAssocCreate.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_tkAssocDestroy.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_callUserDestroyRoutines.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_timespec.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_throw.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_getprocessors.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_calloc.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_new.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_reuse.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_rwlock_check_need_init.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_cond_check_need_init.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_mutex_check_need_init.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_semwait.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_relmillisecs.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_MCS_lock.c
    ${LIBPTHREAD_SRC_DIR}/sched_get_priority_max.c
    ${LIBPTHREAD_SRC_DIR}/sched_get_priority_min.c
    ${LIBPTHREAD_SRC_DIR}/sched_setscheduler.c
    ${LIBPTHREAD_SRC_DIR}/sched_getscheduler.c
    ${LIBPTHREAD_SRC_DIR}/sched_yield.c
    ${LIBPTHREAD_SRC_DIR}/sem_init.c
    ${LIBPTHREAD_SRC_DIR}/sem_destroy.c
    ${LIBPTHREAD_SRC_DIR}/sem_trywait.c
    ${LIBPTHREAD_SRC_DIR}/sem_timedwait.c
    ${LIBPTHREAD_SRC_DIR}/sem_wait.c
    ${LIBPTHREAD_SRC_DIR}/sem_post.c
    ${LIBPTHREAD_SRC_DIR}/sem_post_multiple.c
    ${LIBPTHREAD_SRC_DIR}/sem_getvalue.c
    ${LIBPTHREAD_SRC_DIR}/sem_open.c
    ${LIBPTHREAD_SRC_DIR}/sem_close.c
    ${LIBPTHREAD_SRC_DIR}/sem_unlink.c
    ${LIBPTHREAD_SRC_DIR}/signal.c
    ${LIBPTHREAD_SRC_DIR}/pthread_kill.c
    ${LIBPTHREAD_SRC_DIR}/ptw32_spinlock_check_need_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_spin_init.c
    ${LIBPTHREAD_SRC_DIR}/pthread_spin_destroy.c
    ${LIBPTHREAD_SRC_DIR}/pthread_spin_lock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_spin_unlock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_spin_trylock.c
    ${LIBPTHREAD_SRC_DIR}/pthread_detach.c
    ${LIBPTHREAD_SRC_DIR}/pthread_join.c
    ${LIBPTHREAD_SRC_DIR}/pthread_key_create.c
    ${LIBPTHREAD_SRC_DIR}/pthread_key_delete.c
    ${LIBPTHREAD_SRC_DIR}/pthread_setspecific.c
    ${LIBPTHREAD_SRC_DIR}/pthread_getspecific.c
    ${LIBPTHREAD_SRC_DIR}/w32_CancelableWait.c
)

SET(LIBPTHREAD_H
    ${LIBPTHREAD_SRC_DIR}/pthread.h
    ${LIBPTHREAD_SRC_DIR}/sched.h
    ${LIBPTHREAD_SRC_DIR}/semaphore.h
)

IF(CMAKE_PTHREAD_STATIC)
    SET(LIBPTHREAD_SRC
        ${LIBPTHREAD_SRC_DIR}/implement.h
        ${LIBPTHREAD_SRC_DIR}/config.h
        ${LIBPTHREAD_SRC_DIR}/need_errno.h
        ${LIBPTHREAD_STATIC_SRC}
    )
    ADD_DEFINITIONS(-DHAVE_PTW32_CONFIG_H -DPTW32_BUILD_INLINED -DPTW32_STATIC_LIB -D__CLEANUP_C)
    ADD_LIBRARY(LIBPTHREAD_A STATIC ${LIBPTHREAD_SRC})
    SET_TARGET_PROPERTIES(LIBPTHREAD_A PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    SET_TARGET_PROPERTIES(LIBPTHREAD_A PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	
	INSTALL(TARGETS LIBPTHREAD_A ARCHIVE DESTINATION lib)
ELSE(CMAKE_PTHREAD_STATIC)
    SET(LIBPTHREAD_SRC
        ${LIBPTHREAD_SRC_DIR}/implement.h
        ${LIBPTHREAD_SRC_DIR}/config.h
        ${LIBPTHREAD_SRC_DIR}/need_errno.h
        ${LIBPTHREAD_DLL_SRC}
    )
    ADD_DEFINITIONS(-DHAVE_PTW32_CONFIG_H -D__CLEANUP_C)
    ADD_LIBRARY(LIBPTHREAD_SO SHARED ${LIBPTHREAD_SRC})
    SET_TARGET_PROPERTIES(LIBPTHREAD_SO PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    SET_TARGET_PROPERTIES(LIBPTHREAD_SO PROPERTIES CLEAN_DIRECT_OUTPUT 1)

	INSTALL(TARGETS LIBPTHREAD_SO LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
ENDIF(CMAKE_PTHREAD_STATIC)



INSTALL(FILES
    ${LIBPTHREAD_H} DESTINATION include/tscpdk/platform/win32/pthread
)




