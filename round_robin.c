#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

/* start here */
struct Process {

	int pid, wt, tat;
	int initial_burst_time, new_burst_times;
	int initial_arrival_time, new_arrival_times;
	bool terminated, inQueqe;
};

struct Node {
	struct Process p;
	struct Node* next;
};

/* end here */

struct Node* createNode(struct Process p){

	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->p = p;
	newNode->next = NULL;
	return newNode;
}

int get_queue_length(struct Node** head){

	struct Node* iter = *head;
	int queue_length = 0;
	while(iter != NULL){
		queue_length++;
		iter = iter->next;
	}
	return queue_length;
}


void LOADS_PROCESSES_IN_RAM(struct Process pcb[], int p_id[], int n, int b_time[], int a_time[], int* total_bt){

	int i;
	for(i = 0; i < n; i++){

		pcb[i].pid = p_id[i];
		pcb[i].initial_burst_time = b_time[i];
		pcb[i].new_burst_times = pcb[i].initial_burst_time;
		pcb[i].initial_arrival_time = a_time[i];
		pcb[i].new_arrival_times = pcb[i].initial_arrival_time;
		pcb[i].terminated = false;
		pcb[i].inQueqe = false;
		pcb[i].wt = 0;
		pcb[i].tat = 0;
		(*total_bt) += b_time[i];
	}

}


void update_queue(struct Node** READY_QUEUE, struct Node** QUEUE_BACK, struct Process pcb[], int* time, int* pcb_len){

	int i;
	for(i = 0; i < *pcb_len; i++){

		if(pcb[i].terminated || pcb[i].inQueqe){
			continue;
		}

		if(pcb[i].new_arrival_times <= *time){
			pcb[i].inQueqe = true;
			struct Node* newProcessNode = createNode(pcb[i]);
			if(*READY_QUEUE == NULL){
				*READY_QUEUE = newProcessNode;
				*QUEUE_BACK = newProcessNode;

			}else if(pcb[i].new_arrival_times < (*READY_QUEUE)->p.new_arrival_times){
				// Insert as the new head of the list
				newProcessNode->next = *READY_QUEUE;
				*READY_QUEUE = newProcessNode;

			}else{
				struct Node* prev = NULL;
				struct Node* iter = *READY_QUEUE;
				while(newProcessNode->p.new_arrival_times > iter->p.new_arrival_times){
					prev = iter;
					iter = iter->next;
				}
				newProcessNode->next = iter;
				prev->next = newProcessNode;
			}
		}
	}
}


void update_pcb(struct Process p, struct Process pcb[], int n, bool terminated){
	int i;
	for(i=0; i<n; i++){
		if(pcb[i].pid == p.pid){
			
			pcb[i].wt += p.wt;
			pcb[i].tat += p.tat;
			pcb[i].terminated = terminated;

			if(!terminated){
				pcb[i].new_burst_times = p.new_burst_times;
				pcb[i].new_arrival_times = p.new_arrival_times;

			}else{
				// Calculate Turnaround time
				pcb[i].tat = pcb[i].wt + pcb[i].initial_burst_time;
			}

			break;
		}
	}
}


void roundRobinMagic(struct Process p, struct Node** QUEUE_BACK){

	// Preemption
	struct Node* lastNode = createNode(p);

	(*QUEUE_BACK)->next = lastNode;
	*QUEUE_BACK = lastNode;
}

void remove_executed_node_from_queue(struct Node** READY_QUEUE){
	
	struct Node* executedNode = *READY_QUEUE;
	*READY_QUEUE = (*READY_QUEUE)->next;
	// executedNode->next = NULL; // eventually collected by garbage collector
	free(executedNode);
}


void execute(struct Node** READY_QUEUE, struct Node** QUEUE_BACK, struct Process pcb[], int n, int* t, int* tq){
	
	struct Process p = (*READY_QUEUE)->p;

	if(p.new_burst_times > *tq){
		// Calculate wait and turnaround time
		
		p.wt = *t - p.new_arrival_times;;
		// p.tat = p.wt + *tq; 
		p.new_burst_times -= *tq;
		*t += *tq;
		p.new_arrival_times = *t;
		update_pcb(p, pcb, n, false);
		// put the process at the back of the queue
		roundRobinMagic(p, QUEUE_BACK);

	}else{

		// Last CPU burst of p
		p.wt = *t - p.new_arrival_times;
		// p.tat = p.wt + p.new_burst_times;
		*t += p.new_burst_times;
		update_pcb(p, pcb, n, true);
	}
	// Remove p from the head of the list
	remove_executed_node_from_queue(READY_QUEUE);
}

void print_ready_queue(struct Node** READY_QUEUE){
	struct Node* temp = *READY_QUEUE;
	while(temp != NULL){
		printf("P%d ->", temp->p.pid);
		temp = temp->next;
	}
	printf("\n");
}

int main()
{
	/* code */
	printf("\n\t\tROUND ROBIN SCHEDULING (Preemptive)\t\n");
	int p_id[] = {1, 2, 3, 4};
	int n = sizeof p_id / sizeof p_id[0]; 
	int b_time[] = {5, 4, 2, 1};
	int a_time[] = {0, 1, 2, 3};
	// Theoretically speaking, CPU burst can be evaluated accurately
	// Although theory will only take you so far!
	int estimated_cpu_burst = 0;
	// Process Control Block which holds information about all the processes
	struct Process process_control_block[n];
	// Loads the processes in RAM & calculate total CPU burst
	LOADS_PROCESSES_IN_RAM(process_control_block, p_id, n, b_time, a_time, &estimated_cpu_burst);
	// Preparing the READY_QUEUE
	struct Node* READY_QUEUE = NULL;
	struct Node* QUEUE_BACK = NULL;
	// Scheduler timer & time_quanta 
	int time = 0, time_quanta = 2;

	while(time <= estimated_cpu_burst){	
		
		update_queue(&READY_QUEUE, &QUEUE_BACK, process_control_block, &time, &n);
		print_ready_queue(&READY_QUEUE);
		int current_queue_length = get_queue_length(&READY_QUEUE);
		printf("current_queue_length: %d\n", current_queue_length);	
		if(current_queue_length == 0){
			// No processes has arrieved, CPU is idle
			time++;
			continue;

		}else{
			execute(&READY_QUEUE, &QUEUE_BACK, process_control_block, n, &time, &time_quanta);
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
			process_control_block[i].pid, 
			process_control_block[i].initial_arrival_time,  
			process_control_block[i].initial_burst_time, 
			process_control_block[i].wt, 
			process_control_block[i].tat
		);
	}
	printf("\n\nAverage Turnaround Time: %f", avg_ta_time/n);
	printf("\nAverage Wait Time: %f\n\n",avg_wt_time/n);
	return 0;
}