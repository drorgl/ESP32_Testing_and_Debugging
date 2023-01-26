#################################################################################
# .gdbinit-FreeRTOS-helpers
#
# Created on: 29.03.2013
#     Author: Artem Pisarenko
#
# Helper script providing support for FreeRTOS-aware debugging
# Supported architectures: Cortex-M3
# Features:
# - showing tasks (handle and name);
# - switching context to specified task from almost any context (excluding system exceptions);
# - restore current running context.
#################################################################################

define freertos_show_timers
	printf "Current:\r\n"
	_freertos_show_timer_item pxCurrentTimerList
	printf "Overflow:\r\n"
	_freertos_show_timer_item pxOverflowTimerList
end

# Command "freertos_show_threads"
# Shows tasks table: handle(xTaskHandle) and name
define freertos_show_threads
	set $thread_list_size = 0
	set $thread_list_size = uxCurrentNumberOfTasks
	if ($thread_list_size == 0)
		echo FreeRTOS missing or scheduler isn't started\n
	else
		set $current_thread = pxCurrentTCB
		set $tasks_found = 0
		set $idx = 0

		printf "->Running:\n"
		_freertos_show_thread_header
		
		set $task_list = pxReadyTasksLists
		set $task_list_size = sizeof(pxReadyTasksLists)/sizeof(pxReadyTasksLists[0])
		while ($idx < $task_list_size)
			_freertos_show_thread_item $task_list[$idx]
			set $idx = $idx + 1
		end

		_freertos_show_thread_item xDelayedTaskList1
		_freertos_show_thread_item xDelayedTaskList2
		_freertos_show_thread_item xPendingReadyList
		
		set $VAL_dbgFreeRTOSConfig_suspend = dbgFreeRTOSConfig_vTaskSuspend
		if ($VAL_dbgFreeRTOSConfig_suspend != 0)
			printf "->Suspended:\n"
			_freertos_show_thread_item xSuspendedTaskList
		end

		set $VAL_dbgFreeRTOSConfig_delete = dbgFreeRTOSConfig_vTaskDelete
		if ($VAL_dbgFreeRTOSConfig_delete != 0)
			printf "->Waiting Termination:\n"
			_freertos_show_thread_item xTasksWaitingTermination
		end
	end
end

define freertos_show_queues
	p xQueueRegistry
end

# Command "freertos_switch_to_task"
# Switches debugging context to specified task, argument - task handle
define freertos_switch_to_task
	set var dbgPendingTaskHandle = $arg0
	set $current_IPSR_val = $xpsr & 0xFF
	if (($current_IPSR_val >= 1) && ($current_IPSR_val <= 15))
		echo Switching from system exception context isn't supported
	else
		set $VAL_dbgPendSVHookState = dbgPendSVHookState
		if ($VAL_dbgPendSVHookState == 0)
			set $last_PRIMASK_val = $PRIMASK
			set $last_SCB_ICSR_val = *((volatile unsigned long *)0xE000ED04)
			set $last_SYSPRI2_val = *((volatile unsigned long *)0xE000ED20)
			set $last_SCB_CCR_val = *((volatile unsigned long *)0xE000ED14)
			set $running_IPSR_val = $current_IPSR_val
			set $PRIMASK = 0
			# *(portNVIC_SYSPRI2) &= ~(255 << 16) // temporary increase PendSV priority to highest
				set {unsigned int}0xe000ed20 = ($last_SYSPRI2_val & (~(255 << 16)))
			# set SCB->CCR NONBASETHRDENA bit (allows processor enter thread mode from at any execution priority level)
			set {unsigned int}0xE000ED14 = (1) | $last_SCB_CCR_val
			set var dbgPendSVHookState = 1
		end
		# *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET
			set {unsigned int}0xe000ed04 = 0x10000000
		continue
		# here we stuck at "bkpt" instruction just before "bx lr" (in helper's xPortPendSVHandler)
		# force returning to thread mode with process stack
		set $lr = 0xFFFFFFFD
		stepi
		stepi
		# here we get rewound to task
	end
end

# Command "freertos_restore_running_context"
# Restores context of running task
define freertos_restore_running_context
	set $VAL_dbgPendSVHookState = dbgPendSVHookState
	if ($VAL_dbgPendSVHookState == 0)
		echo Current task is RUNNING, ignoring command...
	else
		set var dbgPendingTaskHandle = (void *)pxCurrentTCB
		# *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET
			set {unsigned int}0xe000ed04 = 0x10000000
		continue
		# here we stuck at "bkpt" instruction just before "bx lr" (in helper's xPortPendSVHandler)
		# check what execution mode was in context we started to switch from
		if ($running_IPSR_val == 0)
			# force returning to thread mode with process stack
			set $lr = 0xFFFFFFFD
		else
			# force returning to handler mode
			set $lr = 0xFFFFFFF1
		end
		stepi
		stepi
		# here we get rewound to running task at place we started switching
		# restore processor state
		set $PRIMASK = $last_PRIMASK_val
		set {unsigned int}0xe000ed20 = $last_SYSPRI2_val
		set {unsigned int}0xE000ED14 = $last_SCB_CCR_val
		if ($last_SCB_ICSR_val & (1 << 28))
			set {unsigned int}0xe000ed04 = 0x10000000
		end
		set var dbgPendSVHookState = 0
	end
end

# Command "show_broken_backtrace"
# Workaround of issue when context is being stuck in the middle of function epilogue (i.e., in vTaskDelay())
# This solution is applied to following situation only:
### ... function body end
### xxxxxxxx+0: add.w r7, r7, #16
### xxxxxxxx+4: mov sp, r7        ; <- debug current instruction pointer
### xxxxxxxx+6: pop {r7, pc}
### }
# (Otherwise it will crash !)
define show_broken_backtrace
	# cancel effect of xxxxxxxx+4 instruction twice (because we will step it to update eclipse views)
	set $r7 = $r7 - 16 - 16
	set $pc = $pc - 4
	stepi
end


#######################
# Internal functions
define _freertos_show_thread_header
	printf "thread\t\tname\tpriority\tcore\truntime\t"
end

define _freertos_show_timer_item
	set $list_timer_count = $arg0.uxNumberOfItems
	set $prev_list_elem_ptr = -1
	set $list_elem_ptr = $arg0.xListEnd.pxPrevious
	while ($list_timer_count > 0)
		set $timerid = $list_elem_ptr->pvOwner

		set $timer_name_str = (*((Timer_t *)$timerid)).pcTimerName
		print "0x%x\t%s\t",$timerid, $timer_name_str

		set $list_timer_count = $list_timer_count - 1
	end
end

define _freertos_show_thread_item
	set $list_thread_count = $arg0.uxNumberOfItems
	set $prev_list_elem_ptr = -1
	set $list_elem_ptr = $arg0.xListEnd.pxPrevious
	while (($list_thread_count > 0) && ($list_elem_ptr != 0) && ($list_elem_ptr != $prev_list_elem_ptr) && ($tasks_found < $thread_list_size))
		set $threadid = $list_elem_ptr->pvOwner
		set $thread_name_str = (*((tskTCB *)$threadid)).pcTaskName
		set $thread_priority = (*((tskTCB *)$threadid)).uxPriority
		set $thread_core = (*((tskTCB *)$threadid)).xCoreID
		set $thread_runtime_stats = -1
		set $dbgFreeRTOSConfig_GENERATE_RUN_TIME_STATS = dbgFreeRTOSConfig_GENERATE_RUN_TIME_STATS
		if (dbgFreeRTOSConfig_GENERATE_RUN_TIME_STATS != 0)
			#display runtime stats
			#set $thread_runtime_stats = (*((tskTCB *)$threadid)).ulRunTimeCounter
		end

		set $tasks_found = $tasks_found + 1
		set $list_thread_count = $list_thread_count - 1
		set $prev_list_elem_ptr = $list_elem_ptr
		set $list_elem_ptr = $prev_list_elem_ptr->pxPrevious
		if ($threadid == $current_thread)
			printf "0x%x\t%s\t%d\t%d\t%d\t\t<---RUNNING\n", $threadid, $thread_name_str, $thread_priority,$thread_core,$thread_runtime_stats
		else
			printf "0x%x\t%s\t%d\t%d\t%d\n", $threadid, $thread_name_str, $thread_priority,$thread_core,$thread_runtime_stats
		end
	end
end
