/*
		PROCESS SCHEDULING USING LINKED LIST

		-----------------------------------
				ACHUTE SHARMA
			    	SMIT 
		      achutes@gmail.com
		-----------------------------------
		
		input  -> burst time and arrival time

		output -> gantt chart , wating time and turn around time



		BUG 1:
		if we run sjf and sjf again we get diffrent results due to the fact et!=0 the second time and simillarly with 
		srtf where rbt=0 

		FIX 1:
			call the runFirst()	function before sjf and srtf 
		
		BUG 2:
			if we insert it once more pid will be kept need to reinitilize it to zero.

*/
#include<stdio.h>
struct process{
	int pid;		//process id
	int bt;			//busrt time
	int at;			//arival time
	int wt;			//waiting time
	int tt;			//turn around time

	int et;			//start of execution time	needed for SJF

	int rbt;		// remaining burst time ; needed for SRTF 
	int pv;			// priority value ; needed for priority scheduling (lower the value higher the priority)
	struct process *next;
};
struct process *front=NULL,*new,*temp=NULL;
struct process *ganttchart;	//		needed for SJF
int total_burst=0,lst_arrival=0;
int p=0;
void runFirst()
{
	/*
		this function will reinitilize the et and rbt , wt and tt .
	*/
	temp=front;
	while(temp!=NULL)
	{
		temp->rbt=temp->bt;
		temp->wt=0;
		temp->tt=0;
		temp->et=0;
		temp->pv=100;
		temp=temp->next;
	}
}
void insert(int b,int a){
	temp=front;
	new = (struct process *)malloc(sizeof(struct process));
	new->pid=++p;
	new->bt=b;
	new->at=a;

	new->wt=0;
	new->tt=0;
	new->et=0;
	new->rbt=b;		// FOR SRTF
	new->next=NULL;
	if(front==NULL)
		front=new;
	else
	{
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new;		
	}
}
void display(int n){
	float avgWT=0,avgTT=0;
	temp=front;
	printf("\n PROCESS ID\tBURST TIME\tARRIVAL TIME\tWAITING TIME\tTURNAROUND TIME");
	while(temp!=NULL)
	{
		printf("\n %d \t\t %d \t\t %d \t\t %d \t\t %d",temp->pid,temp->bt,temp->at,temp->wt,temp->tt);
		avgWT+=temp->wt;
		avgTT+=temp->tt;
		temp=temp->next;
	}
	printf("\n Average WAITING TIME \t %f \t Average TURNAROUND TIME\t %f \n",(avgWT/n),avgTT/n);
}
void fcfs()
{
	//draw the gantt chart for FCFS 
	int time=0;
	//ganttchart=front;
	temp=front;
	while(temp!=NULL)
	{
		if(time>=temp->at)
		{
			
			printf("\n Process %d at Time %d ",temp->pid,time);
			time+=temp->bt;
			//therefore time=completion time now
			temp->wt=time-(temp->at+temp->bt);
			temp->tt=time-(temp->at);
			temp=temp->next;			
			
		}
		else
		{
			printf("\n CPU IDLE : %d",time);
			time++;
		}
	}

}
void sjf(){
	// NON PREEMPTIVE 
	// SHORTEST JOB FIRST YO !!
	int time=0;
	int min=total_burst;			//min = minimum burst time ; here it is initilized with total burst time for comparision
	runFirst();
	temp=front;
	while(time<=total_burst){
			//SCAN FOR THE SHORTEST BURST TIME whose arrival time is less than or equals to the time
			
			ganttchart=NULL;
			temp=front;
			min=total_burst;
			while(temp!=NULL)
			{
				if(temp->et==0)
				{
					if((time>=temp->at)&&(temp->bt<min))
					{
						min=temp->bt;
						ganttchart=temp;
					}	
				}
				temp=temp->next;
			}
			if(ganttchart==NULL)
			{
				//no process was found ..there fore idle time
				printf("\n CPU IDLE at %d",time);
				time++;
			}
			else{
				printf("\n Process %d at Time %d",ganttchart->pid,time);
				time+=ganttchart->bt;
				ganttchart->et+=1;
				ganttchart->wt=time-(ganttchart->at+ganttchart->bt);
				ganttchart->tt=time-(ganttchart->at);
			}
	}
}
void srtf()
{
		/* SRTF
		// SHORTEST REMAINING TIME FIRST --> simillar to SJF but preemptive
		// need to add a new field called rbt -> REMAINING BURST TIME
		
				LOGIC
		same like sjf but once a new process is encountered , again check for minimum burst time.
		
		since this is a simulation .. lets check for new process every 1 ms 
		
		*/
		int time=0;
		int min=total_burst;
		runFirst();
		while(time<=total_burst)
		{
			ganttchart=NULL;
			temp=front;
			min=total_burst;
			while(temp!=NULL)
			{
				if(temp->rbt!=0){
					if(time>=temp->at&&min>temp->rbt)
					{
						min=temp->rbt;
						ganttchart=temp;
					}
					
				}
				temp=temp->next;
			}
			if(ganttchart==NULL)
			{
				//CPU IDLE
				printf("\n CPU IDLE at Time %d",time);
				time++;
			}
			else{
				printf("\n Process %d at Time %d ",ganttchart->pid,time);
				time+=1;
				ganttchart->rbt-=1;
				ganttchart->wt=time-(ganttchart->at+ganttchart->bt);
				ganttchart->tt=time-ganttchart->at;
			}
		}
	
}
void priorityNP()
{
	// NON Preemptive Priority Scheduling 
	//DECLARING LOCAL VARIABLES
	int time=0;
	int min=101;					//min = minimum priority value
	runFirst();
	// NEED TO ADD EXTRA PRIORITY ELEMENT 
	temp=front;
	printf("\n ENTER PRIORITY VALUES \n");
	while(temp!=NULL)
	{
		printf("\n For Process %d ",temp->pid);
		scanf("%d",&temp->pv);
		temp=temp->next;
	}
	//NOW PERFORM THE SCHEDULING
	while(time<=total_burst)
	{
		ganttchart=NULL;
		temp=front;
		min=101;				// assigning it the highest priority value
		while(temp!=NULL)
		{
			if(temp->et==0)
			{
				//the process is not yet executed et==0
				if((min>temp->pv)&&(time>=temp->at))
				{
					min=temp->pv;
					ganttchart=temp;
				}
			}
		temp=temp->next;
		}
		if(ganttchart==NULL)
			time++;
		else
		{
			printf("\n Process %d at Time %d ",ganttchart->pid,time);
			ganttchart->et+=1;
			time+=ganttchart->bt;
			ganttchart->wt=time-(ganttchart->at+ganttchart->bt);
			ganttchart->tt=time-(ganttchart->at);
		}
		
	}
	
}
void priorityP()
{
	// Preemptive Priority Scheduling 
	//DECLARING LOCAL VARIABLES
	int time=0;
	int min=101;					//min = minimum priority value
	runFirst();
	// NEED TO ADD EXTRA PRIORITY ELEMENT 
	temp=front;
	printf("\n ENTER PRIORITY VALUES \n");
	while(temp!=NULL)
	{
		printf("\n For Process %d ",temp->pid);
		scanf("%d",&temp->pv);
		temp=temp->next;
	}
	//NOW PERFORM THE SCHEDULING
	while(time<=total_burst)
		{
			ganttchart=NULL;
			temp=front;
			min=101;
			while(temp!=NULL)
			{
				if(temp->rbt!=0){
					if(time>=temp->at&&min>temp->pv)
					{
						min=temp->pv;
						ganttchart=temp;
					}
					
				}
				temp=temp->next;
			}
			if(ganttchart==NULL)
			{
				//CPU IDLE
				printf("\n CPU IDLE at Time %d",time);
				time++;
			}
			else{
				printf("\n Process %d at Time %d ",ganttchart->pid,time);
				time+=1;
				ganttchart->rbt-=1;
				ganttchart->wt=time-(ganttchart->at+ganttchart->bt);
				ganttchart->tt=time-ganttchart->at;
			}
		}
}
void main()
{
	int ch,i,n,a,b;
	// INSERT ONCE ONLY ..
	// AND DISPLAY .. 
	printf("Enter the no of process \n");
	scanf("%d",&n);
	for(i=0;i<n;i++)
		{
			printf("\n Enter the Burst time And Arrival Time for Process \t %d \t",p+1);
			scanf("%d %d",&b,&a);
			insert(b,a);
			/*
				NOT REQUIRED FOR FCFS
			*/
			if(i==0||i==n-1)
				total_burst+=a; 	// APPLICABLE FOR SJF	
			total_burst+=b;			
			//lst_arrival=a;
	}
	do
	{
		printf("\n Enter the choice \n");
		printf("\n 1 . Display \n");
		printf("\n 2 . FCFS \n");
		printf("\n 3 . SJF \n");
		printf("\n 4 . SRTF \n");
		printf("\n 5 . PRIORITY NON PREEMPTIVE \n");
		printf("\n 6 . PRIORITY PREEMPTIVE \n");
		printf("\n 9 . QUIT \n");
		scanf("%d",&ch);
		switch(ch){
		case 1:	display(n);
				break;
		case 2: fcfs();
				display(n);
				break;
		case 3:	sjf();
				display(n);
				break;
		case 4: srtf();
				display(n);
				break;
		case 5: priorityNP();
				display(n);
				break;
		case 6: priorityP();
				display(n);
				break;
		case 9:	printf("\n QUITING .. \n");
				break;
		default:printf("Enter Valid choice \n");
		}
	}while(ch!=9);
}