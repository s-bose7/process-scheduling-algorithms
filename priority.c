#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

struct Process {
	int pid, bt, at, wt, tat, priority;
	bool terminated;
};


int LOADS_PROCESSES_IN_RAM(struct Process pcb[], int p_id[], int n, int b_time[], int a_time[], int p[]){

	int i, total_bt = 0;
	for(i = 0; i < n; i++){

		pcb[i].pid = p_id[i];
		pcb[i].bt = b_time[i];
		pcb[i].at = a_time[i];
		pcb[i].priority = p[i];
		pcb[i].terminated = false;
		total_bt += b_time[i];
	}

	return total_bt;
}


int update_queue(struct Process RQ[], struct Process pcb[], int time_instance, int n){
	// Afer each CPU burst - updating the READY_QUEUE with new processes
	// that has arrived according to the arrival time in Process Control Block
	int i, len = 0;
	for(i=0; i<n; i++){
		if(pcb[i].at <= time_instance){
			if(!(pcb[i].terminated)){
				RQ[len] = pcb[i];
				len++;
			}
		}
	}
	return len;
}

void execute(struct Process READY_QUEUE[], int curr_len, struct Process pcb[], int n, int* time){
	// Selecting the Highest priority process among all the processes that has arrived
	// in the READY_QUEUE
	struct Process p;
	int i, highest_priority = INT_MAX;
	for(i=0; i<curr_len; i++){
		if(READY_QUEUE[i].priority < highest_priority){
			highest_priority = READY_QUEUE[i].priority;
			p = READY_QUEUE[i];
		}
	}
	// Calculate wt, tat for p 
	p.wt = *time - p.at;
	p.tat = p.wt + p.bt;
	// Update schedule timer
	*time += p.bt;
	// Update PCB
	for(i=0; i<n; i++){
		if(pcb[i].pid == p.pid){
			pcb[i].wt = p.wt;
			pcb[i].tat = p.tat;
			pcb[i].terminated = true;
			break;
		}
	}
}


int main(){
	printf("\n\t\tPRIORITY SCHEDULING (Non preemptive)\n");
	int p_id[] = {1, 2, 3, 4, 5};
	int n = sizeof p_id / sizeof p_id[0]; 
	int b_time[] = {6, 2, 8, 3, 4};
	int a_time[] = {2, 5, 1, 0, 4};
	int priority[] = {3, 1, 4, 5, 2};
	// Process Control Block which holds information about all the processes
	struct Process p_c_b[n]; 
	int estimated_cpu_burst = LOADS_PROCESSES_IN_RAM(p_c_b, p_id, n, b_time, a_time, priority);
	// Scheduler time instance
	int time = 0;
	while(time <= estimated_cpu_burst){
		struct Process READY_QUEUE[n];
		int current_queue_length = update_queue(READY_QUEUE, p_c_b, time, n);;
		if(current_queue_length == 0){
			// CPU sits idle 
			time++;
			continue;

		}else{
			execute(READY_QUEUE, current_queue_length, p_c_b, n, &time);
		}
	}
	// Evaluate Avg Turn around time & Avg wait time for processes in READY_QUE
	printf("\nProcess  \tArrival time\tBrust time\tPrority\t\tWait time\tTurnaround time\n");
	double avg_ta_time = 0, avg_wt_time = 0;
	int i;
	for(i=0; i<n; i++)
	{	
		avg_ta_time += p_c_b[i].tat;
		avg_wt_time += p_c_b[i].wt;	
		printf(
			"\nP%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d", 
			p_c_b[i].pid, p_c_b[i].at, p_c_b[i].bt, 
			p_c_b[i].priority, p_c_b[i].wt, p_c_b[i].tat
		);
	}
	printf("\n\nAverage Turnaround Time: %f ms", avg_ta_time/n);
	printf("\nAverage Wait Time: %f ms\n\n",avg_wt_time/n);
	return 0;
}





