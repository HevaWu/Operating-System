#include <iostream>
#include <vector>
#include <iomanip>
#include "cpu_schd.h"

using namespace std;



int main()
{
	
	
	//5.3
	cpu_schd cpu1;
	cpu1.get_process();  //initialize the process, input the arrival time, burst time and the priority of each process

	//1
	cpu1.fcfs();  //First-Come-First-Serving scheduling algorithm

	//2
	cpu1.sjf();  //Shortest-Job-First scheduling algorithm

	//3
	//has the idle value of 1
	cpu1.sjf();
	
	//5.12
	cpu_schd cpu2;
	cpu2.get_process();  //initialize the process, input the arrival time, burst time and the priority of each process
	
	//1
	cpu2.fcfs();  //First-Come-First-Serving scheduling algorithm

	//2
	cpu2.sjf();  //Shortest-Job-First scheduling algorithm

	//3
	cpu2.np_priority(); //Nonpreemptive Priority scheduling algorithm

	//4
	cpu2.rr();  //Round-Robin scheduling algorithm

	return 0;
}