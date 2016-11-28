#pragma once


#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>
#include "cpu_run.h"

using namespace std;

void fcfs_simulation(cpu_schd cpu2, bool cpu_state, bool io_state);
void sjf_simulation(cpu_schd cpu2, bool cpu_state, bool io_state);
bool check_end(vector<double> check, double n);
double find_job(vector<double> find, double a);
double possion_random(double mean);

void fcfs_simulation(cpu_schd cpu2, bool cpu_state, bool io_state)
{
	cpu2.output_process();

	vector<double> pid_s;
	vector<double> burst_time_s;
	vector<double> arrival_time_s;
	vector<double> length_s;
	vector<double> length_end_s;
	vector<double> burst_control_state_s;
	double number;
	double io_run_s;
	pid_s = cpu2.get_process();
	burst_time_s = cpu2.get_burst_time();
	arrival_time_s = cpu2.get_arrival_time();
	length_s = cpu2.get_length();
	length_end_s = cpu2.get_length_end();
	burst_control_state_s = cpu2.get_burst_control_state();
	number = cpu2.get_num_p();
	io_run_s = cpu2.get_io_run();

	vector<double> ready_queue(number);
	vector<double> io_queue(number);

	//initialize the I/O queue and ready queue
	for (int i = 0; i < number; i++)
	{
		ready_queue[i] = 11;   //there are only 10 jobs, 11 means there is no job in the queue
		io_queue[i] = 11;
	}

	//record the position of the jobs
	int k;
	int j;
	double pick;
	k = 0;
	j = 0;
	for (int i = 0; i < number; i++)
	{
		if (burst_control_state_s[pid_s[i]] == 1)
			pick = pid_s[i];
		else if (burst_control_state_s[pid_s[i]] == 3)
		{
			ready_queue[k] = pid_s[i];
			k = k + 1;
		}
		else if (burst_control_state_s[pid_s[i]] == 2)
		{
			io_queue[j] = pid_s[i];
			j = j + 1;
		}
	}

	double cpu_runtime;   //record the cpu running
	cpu_runtime = 0;
	double io_runtime;   //record the io running
	io_runtime = 0;
	double cpu_time;   //record the whole time of cpu
	cpu_time = 0;
	//double whole_time; //as the whole time of the simulation
	//whole_time = 0;

	vector<double> turnaround_time(number);
	vector<double> waiting_time(number);
	vector<double> io_arrive_time(number);   //record the time when job into the io queue
	double average_turnaround_time;
	double average_waiting_time;
	average_turnaround_time = 0;
	average_waiting_time = 0;

	for (int i = 0; i < number; i++)
	{
		io_arrive_time[i] = 0;
	}

	//prepare fcfs
	waiting_time[pick] = waiting_time[pick] + cpu_runtime - arrival_time_s[pick];
	cpu_time = cpu_time + burst_time_s[pick];
	cpu_runtime = cpu_runtime + burst_time_s[pick];
	length_end_s[pick] = length_end_s[pick] - burst_time_s[pick];
	j = find_job(io_queue, 11);
	io_queue[j] = pick;  //put the pick job into the io queue
	ready_queue[0] == 11;
	io_arrive_time[pick] = cpu_time;
	burst_control_state_s[pick] = 2;
	int count;  //count how many operations the simulation do
	count = 0;

	for (bool check_1 = false; check_1 != true;)
	{
		if (cpu_time <= io_runtime)
		{
			cpu_state = false;
			j = find_job(io_queue, 11);
			if (ready_queue[0] != 11) 
			{
				pick = ready_queue[0];
				burst_control_state_s[pick] = 1;
				if (burst_time_s[pick] <= length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

					cpu_time = cpu_time + burst_time_s[pick];
					cpu_runtime = cpu_runtime + burst_time_s[pick];
					length_end_s[pick] = length_end_s[pick] - burst_time_s[pick];  //shorten the length
					cpu_state = true;

					burst_control_state_s[pick] = 2;
					burst_time_s[pick] = possion_random(30 + pick * 5);
					count = count + 1;
				}
				else if (burst_time_s[pick] > length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

					cpu_time = cpu_time + length_end_s[pick];
					cpu_runtime = cpu_runtime + length_end_s[pick];
					length_end_s[pick] = length_end_s[pick] - length_end_s[pick];  //shorten the length
					cpu_state = true;

					burst_control_state_s[pick] = 4;
					burst_time_s[pick] = possion_random(30 + pick * 5);
					count = count + 1;
				}

				//find how many jobs are in the io_queque
				j = find_job(io_queue, 11);
				io_queue[j] = pick;  //put the pick job into the io queue
				ready_queue[0] = 11;
				io_arrive_time[pick] = cpu_time;
			}
			else //if(ready_queue[0] == 11)
			{
				cpu_time = cpu_time + io_run_s;
				count = count + 1;
			}
		}
		else if (cpu_time > io_runtime)
		{
			io_state = false;
			if (io_queue[0] != 11)
			{
				pick = io_queue[0];
				if (io_run_s <= length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + cpu_time - io_arrive_time[pick];

					io_runtime = io_runtime + io_run_s;
					length_end_s[pick] = length_end_s[pick] - io_run_s;  //shorten the length
					io_state = true;

					burst_control_state_s[pick] = 3;
					count = count + 1;
				}
				else if (io_run_s > length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + cpu_time - io_arrive_time[pick];

					io_runtime = io_runtime + io_run_s;
					length_end_s[pick] = length_end_s[pick] - length_end_s[pick];  //shorten the length
					io_state = true;

					burst_control_state_s[pick] = 4;
					count = count + 1;
				}

				//find how many jobs are in the ready_queue
				k = find_job(ready_queue, 11);
				ready_queue[k] = pick;  //put the pick job into the ready queue
				io_queue[0] = 11;
				arrival_time_s[pick] = io_runtime;

			}
		}


		//initialize the I/O queue and ready queue
		for (int i = 0; i < number; i++)
		{
			ready_queue[i] = 11;   //there are only 10 jobs, 11 means there is no job in the queue
			io_queue[i] = 11;
		}
		k = 0;
		j = 0;
		for (int i = 0; i < number; i++)
		{
			if (burst_control_state_s[pid_s[i]] == 3)
			{
				ready_queue[k] = pid_s[i];
				k = k + 1;
			}
			else if (burst_control_state_s[pid_s[i]] == 2)
			{
				io_queue[j] = pid_s[i];
				j = j + 1;
			}
		}


		//update I/O queue
		double temp;
		double iq;  //record how many jobs in the ready_queue
		iq = find_job(io_queue, 11);
		for (int i = 0; i < iq - 1; i++)
		{
			for (int j = i + 1; j < iq; j++)
			{
				if (io_arrive_time[io_queue[i]] > io_arrive_time[io_queue[j]])
				{
					temp = io_queue[i];
					io_queue[i] = io_queue[j];
					io_queue[j] = temp;
				}
			}
		}
		count = count + 1;


		//update ready queue 
		//sort the ready queue according to the arrival time and the pid
		double rq;  //record how many jobs in the ready_queue
		rq = find_job(ready_queue, 11);
		for (int i = 0; i < rq - 1; i++)
		{
			for (int j = i + 1; j < rq; j++)
			{
				if (arrival_time_s[ready_queue[i]] > arrival_time_s[ready_queue[j]])
				{
					temp = ready_queue[i];
					ready_queue[i] = ready_queue[j];
					ready_queue[j] = temp;
				}
			}
		}
		count = count + 1;

		//find if the simulation can stop
		check_1 = check_end(length_end_s, 0);
	}

	//output the cpu utilization
	double cpu_use;
	cpu_use = cpu_runtime / cpu_time;
	cout << "The CPU utilization is: " << cpu_use << endl;



	//output the waiting time and turnaround time of each process
	vector<double> end_process(number);
	for (int g = 0; g < number; g++)
	{
		if (arrival_time_s[g] < io_arrive_time[g])
			end_process[g] = io_arrive_time[g];
		else if (arrival_time_s[g] >= io_arrive_time[g])
			end_process[g] = arrival_time_s[g];
		turnaround_time[g] = end_process[g];
		cout << "The waiting time of Process P" << g + 1 << " is: " << waiting_time[g] << endl;
		cout << "The turnaround time of Process P" << g + 1 << " is: " << turnaround_time[g] << endl;
		average_waiting_time = average_waiting_time + waiting_time[g];
		average_turnaround_time = average_turnaround_time + turnaround_time[g];
	}

	//output the throughput
	double throughput;
	double end_simulation;
	end_simulation = end_process[0];
	for (int i = 0; i < number; i++)
	{
		if (end_process[i] > end_simulation)
			end_simulation = end_process[i];
	}
	throughput = 10 / end_simulation;
	throughput = throughput * 1000;   //change 'ms' into 's'
	cout << "The throughput is: " << throughput << endl;


	average_waiting_time = static_cast<int>(average_waiting_time) / static_cast<int>(number);
	average_turnaround_time = static_cast<int>(average_turnaround_time) / static_cast<int>(number);

	cout << "The average waiting time is: " << average_waiting_time << endl;
	cout << "The average turnaround time is: " << average_turnaround_time << endl;
	cout << endl;
}

void sjf_simulation(cpu_schd cpu2, bool cpu_state, bool io_state)
{
	cpu2.output_process();

	vector<double> pid_s;
	vector<double> burst_time_s;
	vector<double> arrival_time_s;
	vector<double> length_s;
	vector<double> length_end_s;
	vector<double> burst_control_state_s;
	double number;
	double io_run_s;
	pid_s = cpu2.get_process();
	burst_time_s = cpu2.get_burst_time();
	arrival_time_s = cpu2.get_arrival_time();
	length_s = cpu2.get_length();
	length_end_s = cpu2.get_length_end();
	burst_control_state_s = cpu2.get_burst_control_state();
	number = cpu2.get_num_p();
	io_run_s = cpu2.get_io_run();

	double a_value;    //a_value is use to predict the arrival time of jobs
	cout << "Please input the a values: ";
	cin >> a_value;

	vector<double> ready_queue(number);
	vector<double> io_queue(number);

	//initialize the I/O queue and ready queue
	for (int i = 0; i < number; i++)
	{
		ready_queue[i] = 11;   //there are only 10 jobs, 11 means there is no job in the queue
		io_queue[i] = 11;
	}

	//record the position of the jobs
	int k;
	int j;
	double pick;
	k = 0;
	j = 0;
	pick = 11;
	for (int i = 0; i < number; i++)
	{
		if (burst_control_state_s[pid_s[i]] == 1)
			pick = pid_s[i];
		else if (burst_control_state_s[pid_s[i]] == 3)
		{
			ready_queue[k] = pid_s[i];
			k = k + 1;
		}
		else if (burst_control_state_s[pid_s[i]] == 2)
		{
			io_queue[j] = pid_s[i];
			j = j + 1;
		}
	}
	//if there is no job in CPU, put the first job in ready queue into CPU
	if (pick == 11)
		pick = ready_queue[0];


	double cpu_runtime;   //record the cpu running
	cpu_runtime = 0;
	double io_runtime;   //record the io running
	io_runtime = 0;
	double cpu_time;   //record the whole time of cpu
	cpu_time = 0;

	vector<double> turnaround_time(number);
	vector<double> waiting_time(number);
	vector<double> io_arrive_time(number);   //record the time when job into the io queue
	double average_turnaround_time;
	double average_waiting_time;
	average_turnaround_time = 0;
	average_waiting_time = 0;

	for (int i = 0; i < number; i++)
	{
		io_arrive_time[i] = 0;
	}

	//prepare fcfs
	waiting_time[pick] = waiting_time[pick] + cpu_runtime - arrival_time_s[pick];
	cpu_time = cpu_time + burst_time_s[pick];
	cpu_runtime = cpu_runtime + burst_time_s[pick];
	length_end_s[pick] = length_end_s[pick] - burst_time_s[pick];
	j = find_job(io_queue, 11);
	io_queue[j] = pick;  //put the pick job into the io queue
	ready_queue[0] == 11;
	io_arrive_time[pick] = cpu_time;
	burst_control_state_s[pick] = 2;
	int count;  //count how many operations the simulation do
	count = 0;
	double burst_predict;   // predict the next burst time of CPU
	double burst_predict_last;  //record the last burst time of CPU
	burst_predict_last = 75; //set the first predict burst time is 75ms
	burst_predict = a_value * burst_time_s[pick] + (1 - a_value) * burst_predict_last;

	for (bool check_1 = false; check_1 != true;)
	{
		if (cpu_time <= io_runtime)
		{
			cpu_state = false;
			j = find_job(io_queue, 11);
			if (ready_queue[0] != 11) 
			{
				pick = ready_queue[0];
				burst_control_state_s[pick] = 1;
				if (burst_time_s[pick] <= length_end_s[pick])
				{
					if (burst_time_s[pick] <= burst_predict)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + burst_time_s[pick];
						cpu_runtime = cpu_runtime + burst_time_s[pick];
						length_end_s[pick] = length_end_s[pick] - burst_time_s[pick];  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 2;
						burst_time_s[pick] = possion_random(30 + pick * 5);
						count = count + 1;

						burst_predict_last = burst_predict;
						burst_predict = a_value * burst_time_s[pick] + (1 - a_value) * burst_predict_last;
					}
					else if (burst_time_s[pick] > burst_predict)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + burst_predict;
						cpu_runtime = cpu_runtime + burst_predict;
						length_end_s[pick] = length_end_s[pick] - burst_predict;  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 2;
						burst_time_s[pick] = possion_random(30 + pick * 5);
						count = count + 1;

						burst_predict_last = burst_predict;
						burst_predict = a_value * burst_time_s[pick] + (1 - a_value) * burst_predict_last;
					}

				}
				else if (burst_time_s[pick] > length_end_s[pick])
				{
					if (length_end_s[pick] <= burst_predict)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + length_end_s[pick];
						cpu_runtime = cpu_runtime + length_end_s[pick];
						length_end_s[pick] = length_end_s[pick] - length_end_s[pick];  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 4;
						burst_time_s[pick] = possion_random(30 + pick * 5);
						count = count + 1;

						burst_predict_last = burst_predict;
						burst_predict = a_value * length_end_s[pick] + (1 - a_value) * burst_predict_last;
					}
					else if (length_end_s[pick] > burst_predict)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + burst_predict;
						cpu_runtime = cpu_runtime + burst_predict;
						length_end_s[pick] = length_end_s[pick] - burst_predict;  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 4;
						burst_time_s[pick] = possion_random(30 + pick * 5);
						count = count + 1;

						burst_predict_last = burst_predict;
						burst_predict = a_value * length_end_s[pick] + (1 - a_value) * burst_predict_last;
					}

				}

				//find how many jobs are in the io_queque
				j = find_job(io_queue, 11);
				io_queue[j] = pick;  //put the pick job into the io queue
				ready_queue[0] = 11;
				io_arrive_time[pick] = cpu_time;

			}
			else 
			{
				cpu_time = cpu_time + io_run_s;
				count = count + 1;
			}
		}
		else if (cpu_time > io_runtime)
		{
			io_state = false;
			if (io_queue[0] != 11)
			{
				pick = io_queue[0];
				if (io_run_s <= length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + cpu_time - io_arrive_time[pick];

					io_runtime = io_runtime + io_run_s;
					length_end_s[pick] = length_end_s[pick] - io_run_s;  //shorten the length
					io_state = true;

					burst_control_state_s[pick] = 3;
					count = count + 1;
				}
				else if (io_run_s > length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + cpu_time - io_arrive_time[pick];

					io_runtime = io_runtime + io_run_s;
					length_end_s[pick] = length_end_s[pick] - length_end_s[pick];  //shorten the length
					io_state = true;

					burst_control_state_s[pick] = 4;
					count = count + 1;
				}

				//find how many jobs are in the ready_queue
				k = find_job(ready_queue, 11);
				ready_queue[k] = pick;  //put the pick job into the ready queue
				io_queue[0] = 11;
				arrival_time_s[pick] = io_runtime;

			}
		}


		//initialize the I/O queue and ready queue
		for (int i = 0; i < number; i++)
		{
			ready_queue[i] = 11;   //there are only 10 jobs, 11 means there is no job in the queue
			io_queue[i] = 11;
		}
		k = 0;
		j = 0;
		for (int i = 0; i < number; i++)
		{
			if (burst_control_state_s[pid_s[i]] == 3)
			{
				ready_queue[k] = pid_s[i];
				k = k + 1;
			}
			else if (burst_control_state_s[pid_s[i]] == 2)
			{
				io_queue[j] = pid_s[i];
				j = j + 1;
			}
		}


		//update I/O queue
		double temp;
		double iq;  //record how many jobs in the ready_queue
		iq = find_job(io_queue, 11);
		for (int i = 0; i < iq - 1; i++)
		{
			for (int j = i + 1; j < iq; j++)
			{
				if (io_arrive_time[io_queue[i]] > io_arrive_time[io_queue[j]])
				{
					temp = io_queue[i];
					io_queue[i] = io_queue[j];
					io_queue[j] = temp;
				}
			}
		}
		count = count + 1;


		//update ready queue 
		//sort the ready queue according to the arrival time and the pid
		double rq;  //record how many jobs in the ready_queue
		rq = find_job(ready_queue, 11);
		for (int i = 0; i < rq - 1; i++)
		{
			for (int j = i + 1; j < rq; j++)
			{
				if (burst_time_s[ready_queue[i]] > burst_time_s[ready_queue[j]])
				{
					temp = ready_queue[i];
					ready_queue[i] = ready_queue[j];
					ready_queue[j] = temp;
				}
			}
		}
		count = count + 1;

		//find if the simulation can stop
		check_1 = check_end(length_end_s, 0);
	}

	//output the cpu utilization
	double cpu_use;
	cpu_use = cpu_runtime / cpu_time;
	cout << "The CPU utilization is: " << cpu_use << endl;



	//output the waiting time and turnaround time of each process
	vector<double> end_process(number);
	for (int g = 0; g < number; g++)
	{
		if (arrival_time_s[g] < io_arrive_time[g])
			end_process[g] = io_arrive_time[g];
		else if (arrival_time_s[g] >= io_arrive_time[g])
			end_process[g] = arrival_time_s[g];
		turnaround_time[g] = end_process[g];
		cout << "The waiting time of Process P" << g + 1 << " is: " << waiting_time[g] << endl;
		cout << "The turnaround time of Process P" << g + 1 << " is: " << turnaround_time[g] << endl;
		average_waiting_time = average_waiting_time + waiting_time[g];
		average_turnaround_time = average_turnaround_time + turnaround_time[g];
	}

	//output the throughput
	double throughput;
	double end_simulation;
	end_simulation = end_process[0];
	for (int i = 0; i < number; i++)
	{
		if (end_process[i] > end_simulation)
			end_simulation = end_process[i];
	}
	throughput = 10 / end_simulation;
	throughput = throughput * 1000;   //change 'ms' into 's'
	cout << "The throughput is: " << throughput << endl;


	average_waiting_time = static_cast<int>(average_waiting_time) / static_cast<int>(number);
	average_turnaround_time = static_cast<int>(average_turnaround_time) / static_cast<int>(number);

	cout << "The average waiting time is: " << average_waiting_time << endl;
	cout << "The average turnaround time is: " << average_turnaround_time << endl;
	cout << endl;
}

void rr_simulation(cpu_schd cpu2, bool cpu_state, bool io_state)
{
	cpu2.output_process();

	vector<double> pid_s;
	vector<double> burst_time_s;
	vector<double> arrival_time_s;
	vector<double> length_s;
	vector<double> length_end_s;
	vector<double> burst_control_state_s;
	double number;
	double io_run_s;
	pid_s = cpu2.get_process();
	burst_time_s = cpu2.get_burst_time();
	arrival_time_s = cpu2.get_arrival_time();
	length_s = cpu2.get_length();
	length_end_s = cpu2.get_length_end();
	burst_control_state_s = cpu2.get_burst_control_state();
	number = cpu2.get_num_p();
	io_run_s = cpu2.get_io_run();

	double quantum;    //a_value is use to predict the arrival time of jobs
	cout << "Please input the quantum: ";
	cin >> quantum;

	vector<double> ready_queue(number);
	vector<double> io_queue(number);

	//initialize the I/O queue and ready queue
	for (int i = 0; i < number; i++)
	{
		ready_queue[i] = 11;   //there are only 10 jobs, 11 means there is no job in the queue
		io_queue[i] = 11;
	}

	//record the position of the jobs
	int k;
	int j;
	double pick;
	k = 0;
	j = 0;
	for (int i = 0; i < number; i++)
	{
		if (burst_control_state_s[pid_s[i]] == 1)
			pick = pid_s[i];
		else if (burst_control_state_s[pid_s[i]] == 3)
		{
			ready_queue[k] = pid_s[i];
			k = k + 1;
		}
		else if (burst_control_state_s[pid_s[i]] == 2)
		{
			io_queue[j] = pid_s[i];
			j = j + 1;
		}
	}
	

	double cpu_runtime;   //record the cpu running
	cpu_runtime = 0;
	double io_runtime;   //record the io running
	io_runtime = 0;
	double cpu_time;   //record the whole time of cpu
	cpu_time = 0;

	vector<double> turnaround_time(number);
	vector<double> waiting_time(number);
	vector<double> io_arrive_time(number);   //record the time when job into the io queue
	double average_turnaround_time;
	double average_waiting_time;
	average_turnaround_time = 0;
	average_waiting_time = 0;

	for (int i = 0; i < number; i++)
	{
		io_arrive_time[i] = 0;
	}

	//prepare fcfs
	waiting_time[pick] = waiting_time[pick] + cpu_runtime - arrival_time_s[pick];
	cpu_time = cpu_time + burst_time_s[pick];
	cpu_runtime = cpu_runtime + burst_time_s[pick];
	length_end_s[pick] = length_end_s[pick] - burst_time_s[pick];
	j = find_job(io_queue, 11);
	io_queue[j] = pick;  //put the pick job into the io queue
	ready_queue[0] == 11;
	io_arrive_time[pick] = cpu_time;
	burst_control_state_s[pick] = 2;
	int count;  //count how many operations the simulation do
	count = 0;

	for (bool check_1 = false; check_1 != true;)
	{
		if (cpu_time <= io_runtime)
		{
			cpu_state = false;
			j = find_job(io_queue, 11);
			if (ready_queue[0] != 11) //&& (ready_queue[0] != io_queue[j - 1]))
			{
				pick = ready_queue[0];
				burst_control_state_s[pick] = 1;
				if (burst_time_s[pick] <= length_end_s[pick])
				{
					if (burst_time_s[pick] <= quantum)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + burst_time_s[pick];
						cpu_runtime = cpu_runtime + burst_time_s[pick];
						length_end_s[pick] = length_end_s[pick] - burst_time_s[pick];  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 2;
						burst_time_s[pick] = 30 + pick * 5;
						count = count + 1;
					}
					else if (burst_time_s[pick] > quantum)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + quantum;
						cpu_runtime = cpu_runtime + quantum;
						length_end_s[pick] = length_end_s[pick] - quantum;  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 2;
						burst_time_s[pick] = 30 + pick * 5;
						count = count + 1;

					}

				}
				else if (burst_time_s[pick] > length_end_s[pick])
				{
					if (length_end_s[pick] <= quantum)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + length_end_s[pick];
						cpu_runtime = cpu_runtime + length_end_s[pick];
						length_end_s[pick] = length_end_s[pick] - length_end_s[pick];  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 4;
						burst_time_s[pick] = 30 + pick * 5;
						count = count + 1;

					}
					else if (length_end_s[pick] > quantum)
					{
						waiting_time[pick] = waiting_time[pick] + io_runtime - arrival_time_s[pick];

						cpu_time = cpu_time + quantum;
						cpu_runtime = cpu_runtime + quantum;
						length_end_s[pick] = length_end_s[pick] - quantum;  //shorten the length
						cpu_state = true;

						burst_control_state_s[pick] = 4;
						burst_time_s[pick] = 30 + pick * 5;
						count = count + 1;
					}

				}

				//find how many jobs are in the io_queque
				j = find_job(io_queue, 11);
				io_queue[j] = pick;  //put the pick job into the io queue
				ready_queue[0] = 11;
				io_arrive_time[pick] = cpu_time;

			}
			else 
			{
				cpu_time = cpu_time + io_run_s;
				count = count + 1;
			}
		}
		else if (cpu_time > io_runtime)
		{
			io_state = false;
			if (io_queue[0] != 11)
			{
				pick = io_queue[0];
				if (io_run_s <= length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + cpu_time - io_arrive_time[pick];

					io_runtime = io_runtime + io_run_s;
					length_end_s[pick] = length_end_s[pick] - io_run_s;  //shorten the length
					io_state = true;

					burst_control_state_s[pick] = 3;
					count = count + 1;
				}
				else if (io_run_s > length_end_s[pick])
				{
					waiting_time[pick] = waiting_time[pick] + cpu_time - io_arrive_time[pick];

					io_runtime = io_runtime + io_run_s;
					length_end_s[pick] = length_end_s[pick] - length_end_s[pick];  //shorten the length
					io_state = true;

					burst_control_state_s[pick] = 4;
					count = count + 1;
				}

				//find how many jobs are in the ready_queue
				k = find_job(ready_queue, 11);
				ready_queue[k] = pick;  //put the pick job into the ready queue
				io_queue[0] = 11;
				arrival_time_s[pick] = io_runtime;
			}
			else
			{
				io_runtime = io_runtime + io_run_s;
			}
		}


		//initialize the I/O queue and ready queue
		for (int i = 0; i < number; i++)
		{
			ready_queue[i] = 11;   //there are only 10 jobs, 11 means there is no job in the queue
			io_queue[i] = 11;
		}
		k = 0;
		j = 0;
		for (int i = 0; i < number; i++)
		{
			if (burst_control_state_s[pid_s[i]] == 3)
			{
				ready_queue[k] = pid_s[i];
				k = k + 1;
			}
			else if (burst_control_state_s[pid_s[i]] == 2)
			{
				io_queue[j] = pid_s[i];
				j = j + 1;
			}
		}


		//update I/O queue
		double temp;
		double iq;  //record how many jobs in the ready_queue
		iq = find_job(io_queue, 11);
		for (int i = 0; i < iq - 1; i++)
		{
			for (int j = i + 1; j < iq; j++)
			{
				if (io_arrive_time[io_queue[i]] > io_arrive_time[io_queue[j]])
				{
					temp = io_queue[i];
					io_queue[i] = io_queue[j];
					io_queue[j] = temp;
				}
			}
		}
		count = count + 1;

		count = count + 1;

		//find if the simulation can stop
		check_1 = check_end(length_end_s, 0);
	}

	//output the cpu utilization
	double cpu_use;
	cpu_use = cpu_runtime / cpu_time;
	cout << "The CPU utilization is: " << cpu_use << endl;



	//output the waiting time and turnaround time of each process
	vector<double> end_process(number);
	for (int g = 0; g < number; g++)
	{
		if (arrival_time_s[g] < io_arrive_time[g])
			end_process[g] = io_arrive_time[g];
		else if (arrival_time_s[g] >= io_arrive_time[g])
			end_process[g] = arrival_time_s[g];
		turnaround_time[g] = end_process[g];
		cout << "The waiting time of Process P" << g + 1 << " is: " << waiting_time[g] << endl;
		cout << "The turnaround time of Process P" << g + 1 << " is: " << turnaround_time[g] << endl;
		average_waiting_time = average_waiting_time + waiting_time[g];
		average_turnaround_time = average_turnaround_time + turnaround_time[g];
	}

	//output the throughput
	double throughput;
	double end_simulation;
	end_simulation = end_process[0];
	for (int i = 0; i < number; i++)
	{
		if (end_process[i] > end_simulation)
			end_simulation = end_process[i];
	}
	throughput = 10 / end_simulation;
	throughput = throughput * 1000;   //change 'ms' into 's'
	cout << "The throughput is: " << throughput << endl;


	average_waiting_time = static_cast<int>(average_waiting_time) / static_cast<int>(number);
	average_turnaround_time = static_cast<int>(average_turnaround_time) / static_cast<int>(number);

	cout << "The average waiting time is: " << average_waiting_time << endl;
	cout << "The average turnaround time is: " << average_turnaround_time << endl;
	cout << endl;
}

//check if there is some value in check vector are not equal to n
//if all of the value in the check are n, return true
//if not, return false
bool check_end(vector<double> check, double n)
{
	for (int i = 0; i < check.size(); i++)
	{
		if (check[i] != n)
		{
			return false;
		}
	}
	return true;
}

double find_job(vector<double> find, double a)
{
	for (int i = 0; i < find.size(); i++)
	{
		if (find[i] == a)
			return i;
	}
}

double max_value(vector<double> val)
{
	double max;
	max = val[0];
	for (int g = 0; g < val.size(); g++)
	{
		if (val[g] > max)
			max = val[g];
	}
	return max;
}

//use possion distribution to acculate the possibility of random
//reference from http://www.cplusplus.com/reference/random/poisson_distribution/
double possion_random(double mean)
{
	// construct a trivial random generator engine from a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::poisson_distribution<int> distribution(mean);
	return distribution(generator);
}
