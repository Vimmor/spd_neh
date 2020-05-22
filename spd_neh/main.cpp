#include <iostream>
#include "job.h"
#include <chrono>
#include <iomanip>

int main() {
	auto jobs_vec = load_Jobs(121, "data.txt");
	auto beginning = std::chrono::steady_clock::now();
	for (auto& jobs : jobs_vec) {
		neh_algorithm(jobs, jobs.size(), jobs[0].number_of_machines);
	}
	auto finish = std::chrono::steady_clock::now();
	std::chrono::duration<double> sum_of_time = finish - beginning;
	std::cout << "Time of the test for all instention: " << sum_of_time.count() << "s" << std::endl;

}