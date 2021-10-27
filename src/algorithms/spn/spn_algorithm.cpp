#include "algorithms/spn/spn_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

SPNScheduler::SPNScheduler(int slice) {
    if (slice != -1) {
        throw("SPN must have a timeslice of -1");
    }
}

std::shared_ptr<SchedulingDecision> SPNScheduler::get_next_thread() {
        std::shared_ptr<SchedulingDecision> decision = std::make_shared<SchedulingDecision>();
        if (!ready_queue.empty()) {
            decision->thread = ready_queue.top();
            decision->explanation = "Selected from " + std::to_string(ready_queue.size()) + " threads. Will run to completion of burst.";
            decision->time_slice = -1;
            ready_queue.pop();
        } else {
            decision->explanation = "No threads available for scheduling.";
        }

        return decision;
}

void SPNScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        ready_queue.push(thread->bursts.front()->length, thread);
}

size_t SPNScheduler::size() const {
        return ready_queue.size();
}
