#include "algorithms/priority/priority_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include "utilities/fmt/format.h"

PRIORITYScheduler::PRIORITYScheduler(int slice) {
    if (slice != -1) {
        throw("SPN must have a timeslice of -1");
    }

}

std::shared_ptr<SchedulingDecision> PRIORITYScheduler::get_next_thread() {
        std::shared_ptr<SchedulingDecision> decision = std::make_shared<SchedulingDecision>();
        if (!ready_queue.empty()) {
            std::map<int, std::queue<std::shared_ptr<Thread>>> map = ready_queue.getMQueues();
            int u[4];
            int v[4];

            for (int i = 0; i < 4; i++) {
                if (map.count(i)) {
                    u[i] = map.at(i).size();
                } else {
                    u[i] = 0;
                }
            }
            
            decision->thread = ready_queue.top();
            decision->explanation = "[S: " + std::to_string(u[0]) + " I: " + std::to_string(u[1]) + " N: " + std::to_string(u[2]) + " B: " + std::to_string(u[3]) + "] -> ";
            decision->time_slice = -1;
            ready_queue.pop();
            map = ready_queue.getMQueues();
            
            int sel = 0;
            for (int i = 0; i < 4; i++) {
                if (map.count(i)) {
                    v[i] = map.at(i).size();
                } else {
                    v[i] = 0;
                }

                if (v[i] < u[i]) {
                    sel = i;
                }
            }
            std::string str = sel == 0 ? "SYSTEM" : sel == 1 ? "INTERACTIVE" : sel == 2 ? "NORMAL" : "BATCH";
            decision->explanation += "[S: " + std::to_string(v[0]) + " I: " + std::to_string(v[1]) + " N: " + std::to_string(v[2]) + " B: " + std::to_string(v[3]) + "]";
            decision->explanation = "Selected from " + str + " queue. " + decision->explanation;
        } else {
            decision->explanation = "No threads available for scheduling.";
        }

        return decision;
}

void PRIORITYScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
        ready_queue.push(thread->priority == SYSTEM ? 0 : thread->priority == INTERACTIVE ? 1 : thread->priority == NORMAL ? 2 : 3, thread);
}

size_t PRIORITYScheduler::size() const {
        return ready_queue.size();
}
