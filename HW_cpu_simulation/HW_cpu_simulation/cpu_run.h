#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

class cpu_schd
{
private:
	vector<double> process;
	vector<double> burst_time;
	vector<double> arrival_time;
	vector<double> priority;
	vector<double> pid;
	vector<double> length;
	vector<double> length_end;   //control the length of job, when length_end == 0, means the job is finishing
	vector<double> burst_control_state;
	double io_run;
	double idle;
	double num_p;  //number of process

public:
	cpu_schd();
	cpu_schd(bool judge);

	//initialize the process, burst_time, arrival_time, and priority
	vector<double> process_initialize();
	vector<double> burst_time_initialize();
	vector<double> arrival_time_initialize();
	vector<double> priority_initialize();
	vector<double> length_initialize();
	vector<double> pid_initialize();
	vector<double> burst_control();   // the number represent the state of the process
	void swap_process(double a, double b);
	bool check_arrival(vector<double> check, int n);
	void output_process();
	vector<double> get_process();
	vector<double> get_burst_time();
	vector<double> get_arrival_time();
	vector<double> get_priority();
	vector<double> get_pid();
	vector<double> get_length();
	vector<double> get_length_end();
	vector<double> get_burst_control_state();
	double get_io_run();
	double get_num_p();

	double possion_random(double mean);   //random for I/O interval
	double uniform_random_length(int, int);  //random for length of jobs

};

vector<double> cpu_schd::process_initialize()
{
	double number;
	number = num_p;
	vector<double> process_1(number);
	for (int i = 0; i < number; i++)
	{
		process_1[i] = i;
	}
	return process_1;
}

//initialize the burst control vector, the value represent the state of the process
vector<double> cpu_schd::burst_control()
{
	double number;
	number = num_p;
	vector<double> burst_control_1(number);

	//initialize the burst_control_1 vector
	for (int i = 0; i < number; i++)
	{
		burst_control_1[i] = -1;
	}

	//put each process into different places, ready queue or I/O waiting queue, or CPU
	//1 represent process is in the CPU 
	//2 represent process is in the I/O waiting queue
	//3 represent process is in the ready queue
	for (int i = 0; i < number; i++)
	{
		burst_control_1[i] = uniform_random_length(1, 3);
		if (burst_control_1[i] == 1)
		{
			for (int j = 0; j <= i; j++)
			{
				if ((burst_control_1[j] == 1) && (j != i))
				{
					i = i - 1;
				}
			}
		}
	}
	return burst_control_1;
}

//the mean inter-I/O intervals for the jobs are respectively 
//30ms, 40ms, 45ms, 50ms, 55ms, 60ms, 65ms, 70ms, 75ms
vector<double> cpu_schd::burst_time_initialize()
{
	double number;
	double mean;
	number = num_p;
	vector<double> burst_time_1(number);
	for (int i = 0; i < number; i++)
	{
		mean = 30 + i * 5;
		burst_time_1[i] = possion_random(mean);
	}
	return burst_time_1;
}

//use possion distribution to acculate the possibility of random
//reference from http://www.cplusplus.com/reference/random/poisson_distribution/
double cpu_schd::possion_random(double mean)
{
	// construct a trivial random generator engine from a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::poisson_distribution<int> distribution(mean);
	return distribution(generator);
}

vector<double> cpu_schd::arrival_time_initialize()
{
	double number;
	number = num_p;
	vector<double> arrival_time_1(number);
	for (int i = 0; i < number; i++)
	{
		arrival_time_1[i] = 0;
	}
	return arrival_time_1;
}

vector<double> cpu_schd::priority_initialize()
{
	double number;
	number = num_p;
	vector<double> priority_1(number);
	for (int i = 0; i < number; i++)
	{
		priority_1[i] = 0;
	}
	return priority_1;
}

vector<double> cpu_schd::pid_initialize()
{
	double number;
	number = num_p;
	vector<double> pid_1(number);
	for (int i = 0; i < number; i++)
	{
		pid_1[i] = 0;
	}
	return pid_1;
}

vector<double> cpu_schd::length_initialize()
{
	double number;
	number = num_p;
	vector<double> length_1(number);
	for (int i = 0; i < number; i++)
	{
		length_1[i] = uniform_random_length(2  * 1000, 4 * 1000);
		//length_1[i] = uniform_random_length(2 * 60 * 1000, 4 * 60 * 1000);  //120000-240000  2min to 4 min
	}
	return length_1;
}

//use uniform_int_distribution to acculate the length of jobs
//reference from http://www.cplusplus.com/reference/random/uniform_int_distribution/uniform_int_distribution/
double cpu_schd::uniform_random_length(int start, int end)
{
	// construct a trivial random generator engine from a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> distribution(start, end);

	double number_1 = distribution(generator);
	return number_1;
}

void cpu_schd::swap_process(double a, double b)
{
	double temp;

	//swap the value of process[a] and process[b]
	temp = process[a];
	process[a] = process[b];
	process[b] = temp;

}

//check if there is a value exit in the vector
bool cpu_schd::check_arrival(vector<double> check, int n)
{
	for (int i = 0; i < num_p; i++)
	{
		if (check[i] != n)
		{
			return false;
		}
	}
	return true;
}

cpu_schd::cpu_schd()
{
	num_p = 10;
	process = process_initialize();
	burst_time = burst_time_initialize();
	arrival_time = arrival_time_initialize();
	priority = priority_initialize();
	pid = pid_initialize();
	length = length_initialize();
	length_end = length;  //control the length of job, when length_end == 0, means the job is finishing
	burst_control_state = burst_control();
	io_run = 60;
	idle = 0;
	
}

cpu_schd::cpu_schd(bool judge)
{
	//if judge == true, provide the fixed burst_time, and fixed length of jobs, and fixed burst statement
	if (judge == true)
	{
		num_p = 10;
		process = process_initialize();
		arrival_time = arrival_time_initialize();
		priority = priority_initialize();
		pid = pid_initialize();
		io_run = 60;
		idle = 0;

		//initialize burst time
		double number;
		number = num_p;
		vector<double> burst_time_1(number);
		for (int i = 0; i < number; i++)
		{
			burst_time_1[i] = 30 + i * 5;
		}
		burst_time = burst_time_1;

		//initialize length
		vector<double> length_1(number);
		for (int i = 0; i < number; i++)
		{
			length_1[i] = 2 * 1000 + i * 2 * 100;
			//length_1[i] = 2 * 60 * 1000 + i * 2 * 60 * 100;  //120000-240000  2min to 4 min  fixed the length
		}
		length = length_1;
		length_end = length;

		//initialize burst state
		vector<double> burst_control_1(number);
		burst_control_1[0] = 1;   //put the first job into CPU
		for (int i = 1; i < 6; i++)
		{
			burst_control_1[i] = 2;  //put the 2-6 job into io_queue
		}
		for (int i = 0; i < 4; i++)
		{
			burst_control_1[6 + i] = 3;  //put the 7-10 job into ready_queue
		}
		burst_control_state = burst_control_1;
	}
}

void cpu_schd::output_process()
{
	//find out how many process
	//Create 10 jobs
	
	num_p = 10;
	

	//ouput the table of Process, Arrival Time, and Burst Time
	cout << "Process   Arrival Time   Burst Time   Priority   Length   Burst_Control" << endl;
	for (int i = 0; i < num_p; i++)
	{
		cout << setw(7) << "P" << process[i] + 1 <<
			"   " << setw(11) << arrival_time[process[i]] <<
			"   " << setw(10) << burst_time[process[i]] <<
			"   " << setw(8) << priority[process[i]] <<
			"   " << setw(6) << length[process[i]] <<
			"   " << setw(13) << burst_control_state[process[i]] << endl;
	}

}

vector<double> cpu_schd::get_process()
{
	return process;
}

vector<double> cpu_schd::get_burst_time()
{
	return burst_time;
}

vector<double> cpu_schd::get_arrival_time()
{
	return arrival_time;
}

vector<double> cpu_schd::get_priority()
{
	return priority;
}

vector<double> cpu_schd::get_pid()
{
	return pid;
}

vector<double> cpu_schd::get_length()
{
	return length;
}

vector<double> cpu_schd::get_length_end()
{
	return length_end;
}

vector<double> cpu_schd::get_burst_control_state()
{
	return burst_control_state;
}

double cpu_schd::get_io_run()
{
	return io_run;
}

double cpu_schd::get_num_p()
{
	return num_p;
}