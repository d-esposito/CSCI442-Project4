#include "algorithms/cfs/cfs_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

CFSScheduler::CFSScheduler(int slice) {
    if (slice <= 0) {
        min_granularity = 3;
    }
    else {
        min_granularity = slice;
    }
}

std::shared_ptr<SchedulingDecision> CFSScheduler::get_next_thread() {
        std::shared_ptr<SchedulingDecision> decision = std::make_shared<SchedulingDecision>();

        if (ready_queue.size() == 0) {
                decision->explanation = "No threads available for scheduling.";
                return decision;
        }

        std::shared_ptr<Thread> next = ready_queue.top();
        double sched_latency = 48.0 / ((double) ready_queue.size());
        double q = next->cfs_weight / weight_sum * sched_latency;
        time_slice = q < min_granularity ? min_granularity : q;

        decision->time_slice = time_slice;
        decision->thread = next;
        decision->explanation = "Selected from " + std::to_string(ready_queue.size()) + " threads (vruntime = " + std::to_string((int) next->vruntime) + "). Will run for at most " + std::to_string(time_slice) + " ticks.";
        
        ready_queue.pop();
        weight_sum -= next->cfs_weight;

        return decision;
}

void CFSScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        if (thread->cfs_weight == 0) {
                switch (thread->priority) {
                        case SYSTEM:
                                thread->cfs_weight = 88761.0;
                                break;
                        case INTERACTIVE:
                                thread->cfs_weight = 29154.0;
                                break;
                        case NORMAL:
                                thread->cfs_weight = 1024.0;
                                break;
                        default:
                                thread->cfs_weight = 15.0;
                }
        }

        thread->vruntime = 1024.0 / (thread->cfs_weight) * (thread->service_time);

        weight_sum += thread->cfs_weight;
        ready_queue.push(thread->vruntime, thread);
}

size_t CFSScheduler::size() const {
        return ready_queue.size();
}
