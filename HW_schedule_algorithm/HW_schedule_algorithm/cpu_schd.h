#include <iostream>
#include <vector>
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
	double idle;
	double num_p;  //number of process

public:
	//initialize the process, burst_time, arrival_time, and priority
	vector<double> process_initialize();
	vector<double> burst_time_initialize();
	vector<double> arrival_time_initialize();
	vector<double> priority_initialize();
	vector<double> pid_initialize();
	bool check_arrival(vector<double> check, int n);  //help SJF algorithm check if it can end
	void get_process();
	void get_burst_time();
	void get_arrival_time();
	void get_priority();
	void output_process();
	void swap_process(double a, double b);
	double max_value(vector<double> a);

	//Fist Come First Served scheduling algorithm
	void fcfs();

	//Shortest Job First with nonpreemptive scheduling algorithm
	void sjf();

	//nonpreemptive priority algorithm
	void np_priority();

	//Round Robin scheduling algorithm
	void rr();
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

vector<double> cpu_schd::burst_time_initialize()
{
	double number;
	number = num_p;
	vector<double> burst_time_1(number);
	for (int i = 0; i < number; i++)
	{
		burst_time_1[i] = 0;
	}
	return burst_time_1;
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

void cpu_schd::swap_process(double a, double b)
{
	double temp;

	//swap the value of process[a] and process[b]
	temp = process[a];
	process[a] = process[b];
	process[b] = temp;

}

void cpu_schd::get_burst_time()
{
	process = process_initialize();
	burst_time = burst_time_initialize();

	//Input the Burst Time of each process
	for (int i = 0; i < num_p; i++)
	{
		cout << "Enter the Burst Time of Process P" << i + 1 << " : ";
		cin >> burst_time[i];
	}
}

void cpu_schd::get_arrival_time()
{
	process = process_initialize();
	arrival_time = arrival_time_initialize();

	//Input the Arrival Time of each process
	for (int i = 0; i < num_p; i++)
	{
		cout << "Enter the Arrival Time of Process P" << i + 1 << " : ";
		cin >> arrival_time[i];
	}
}

void cpu_schd::get_priority()
{
	process = process_initialize();
	priority = priority_initialize();

	//Input the priority of each process
	for (int i = 0; i<num_p; i++)
	{
		cout << "Enter the priority of Process P" << i + 1 << " : ";
		cin >> priority[i];
	}
}

void cpu_schd::output_process()
{
	//sort the process according to the arrival time
	for (int i = 0; i< num_p - 1; i++)
	{
		for (int j = i + 1; j< num_p; j++)
		{
			if (arrival_time[process[j]] < arrival_time[process[i]])
				swap_process(process[j], process[i]);
		}
	}

	//ouput the table of Process, Arrival Time, and Burst Time
	cout << "Process   Arrival Time   Burst Time   Priority" << endl;
	for (int i = 0; i < num_p; i++)
	{
		cout << setw(7) << "P" << process[i] + 1 <<
			"   " << setw(11) << arrival_time[process[i]] <<
			"   " << setw(10) << burst_time[process[i]] <<
			"   " << setw(8) << priority[process[i]] << endl;
	}
}


void cpu_schd::get_process()
{
	//find out how many process
	cout << "Enter the number of the process: ";
	cin >> num_p;
	process = process_initialize();
	burst_time = burst_time_initialize();
	arrival_time = arrival_time_initialize();
	priority = priority_initialize();
	pid = pid_initialize();

	//Input the Burst Time and Arrival Time of each process
	for (int i = 0; i < num_p; i++)
	{
		cout << "Enter the Arrival Time of Process P" << i + 1 << " : ";
		cin >> arrival_time[i];
		cout << "Enter the Burst Time of Process P" << i + 1 << " : ";
		cin >> burst_time[i];
		cout << "Enter the priority of Process P" << i + 1 << " : ";
		cin >> priority[i];
	}

	//sort the process according to the arrival time
	for (int i = 0; i< num_p - 1; i++)
	{
		for (int j = i + 1; j< num_p; j++)
		{
			if (arrival_time[process[j]] < arrival_time[process[i]])
				swap_process(process[j], process[i]);
		}
	}


	//ouput the table of Process, Arrival Time, and Burst Time
	cout << "Process   Arrival Time   Burst Time   Priority" << endl;
	for (int i = 0; i < num_p; i++)
	{
		cout << setfill(' ') << setw(7) << "P" << process[i] + 1 <<
			"   " << setw(11) << arrival_time[process[i]] <<
			"   " << setw(10) << burst_time[process[i]] <<
			"   " << setw(8) << priority[process[i]] << endl;
	}
	
}


//First come First served scheduling algorithm
void cpu_schd::fcfs()
{
	

	//the first come first served
	pid = process;

	vector<double> turnaround_time(num_p);
	vector<double> waiting_time(num_p);
	vector<double> ready_process(num_p);
	double average_turnaround_time;
	double average_waiting_time;
	double temp_burst;
	double end_time; // control the end of program
	int k;
	double pick;
	double idle_1;
	temp_burst = 0;
	average_turnaround_time = 0;
	average_waiting_time = 0;
	end_time = 0;

	cout << "Start using Fist Come First Served scheduling algorithm." << endl;
	cout << "If CPU is left idle, please input it(if not, please enter 0): ";
	cin >> idle;
	idle_1 = idle;

	//find the largest burst time 
	pick = burst_time[0];
	for (int h = 0; h < num_p; h++)
	{
		if (burst_time[h] > pick)
			pick = burst_time[h];
		end_time = 2 * end_time + burst_time[h];
	}

	//initialize ready queue
	for (int g = 0; g< num_p; g++)
	{
		ready_process[g] = pick + 1;
	}

	//avoid changing the value of arrival time
	vector<double> arrival_time_1;
	arrival_time_1 = arrival_time;

	double whole_time;//store the system time of fcfs scheduling
	whole_time = 0;

	//star FCFS algorithm
	//output Gantt chart
	cout << "0 |";
	for (bool check_1 = false; check_1 != true; )
	{

		k = 0;
		for (int i = 0; i < num_p; i++)
		{
			if (idle == 0)
			{
				if (arrival_time_1[pid[i]] <= idle)
				{
					ready_process[k] = pid[i];
					k = k + 1;
				}
			}
			else if(idle != 0)
			{
				if (arrival_time_1[pid[i]] < idle)
				{
					ready_process[k] = pid[i];
					k = k + 1;
				}
			}
			
		}
		//the first process into the cpu
		pick = ready_process[0];

		waiting_time[pick] = idle_1 - arrival_time_1[pick] + temp_burst;
		turnaround_time[pick] = waiting_time[pick] + burst_time[pick];
		temp_burst = temp_burst + burst_time[pick];

		whole_time = whole_time + burst_time[pick];
		//output Gantt chart
		cout << setw(3) << setfill(' ') << "P" << pick + 1 <<
			setw(burst_time[pick] + 3) << setfill('_') << "| " << whole_time;
		arrival_time_1[pick] = end_time + 1;
		idle = idle + burst_time[pick];

		check_1 = check_arrival(arrival_time_1, end_time + 1);
	}
	cout << endl;

	//output the waiting time and turnaround time of each process
	for (int g = 0; g < num_p; g++)
	{
		cout << "The waiting time of Process P" << g + 1 << " is: " << waiting_time[g] << endl;
		cout << "The turnaround time of Process P" << g + 1 << " is: " << turnaround_time[g] << endl;
		average_waiting_time = average_waiting_time + waiting_time[g];
		average_turnaround_time = average_turnaround_time + turnaround_time[g];
	}

	average_waiting_time = average_waiting_time / num_p;
	average_turnaround_time = average_turnaround_time / num_p;

	cout << "The average waiting time is: " << average_waiting_time << endl;
	cout << "The average turnaround time is: " << average_turnaround_time << endl;
	cout << endl;
}


//Shortest Job First with nonpreemptive scheduling algorithm
void cpu_schd::sjf()
{

	pid = process;

	vector<double> turnaround_time(num_p);
	vector<double> waiting_time(num_p);
	vector<double> ready_process(num_p);
	double average_turnaround_time;
	double average_waiting_time;
	double temp_burst;
	double end_time; // control the end of program
	int k;
	double pick;
	double idle_1;
	temp_burst = 0;
	average_turnaround_time = 0;
	average_waiting_time = 0;
	end_time = 0;

	cout << "Start using Shortest Job First scheduling algorithm." << endl;
	cout << "If CPU is left idle, please input it(if not, please enter 0): ";
	cin >> idle;
	idle_1 = idle;

	//find the largest burst time 
	pick = burst_time[0];
	for (int h = 0; h < num_p; h++)
	{
		if (burst_time[h] > pick)
			pick = burst_time[h];
		end_time = 2 * end_time + burst_time[h];
	}

	//initialize ready queue
	for (int g = 0; g< num_p; g++)
	{
		ready_process[g] = pick + 1;
	}

	double whole_time;//store the system time of rr scheduling
	whole_time = 0;

	//start SJF algorithm
	//avoid changing the value of arrival time
	vector<double> arrival_time_1;
	arrival_time_1 = arrival_time;
	//output Gantt chart
	cout << "0 |";
	for (bool check_1 = false; check_1 != true; )
	{
		//search the arrival time, and put them into ready queue
		k = 0;
		for (int i = 0; i < num_p; i++)
		{
			if (idle == 0)
			{
				if (arrival_time_1[pid[i]] <= idle)
				{
					ready_process[k] = pid[i];
					k = k + 1;
				}
			}
			else
			{
				if (arrival_time_1[pid[i]] < idle)
				{
					ready_process[k] = pid[i];
					k = k + 1;
				}
			}
		}
		//find shortest burst time between these processes
		pick = ready_process[0];
		for (int q = 0; q < k; q++)
		{
			if (burst_time[ready_process[q]] < burst_time[pick])
			{
				pick = ready_process[q];
			}
		}
		waiting_time[pick] = idle_1 - arrival_time_1[pick] + temp_burst;
		turnaround_time[pick] = waiting_time[pick] + burst_time[pick];
		temp_burst = temp_burst + burst_time[pick];

		whole_time = whole_time + burst_time[pick];
		//output Gantt chart
		cout << setw(3) << setfill(' ') << "P" << pick + 1 <<
			setw(burst_time[pick] + 3) << setfill('_') << "| " << whole_time;
		arrival_time_1[pick] = end_time + 1;
		idle = idle + burst_time[pick];

		check_1 = check_arrival(arrival_time_1, end_time + 1);
	}
	cout << endl;

	//output the waiting time and turnaround time of each process
	for (int g = 0; g < num_p; g++)
	{
		cout << "The waiting time of Process P" << g + 1 << " is: " << waiting_time[g] << endl;
		cout << "The turnaround time of Process P" << g + 1 << " is: " << turnaround_time[g] << endl;
		average_waiting_time = average_waiting_time + waiting_time[g];
		average_turnaround_time = average_turnaround_time + turnaround_time[g];
	}

	average_waiting_time = average_waiting_time / num_p;
	average_turnaround_time = average_turnaround_time / num_p;

	cout << "The average waiting time is: " << average_waiting_time << endl;
	cout << "The average turnaround time is: " << average_turnaround_time << endl;
	cout << endl;
}

//help SJF check if it can end
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


//nonpreemptive priority algorithm
void cpu_schd::np_priority()
{
	
	pid = process;

	vector<double> turnaround_time(num_p);
	vector<double> waiting_time(num_p);
	vector<double> ready_process(num_p);
	double average_turnaround_time;
	double average_waiting_time;
	double temp_burst;
	double end_time;  //control the end of the program
	int k;
	double pick;
	double idle_1;
	temp_burst = 0;
	average_turnaround_time = 0;
	average_waiting_time = 0;
	end_time = 0;

	cout << "Start using Nonpreemptive Priority scheduling algorithm." << endl;
	cout << "If CPU is left idle, please input it(if not, please enter 0): ";
	cin >> idle;
	idle_1 = idle;

	//find the largest burst time
	pick = burst_time[0];
	for (int h = 0; h < num_p; h++)
	{
		if (burst_time[h] > pick)
			pick = burst_time[h];
		end_time = 2 * end_time + burst_time[h];
	}

	//initialize ready queue
	for (int g = 0; g< num_p; g++)
	{
		ready_process[g] = pick + 1;
	}

	double whole_time;//store the system time of rr scheduling
	whole_time = 0;

	//start Nonpreemptive Priority algorithm
	//avoid changing the value of arrival time
	vector<double> arrival_time_1;
	arrival_time_1 = arrival_time;
	//output Gantt chart
	cout << "0 |";
	for (bool check_1 = false; check_1 != true; )
	{
		//search the arrival time, and put them into ready queue
		k = 0;
		for (int i = 0; i < num_p; i++)
		{
			if (idle == 0)
			{
				if (arrival_time_1[pid[i]] <= idle)
				{
					ready_process[k] = pid[i];
					k = k + 1;
				}
			}
			else
			{
				if (arrival_time_1[pid[i]] < idle)
				{
					ready_process[k] = pid[i];
					k = k + 1;
				}
			}
		}
		//find lowest priority between these processes
		pick = ready_process[0];
		for (int q = 0; q < k; q++)
		{
			if (priority[ready_process[q]] < priority[pick])
			{
				pick = ready_process[q];
			}
		}
		waiting_time[pick] = idle_1 - arrival_time_1[pick] + temp_burst;
		turnaround_time[pick] = waiting_time[pick] + burst_time[pick];
		temp_burst = temp_burst + burst_time[pick];

		whole_time = whole_time + burst_time[pick];
		//output Gantt chart
		cout << setw(3) << setfill(' ') << "P" << pick + 1 <<
			setw(burst_time[pick] + 3) << setfill('_') << "| " << whole_time;
		arrival_time_1[pick] = end_time + 1;
		idle = idle + burst_time[pick];

		check_1 = check_arrival(arrival_time_1, end_time + 1);
	}
	cout << endl;

	//output the waiting time and turnaround time of each process
	for (int g = 0; g < num_p; g++)
	{
		cout << "The waiting time of Process P" << g + 1 << " is: " << waiting_time[g] << endl;
		cout << "The turnaround time of Process P" << g + 1 << " is: " << turnaround_time[g] << endl;
		average_waiting_time = average_waiting_time + waiting_time[g];
		average_turnaround_time = average_turnaround_time + turnaround_time[g];
	}


	average_waiting_time = average_waiting_time / num_p;
	average_turnaround_time = average_turnaround_time / num_p;

	cout << "The average waiting time is: " << average_waiting_time << endl;
	cout << "The average turnaround time is: " << average_turnaround_time << endl;
	cout << endl;
}


//Round-Robin scheduling algorithm
void cpu_schd::rr()
{
	

	pid = process;

	vector<double> turnaround_time(num_p);
	vector<double> waiting_time(num_p);
	vector<double> ready_process(num_p);
	double average_turnaround_time;
	double average_waiting_time;
	double temp_burst;
	double end_time;  //control the end of the program
	int k;
	double pick;
	double idle_1;
	temp_burst = 0;
	average_turnaround_time = 0;
	average_waiting_time = 0;
	end_time = 0;

	//input the quantum
	double quantum;
	cout << "Please input the quantum of Round-Robin scheduing: ";
	cin >> quantum;

	cout << "Start using Round-Robin scheduling algorithm." << endl;
	cout << "If CPU is left idle, please input it(if not, please enter 0): ";
	cin >> idle;
	idle_1 = idle;

	//find the largest burst time
	pick = burst_time[0];
	for (int h = 0; h < num_p; h++)
	{
		if (burst_time[h] > pick)
			pick = burst_time[h];
		end_time = 2 * end_time + burst_time[h];
	}

	//initialize ready queue
	for (int g = 0; g< num_p; g++)
	{
		ready_process[g] = pick + 1;
	}


	//avoid changing the value of arrival time
	vector<double> arrival_time_1;
	arrival_time_1 = arrival_time;
	//avoid changing the value of burst time
	vector<double> burst_time_1;
	burst_time_1 = burst_time;

	//given each process one end_time for documenting the last finish time
	vector<double> process_end(num_p);
	//initialize end time
	for (int w = 0; w< num_p; w++)
	{
		process_end[w] = 0;
	}

	//for detecting which process goes first
	vector<double> priority_decide(num_p);
	priority_decide = priority;
	//initialize priority_decide queue
	for (int v = 0; v< num_p; v++)
	{
		priority_decide[v] = 0;
	}

	//start Round-Robin scheduling algorithm
	double max_priority;//store the max priority decide
	max_priority = 0;
	double whole_time;//store the system time of rr scheduling
	whole_time = 0;
	//output Gantt chart
	cout << "0 |";


	for (bool check_1 = false; check_1 != true; )
	{

		//search the arrival time, and put them into ready queue
		//through giving the process priority_decide to sort the ready queue

		k = 0;
		max_priority = max_value(priority_decide);
		for (int d = 0; d <= max_priority; d++)
		{
			for (int i = 0; i < num_p; i++)
			{
				if (idle == 0)
				{
					if ((arrival_time_1[pid[i]] <= idle) && (priority_decide[pid[i]] == d))
					{
						ready_process[k] = pid[i];
						k = k + 1;
					}
				}
				else
				{
					if ((arrival_time_1[pid[i]] < idle) && (priority_decide[pid[i]] == d))
					{
						ready_process[k] = pid[i];
						k = k + 1;
					}
				}
			}
		}

			//choose the first process of ready queue running
			pick = ready_process[0];

			if (burst_time_1[pick] > quantum)
			{
				if (priority_decide[pick] == 0)
				{
					waiting_time[pick] = idle_1 - arrival_time_1[pick] + temp_burst;
					temp_burst = temp_burst + quantum;

					whole_time = whole_time + quantum;
					process_end[pick] = whole_time;
					burst_time_1[pick] = burst_time_1[pick] - quantum;
					priority_decide[pick] = priority_decide[pick] + 1;

					idle = idle + quantum;

					//output Gantt chart
					cout << setw(3) << setfill(' ') << "P" << pick + 1 <<
						setw(quantum + 3) << setfill('_') << "| " << whole_time;

				}
				else
				{
					waiting_time[pick] = waiting_time[pick] + whole_time - process_end[pick];
					temp_burst = temp_burst + quantum;

					whole_time = whole_time + quantum;
					process_end[pick] = whole_time;;
					burst_time_1[pick] = burst_time_1[pick] - quantum;
					priority_decide[pick] = priority_decide[pick] + 1;

					idle = idle + quantum;

					//output Gantt chart
					cout << setw(3) << setfill(' ') << "P" << pick + 1 <<
						setw(quantum + 3) << setfill('_') << "| " << whole_time;
				}
			}
			else if (burst_time_1[pick] <= quantum)
			{
				if (priority_decide[pick] == 0)
				{
					waiting_time[pick] = idle_1 - arrival_time_1[pick] + temp_burst;
					temp_burst = temp_burst + burst_time_1[pick];

					whole_time = whole_time + burst_time_1[pick];
					process_end[pick] = whole_time;;
					burst_time_1[pick] = 0;
					priority_decide[pick] = priority_decide[pick] + 1;

					arrival_time_1[pick] = end_time + 1;
					idle = idle + burst_time_1[pick];

					//output Gantt chart
					cout << setw(3) << setfill(' ') << "P" << pick + 1 <<
						setw(burst_time_1[pick] + 3) << setfill('_') << "| " << whole_time;
				}
				else
				{
					waiting_time[pick] = waiting_time[pick] + whole_time - process_end[pick];
					temp_burst = temp_burst + burst_time_1[pick];

					whole_time = whole_time + burst_time_1[pick];
					process_end[pick] = whole_time;;
					burst_time_1[pick] = 0;
					priority_decide[pick] = priority_decide[pick] + 1;

					arrival_time_1[pick] = end_time + 1;
					idle = idle + burst_time_1[pick];

					//output Gantt chart
					cout << setw(3) << setfill(' ') << "P" << pick + 1 <<
						setw(burst_time_1[pick] + 3) << setfill('_') << "| " << whole_time;
				}
			}


			check_1 = check_arrival(burst_time_1, 0);
		}
		cout << endl;

	//output the waiting time and turnaround time of each process
	for (int g = 0; g < num_p; g++)
	{
		turnaround_time[g] = process_end[g] - arrival_time[g];
		cout << "The waiting time of Process P" << g + 1 << " is: " << waiting_time[g] << endl;
		cout << "The turnaround time of Process P" << g + 1 << " is: " << turnaround_time[g] << endl;
		average_waiting_time = average_waiting_time + waiting_time[g];
		average_turnaround_time = average_turnaround_time + turnaround_time[g];
	}

	average_waiting_time = average_waiting_time / num_p;
	average_turnaround_time = average_turnaround_time / num_p;

	cout << "The average waiting time is: " << average_waiting_time << endl;
	cout << "The average turnaround time is: " << average_turnaround_time << endl;
	cout << endl;
}


double cpu_schd::max_value(vector<double> a)
{
	double max;
	max = a[0];
	for (int i = 0; i < num_p; i++)
	{
		if (a[i] > max)
		{
			max = a[i];
		}
	}
	return max;
}
