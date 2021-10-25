#include "algorithms/fcfs/fcfs_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

/*
    Here is where you should define the logic for the FCFS algorithm.
*/

FCFSScheduler::FCFSScheduler(int slice) {
    if (slice != -1) {
        throw("FCFS must have a timeslice of -1");
    }
}

std::shared_ptr<SchedulingDecision> FCFSScheduler::get_next_thread() {
        // TODO: implement me!
        std::shared_ptr<SchedulingDecision> decision = std::make_shared<SchedulingDecision>();

        decision->thread = ready_queue.front();
        decision->explanation = "Selected from " + std::to_string(ready_queue.size()) + " threads. Will run to completion of burst.";
        decision->time_slice = -1;
        ready_queue.pop();

        return decision;
}

void FCFSScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        // TODO: implement me!
        ready_queue.push(thread);
}

size_t FCFSScheduler::size() const {
        // TODO: implement me!
        return ready_queue.size();
}
