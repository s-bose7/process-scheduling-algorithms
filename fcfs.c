#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

struct Process {
	int pid, bt, at, ct, wt, tat;
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
	// Selecting the process that has lowest arrival time among all the processes arrived in the
	// READY_QUEUE
	struct Process p;
	int i, lowest_arrival_time = INT_MAX;
	for(i=0; i<curr_len; i++){
		if(READY_QUEUE[i].at < lowest_arrival_time){
			lowest_arrival_time = READY_QUEUE[i].at;
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

	printf("\n\t\tFCFS SCHEDULING\t\n");
	int process_id[] = {1, 2, 3, 4};
	int n = sizeof process_id / sizeof process_id[0]; 
	int burst_time[] = {2, 1, 10, 20};
	int arrival_time[] = {4, 1, 2, 9};
	
	// Process Control Block which holds information about all the processes
	struct Process process_control_block[n]; 
	int estimated_cpu_burst = LOADS_PROCESSES_IN_RAM(process_control_block, process_id, n, burst_time, arrival_time);

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
	double avg_ta_time, avg_wt_time;
	int p=0;
	for(p=0; p<n; p++){
		
		avg_ta_time += process_control_block[p].tat;
		avg_wt_time += process_control_block[p].wt;
		
		printf(
			"\nP%d\t\t%d\t\t%d\t\t%d\t\t%d", 
			process_control_block[p].pid, process_control_block[p].at, process_control_block[p].bt, 
			process_control_block[p].wt, process_control_block[p].tat
		);
	}
	printf("\n\nAverage Turnaround Time: %f", avg_ta_time/p);
	printf("\nAverage Wait Time: %f\n\n",avg_wt_time/p);
	return 0;
}





