#include "algorithms/rr/rr_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

RRScheduler::RRScheduler(int slice) {
    if (slice <= 0) {
        time_slice = 3;
    }
    else {
        time_slice = slice;
    }
}

std::shared_ptr<SchedulingDecision> RRScheduler::get_next_thread() {
        std::shared_ptr<SchedulingDecision> decision = std::make_shared<SchedulingDecision>();
        if (!ready_queue.empty()) {
            decision->thread = ready_queue.front();
            decision->explanation = "Selected from " + std::to_string(ready_queue.size()) + " threads. Will run for at most " + std::to_string(time_slice) + " ticks.";
            ready_queue.pop();
        } else {
            decision->explanation = "No threads available for scheduling.";
        }
        decision->time_slice = time_slice;

        return decision;
}

void RRScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        ready_queue.push(thread);
}

size_t RRScheduler::size() const {
        return ready_queue.size();
}
