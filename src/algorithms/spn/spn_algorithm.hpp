#ifndef SPN_ALGORITHM_HPP
#define SPN_ALGORITHM_HPP

#include <memory>
#include "algorithms/scheduling_algorithm.hpp"
#include "utilities/stable_priority_queue/stable_priority_queue.hpp"

/*
    SPNScheduler:
        A representation of a scheduling queue that uses shortest process next logic.
        
        This is a derived class from the base scheduling algorithm class.
*/

class SPNScheduler : public Scheduler {
public:

    //==================================================
    //  Member variables
    //==================================================

    Stable_Priority_Queue<std::shared_ptr<Thread>> ready_queue;

    //==================================================
    //  Member functions
    //==================================================

    SPNScheduler(int slice = -1);

    std::shared_ptr<SchedulingDecision> get_next_thread();

    void add_to_ready_queue(std::shared_ptr<Thread> thread);

    size_t size() const;

};

#endif
