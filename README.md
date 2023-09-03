#### Process scheduling unlocked!!

#### Process scheduling algorithms are primarily two types - preemptive and non-preeemptive, while non-preemtive algorithms are easier to implement, preemptive algorithms are more suited for real world use cases.



## Things I'm assuming:
  #### 1. The length of the Process Control Block is constant in runtime of these algorithms.
  #### 2. We know the total burst time of CPU, after loading the processes in main memory.
  #### 3. There would not be any I/O wait time associated with the processes.



## Non-preemptive implementation
  #### STEP 1: Take input of number of processes, and respective burst time, arrival time, finally make a process control block, which holds all of the information about each process.
  #### STEP 2: Load all the processes into RAM.
  #### STEP 3: During the loading process, we will estimate a value of total burst time for all the processes in the RAM. Assuming there are a limited number of processes & each has burst time associated                  with it, although in real world scenarios we often are not able to estimate the total burst time, for those cases we’d have to predict the value of total burst time.
  #### STEP 4: Declare a time variable, which will track the schedule time of each process. Initially time = 0;  
  #### STEP 5:  Iterate until time <= estimated_total_burst_time
  #### STEP 6: For each iteration we will update the READY_QUEUE, our algorithm will pick the process that has a Minimal burst time for SJF, Highest priority for Priority scheduling and Lowest arrival time                for FCFS.
  #### STEP 7; After picking a process, we will compute its wait time and turnaround time, and flag the process as terminated. 
  #### STEP 8: Finally we will estimate the avg wait time and avg turnaround time for all of the processes in the process control block.  


  ## Preemptive implementation
   #### Same upto STEP 5
   #### STEP 6: We need to preemt processes from the CPU and put it back to the ready queue, in non-preemtive cases we were using arrays as READY_QUEUE, here we will be using linked list,             each node will store a process, and if a process need to preempt then we will simply put the running process back into a node, and insert this node at the tail of READY_QUEUE, on the          other hand if the process finish running, then we will destroy the node after updating nesseary info in PCB. Finally similar to non-preemtive we will update the ready queue in each 
         iteration for stroing newly arrived processes in the ready queue.
   #### STEP 7: After picking a process from the ready queue, we need to compute its wait time and we have to make a decision on whether this process has finished running or it needs to 
         preempted back at the ready queue, if it finished then we will compute its turnaround time and update the pcb. 
   #### STEP 8: Finally we will estimate the avg wait time and avg turnaround time for all of the processes in the process control block.  
