#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>
#include "cpu_run.h"
#include "schedule_algorithm.h"

using namespace std;

int main()
{
	bool cpu_state;  //initialize the state of cpu, when cpu is true, it means cpu is free
	cpu_state = true;  //when cpu is false, it means cpu is busy 
	bool io_state;  //initialize the state of I/O, when I/O is false, it means I/O is busy   
	io_state = true;  // when I/O is true, it means I/O is free

	cpu_schd cpu1; //create one cpu1 simulation

	fcfs_simulation(cpu1, cpu_state, io_state);

	sjf_simulation(cpu1, cpu_state, io_state);
	
	//for promising there is only the quantum difference
	//create one fixed cpu, which burst time is not changing
	//cpu_schd cpu3(true);
	//rr_simulation(cpu3, cpu_state, io_state);

	return 0;
}
