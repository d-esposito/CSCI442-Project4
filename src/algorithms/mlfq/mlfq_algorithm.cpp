#include "algorithms/mlfq/mlfq_algorithm.hpp"

#include <cassert>
#include <stdexcept>
#include <math.h>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

MLFQScheduler::MLFQScheduler(int slice) {
    if (slice <= 0) {
        time_slice = 3;
    }
    else {
        time_slice = slice;
    }
}

std::shared_ptr<SchedulingDecision> MLFQScheduler::get_next_thread() {
        std::shared_ptr<SchedulingDecision> decision = std::make_shared<SchedulingDecision>();

        if (size() == 0) {
            decision->explanation = "No threads available for scheduling.";
            return decision;
        }

        int queue_num ;
        for (queue_num = 0; ready_queue[queue_num].size() == 0 && queue_num < 10; queue_num++);

        time_slice = pow(2,queue_num);
        decision->time_slice = time_slice;

        std::shared_ptr<Thread> next = ready_queue[queue_num].top();
        
        std::string priority;
        switch (next->priority) {
            case SYSTEM:
                priority = "SYSTEM";
                break;
            case INTERACTIVE:
                priority = "INTERACTIVE";
                break;
            case NORMAL:
                priority = "NORMAL";
                break;
            default:
                priority = "BATCH";
        }

        decision->explanation = "Selected from queue " + std::to_string(queue_num) + " (priority = " + priority + ", runtime = " + std::to_string(next->time_in_queue) + "). Will run for at most " + std::to_string(time_slice) + " ticks.";
        next->time_in_queue += (next->bursts.front())->length;
        decision->thread = next;
        ready_queue[queue_num].pop();

        return decision;
}

void MLFQScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        int slice = pow(2, thread->previous_queue);
        if (thread->time_in_queue >= slice && thread->previous_queue < 10) {
            (thread->previous_queue)++;
            thread->time_in_queue = 0;
        }
        ready_queue[thread->previous_queue].push(thread->priority == SYSTEM ? 0 : thread->priority == INTERACTIVE ? 1 : thread->priority == NORMAL ? 2 : 3, thread);
}

size_t MLFQScheduler::size() const {
        int size = 0;
        for (int i = 0; i < 10; i++)
            size += ready_queue[i].size();
        return size;
}
