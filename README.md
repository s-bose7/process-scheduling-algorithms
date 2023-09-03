# Process Scheduling Unlocked!!

Process scheduling algorithms are primarily two types - preemptive and non-preemptive, while non-preemptive algorithms are easier to implement, preemptive algorithms are more suited for real-world use cases.

## Things I'm assuming:
1. The length of the Process Control Block is constant in the runtime of these algorithms.
2. We know the total burst time of the CPU, after loading the processes in main memory.
3. There would not be any I/O wait time associated with the processes.

## Non-preemptive Implementation
**Step 1**: Take input of the number of processes, and their respective burst time and arrival time, finally make a process control block, which holds all of the information about each process.

**Step 2:** Load all the processes into RAM.

**Step 3:** During the loading process, estimate a value of total burst time for all the processes in RAM. Assuming there are a limited number of processes & each has burst time associated with it, although in real-world scenarios we often are not able to estimate the total burst time, for those cases we’d have to predict the value of total burst time.

**Step 4:** Declare a time variable, which will track the schedule time of each process. Initially, `time = 0`.

**Step 5:** Iterate until `time <= estimated_total_burst_time`.

**Step 6:** For each iteration, update the READY_QUEUE. The algorithm will pick the process that has Minimal burst time for SJF, Highest priority for Priority scheduling, and Lowest arrival time for FCFS.

**Step 7:** After picking a process, compute its wait time and turnaround time, and flag the process as terminated.

**Step 8:** Finally, estimate the average wait time and average turnaround time for all of the processes in the process control block.

## Preemptive Implementation
Same as up to **Step 5**.

**Step 6:** To preempt processes from the CPU and put them back into the ready queue, use a linked list. Each node will store a process, and if a process needs to preempt, put the running process back into a node, and insert this node at the tail of READY_QUEUE. If the process finishes running, destroy the node after updating necessary info in PCB. Similar to non-preemptive, update the ready queue in each iteration to store newly arrived processes in the ready queue.

**Step 7:** After picking a process from the ready queue, compute its wait time and make a decision on whether this process has finished running or needs to be preempted back to the ready queue. If it finishes, compute its turnaround time and update the PCB.

**Step 8:** Finally, estimate the average wait time and average turnaround time for all of the processes in the process control block.
