#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

struct Process {
	int pid, bt, at, wt, tat;
	bool terminated;
};


int LOADS_PROCESSES_IN_RAM(struct Process pcb[], int p_id[], int n, int b_time[], int a_time[]){

	int i, total_bt = 0;
	for(i = 0; i < n; i++){

		pcb[i].pid = p_id[i];
		pcb[i].bt = b_time[i];
		pcb[i].at = a_time[i];
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
	// Selecting the shortest CPU Burst process among all the processes that has arrived
	// in the READY_QUEUE
	struct Process p;
	int i, shortest_job = INT_MAX;
	for(i=0; i<curr_len; i++){
		if(READY_QUEUE[i].bt < shortest_job){
			shortest_job = READY_QUEUE[i].bt;
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
	printf("\n\t\tSJF SCHEDULING\t\n");
	int p_id[] = {1, 2, 3, 4, 5};
	int n = sizeof p_id / sizeof p_id[0]; 
	int b_time[] = {6, 2, 8, 3, 4};
	int a_time[] = {2, 5, 1, 0, 4};
	// Process Control Block which holds information about all the processes
	struct Process process_control_block[n]; 
	int estimated_cpu_burst = LOADS_PROCESSES_IN_RAM(process_control_block, p_id, n, b_time, a_time);
	// Scheduler time instance
	int time = 0;
	while(time <= estimated_cpu_burst){
		struct Process READY_QUEUE[n];
		int current_queue_length = update_queue(READY_QUEUE, process_control_block, time, n);;
		if(current_queue_length == 0){
			// CPU sits idle 
			time++;
			continue;

		}else{
			execute(READY_QUEUE, current_queue_length, process_control_block, n, &time);
		}
	}
	// Evaluate Avg Turn around time & Avg wait time for processes in READY_QUE
	printf("\nProcess  \tArrival time\tBrust time\tWait time\tTurnaround time\n");
	double avg_ta_time = 0, avg_wt_time = 0;
	int i;
	for(i=0; i<n; i++)
	{	
		avg_ta_time += process_control_block[i].tat;
		avg_wt_time += process_control_block[i].wt;	
		printf(
			"\nP%d\t\t%d\t\t%d\t\t%d\t\t%d", 
			process_control_block[i].pid, process_control_block[i].at, process_control_block[i].bt, 
			process_control_block[i].wt, process_control_block[i].tat
		);
	}
	printf("\n\nAverage Turnaround Time: %f", avg_ta_time/n);
	printf("\nAverage Wait Time: %f\n\n",avg_wt_time/n);
	return 0;
}





