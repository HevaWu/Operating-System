/*  *  *  *  *  *  *  *  *  *  *
 *  Course        :CS-520-A    *
 *  Student Name  :He Wu       *
 *  CWID          :10406347    *
 *         Homework 2          *
 *  *  *  *  *  *  *  *  *  *  */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

/*
* author: http://p9as.blogspot.com/2012/06/c11-semaphores.html
* modified by: bus_semaphore
*/
namespace bus_semaphore {
	class semaphore {
	public:
		semaphore(int value = 1) : count{ value }, wakeups{ 0 } {}

		void wait() {
			std::unique_lock<std::mutex> lock{ mutex };
			if (--count<0) { // the process is empty, can be entered
				condition.wait(lock, [&]()->bool { return wakeups>0; }); // suspend and wait ...
				--wakeups;  // enter the process
			}
		}
		void signal() {
			std::lock_guard<std::mutex> lock{ mutex };
			if (++count <= 0) { // the process is full, there is a process in it, another should wait for it
				++wakeups;
				condition.notify_one(); // notify on it
			}
		}

	private:
		int count;
		int wakeups;
		std::mutex mutex;
		std::condition_variable condition;
	};
};


//set the mutex signal "mtx"
std::mutex mtx;

bus_semaphore::semaphore busrun(0);

void runbus()
{
	busrun.wait();
	std::lock_guard<std::mutex> lock{ mtx };
	//std::cout << "thread bus" << '\n';
	busrun.signal();
}

//declarition of function now_clock
//this function is used to print the time into **hours **minutes **seconds
void now_clock(double clock);

class busstop
{
public:
	int person_amount_queue(double  person_inter_clock, double clock);  //return person amount
	int possion_random(double mean, double clock);  //return random
	double start_time;   //person start to inter the bus stop
	int person_amount;   //calculate the person amount of this stop
	int wait_queue;      //for calculate the sum of the waiting queue
	int maximum_queue;   //store the maximum queue of this stop
	int minimum_queue;   //store the minimum queue of this stop
	int stop_count;      //counting the number of bus which coming to this stop
	int average_queue;   //average size of the waiting queue of this stop
};

class bus
{
public:
	double begin_time;        //the time bus start running
	double boarding_time;     //person inter the bus
	double before_begin_time; //the time bus finish the last stop and prepare to inter the next stop
	int stop_number;          //the stop where the bus is 

};

int main()
{
	double clock, stop_time, clock_last, clock_judge, boarding_time, clock_snapshot;
	int earliest_bus, stop_number, smallest_begin_time_bus;
	bus run_bus[5];       //there are 5 buses
	busstop bus_stop[15]; //there are 15 bus stops
	clock = 0;            //the system clock in minutes
	clock_last = 0;       //document the last circle clock, used to control the periodic print
	clock_judge = 0;      //judgement whether it is time to print the maximum/minimum/average waiting queue
	clock_snapshot = 0;   //judgement whether it is time to print the address of each bus
	stop_time = 8 * 60;   //control the simulation stop
	boarding_time = 0.033;//each person has 2 seconds boarding time, here we changed it into minutes

	//initialize run_bus and bus_stop
	for (int i = 0; i <= 4; i++)
	{
		//clock in minutes
		run_bus[i].begin_time = 0;
		run_bus[i].boarding_time = 0;
		run_bus[i].before_begin_time = run_bus[i].begin_time;
		run_bus[i].stop_number = i * 3;
	}
	for (int j = 0; j <= 14; j++)
	{
		bus_stop[j].person_amount = 0;
		bus_stop[j].start_time = 0;
		bus_stop[j].wait_queue = 0;
		bus_stop[j].maximum_queue = 0;
		bus_stop[j].minimum_queue = 10000;
		bus_stop[j].stop_count = 0;
		bus_stop[j].average_queue = 0;
	}

	//clock in minutes
	do
	{
		//the less number of bus begin running
		//judge which bus should run first
		//control the later number of the bus cannot pass the former number bus
		earliest_bus = 0;
		for (int i = 0; i <= 4; i++)
		{
			if (run_bus[earliest_bus].begin_time > run_bus[i].begin_time)
				earliest_bus = i;
		}
		//std::cout << "The earliest running bus is: " << earliest_bus + 1 << endl;
		//std::thread(busrun);

		//lock the mutex
		//one bus begin running, according to former judgement
		mtx.lock();

		//record where is this first running bus
		stop_number = run_bus[earliest_bus].stop_number;
		//std::cout << "The stop number is: " << stop_number + 1 << endl;

		//record how many people are waiting in this stop at this circle
		bus_stop[stop_number].person_amount = bus_stop[stop_number].person_amount_queue(bus_stop[stop_number].start_time, run_bus[earliest_bus].begin_time);
		//std::cout << "The person amount is: " << bus_stop[stop_number].person_amount << endl;

		//record the total waiting people of this stop from begin to now
		//this wait queue will be set to 0 until it periodical print out the maximum/minimum/average waiting queue
		bus_stop[stop_number].wait_queue = bus_stop[stop_number].wait_queue + bus_stop[stop_number].person_amount;
		//std::cout << "The waiting queue of this stop is: " << bus_stop[stop_number].wait_queue << endl;
		
		//compare the former waiting queue, and record the maximum/minimum/average waiting queue of this stop from begin to now
		//this record will be set to initialization until it periodical print out the maximum/minimum/average waiting queue
		if (bus_stop[stop_number].person_amount < bus_stop[stop_number].minimum_queue)
			bus_stop[stop_number].minimum_queue = bus_stop[stop_number].person_amount;
		if (bus_stop[stop_number].person_amount > bus_stop[stop_number].maximum_queue)
			bus_stop[stop_number].maximum_queue = bus_stop[stop_number].person_amount;

		//record how many bus have been to this stop
		//this count used to calculate the average waiting queue of this stop
		bus_stop[stop_number].stop_count = bus_stop[stop_number].stop_count + 1;

		//record how long it will take in boarding time at this circle
		run_bus[earliest_bus].boarding_time = boarding_time * bus_stop[stop_number].person_amount;

		//after this bus leaving, the waiting queue of this stop should calculate again
		//this time used to control when the people start enter the waiting queue
		bus_stop[stop_number].start_time = run_bus[earliest_bus].begin_time + run_bus[earliest_bus].boarding_time;
		//std::cout << "The start time of this stop change into: " << bus_stop[stop_number].start_time << endl;

		//before_begin_time is the bus finished this stop and perpare to go to the next stop
		/*we set the begin_time as the time the bus begin running, after this circle finished, it has to go to the next stop,
		and the running time of this bus has changed*/
		//these two time also used to judge the position of the bus
		run_bus[earliest_bus].before_begin_time = run_bus[earliest_bus].begin_time + run_bus[earliest_bus].boarding_time;
		run_bus[earliest_bus].begin_time = run_bus[earliest_bus].begin_time + run_bus[earliest_bus].boarding_time + 5;
		//std::cout << "The begin time of this bus change into: " << run_bus[earliest_bus].begin_time << endl;
		//std::cout << "Before enter the next stop, the begin time of this bus is: " << run_bus[earliest_bus].before_begin_time << endl;

		//judge the circle of the buses
		//if this bus is at the last stop, stop[14], it should go to the first stop, stop[0]
		if (run_bus[earliest_bus].stop_number < 14)
		{
			run_bus[earliest_bus].stop_number = run_bus[earliest_bus].stop_number + 1;
		}
		else if (run_bus[earliest_bus].stop_number == 14)
		{
			run_bus[earliest_bus].stop_number = 0;
			//std::cout << "The next stop of this bus is: " << run_bus[earliest_bus].stop_number + 1 << endl;
		}
		//std::cout << "The next stop of this bus is: " << run_bus[earliest_bus].stop_number + 1 << endl;

		//control the area of the clock
		//we used the smallest begin time of these five buses as the clock
		//for this step, we can make sure the running of each bus
		smallest_begin_time_bus = 0;
		clock_last = clock;
		for (int i = 0; i <= 4; i++)
		{
			if (run_bus[smallest_begin_time_bus].begin_time > run_bus[i].begin_time)
			{
				clock = run_bus[i].begin_time;
			}
		}
		//std::cout << "The clock is: " << clock << endl;
		//std::cout << "The last clock is: " << clock_last << endl;

		//print the maximum/minimum/average of the waiting queue
		clock_judge = static_cast<int>(clock) / 60 - static_cast<int>(clock_last) / 60;
		if (clock_judge == 1)
		{
			std::cout << "When clock is:";
			now_clock(clock);
			//print the maximum/minimum/average of the waiting queue
			for (int j = 0; j <= 14; j++)
			{
				//whether there are buses enter this stop
				//if there are buses enter this stop at this hour, we can calculate the average waiting queueof this hour.
				/*if there is no bus enter this stop at this hour, the waiting queue is keep increasing,
				the maximum/minimum/average waiting queue should calculate again*/
				if (bus_stop[j].stop_count > 0)
					bus_stop[j].average_queue = bus_stop[j].wait_queue / bus_stop[j].stop_count;
				else if (bus_stop[j].stop_count == 0)
					bus_stop[j].average_queue = 0;
				if (bus_stop[j].wait_queue == 0)
				{
					bus_stop[j].person_amount = bus_stop[j].person_amount_queue(bus_stop[j].start_time, clock);
					bus_stop[j].wait_queue = bus_stop[j].person_amount;
					bus_stop[j].minimum_queue = bus_stop[j].person_amount;
					bus_stop[j].maximum_queue = bus_stop[j].person_amount;
					bus_stop[j].stop_count = 1;
					bus_stop[j].average_queue = bus_stop[j].wait_queue / bus_stop[j].stop_count;
				}
				//print out the maximum/minimum/average waiting queue of this stop
				//std::cout << "The waiting queue of stop " << j + 1 << " is: " << bus_stop[j].wait_queue << endl;
				std::cout << "The maximum waiting queue of stop " << j + 1 << " is: " << bus_stop[j].maximum_queue << endl;
				std::cout << "The minimum waiting queue of stop " << j + 1 << " is: " << bus_stop[j].minimum_queue << endl;
				std::cout << "The average waiting queue of stop " << j + 1 << " is: " << bus_stop[j].average_queue << endl;
				std::cout << endl;

				//if you want to print out the maximum/minimum/average queue of this stop in every hour, you can use following code
				//if you want to calculate the waiting queue of the whole simulation, you can ignore following code
				//after finishing print out the waiting queue of this stop, some parameter should set into the initialization
				//it is prepare for the next print out of the waiting queue of this stop
				//bus_stop[j].wait_queue = 0;
				//bus_stop[j].maximum_queue = 0;
				//bus_stop[j].minimum_queue = 10000;
				//bus_stop[j].stop_count = 0;
				//bus_stop[j].average_queue = 0;
			}
		}

		//periodic snapshots of the postions of buses
		//freetime means how frequently you want to show the postion
		int freetime;
		freetime = 240;
		clock_snapshot = static_cast<int>(clock) / freetime - static_cast<int>(clock_last) / freetime;
		if (clock_snapshot == 1)
		{
			std::cout << "Plot the positions of buses in " << freetime << " minutes." << endl;
			std::cout << "When clock is:";
			//this function is to change clock into **hours **minutes **seconds
			now_clock(clock);
			for (int i = 0; i <= 4; i++)
			{
				//before_begin_time is the bus finished this stop and perpare to go to the next stop
				/*we set the begin_time as the time the bus begin running, after this circle finished, it has to go to the next stop,
				and the running time of this bus has changed*/
				//these two time also used to judge the position of the bus
				if ((clock >= run_bus[i].before_begin_time) && (clock < run_bus[i].begin_time))
				{
					std::cout << "Bus " << i + 1 << " is on the road, which is between stop " << run_bus[i].stop_number << " and stop " << run_bus[i].stop_number + 1 << endl;
				}
				else
					std::cout << "Bus " << i + 1 << " is at the stop  " << run_bus[i].stop_number + 1 << endl;
			}
		}
		//busrun.join();

		//release the mutex lock
		mtx.unlock();
	} while (clock <= stop_time/*80*/);

	return 0;
}

//turn clock into hours/minutes/seconds
void now_clock(double clock)
{
	int second, minute, hour;

	//clock is in minutes
	second = (clock - static_cast<int>(clock)) * 60;
	hour = static_cast<int>(clock) / 60;
	minute = clock - hour * 60;
	std::cout << hour << " hours " << minute << " minutes " << second << " seconds" << endl;
}

//calculate the person number of each bus stop
int busstop::person_amount_queue(double person_inter_clock, double clock)
{
	int person_amount, random;
	double mean_inter_arrival_rate, mean_arrival_rate;
	mean_arrival_rate = 5;  //the passenger's mean arrival rate at each stop is 5 persons/min
	mean_inter_arrival_rate = 1 / mean_arrival_rate;
	//std::cout << mean_inter_arrival_rate << endl;
	person_amount = 0;
	//std::cout << "person inter "<<person_inter_clock << " clock " << clock << endl;
	do
	{
		random = possion_random(2/*mean_arrival_rate*//*5*//*mean_inter_arrival_rate*/, person_inter_clock);
		//std::cout << "random" << random;
		person_inter_clock = person_inter_clock + mean_inter_arrival_rate * random;
		//person_inter_clock = person_inter_clock + mean_arrival_rate * random;
		person_amount = person_amount + 1;
		//std::cout << "person amount"<<person_amount << endl;
		//std::cout << "person inter clock"<<person_inter_clock;
	} while (person_inter_clock < clock);
	//std::cout << "person amount"<<person_amount;
	return person_amount;
}

//use possion distribution to acculate the possibility of random
//reference from http://www.cplusplus.com/reference/random/poisson_distribution/
int busstop::possion_random(double mean, double clock)
{
	// construct a trivial random generator engine from a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::poisson_distribution<int> distribution(mean);
	return distribution(generator);
}