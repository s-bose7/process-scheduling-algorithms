#### process scheduling unlocked!!

#### Process scheduling algorithms are primarily two types - preemptive and non-preeemptive, while non-preemtive algorithms are easier to implement, preemptive algorithms are more suited for real world use cases



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
