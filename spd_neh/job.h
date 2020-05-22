#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

struct Job {
    int index, number_of_machines, time_sum;
    std::vector<int> job_time;

    Job() : index(0), time_sum(0), number_of_machines(0) {}

    // Constructor to load time's single 
    // job on each machine
    Job(int id, std::ifstream& input, int machines) : index(id), number_of_machines(machines), time_sum(0) {
        int data;
        for (int i = 0; i < number_of_machines; ++i) {
            input >> data;
            job_time.push_back(data);
        }
        setTimeSum();
    }

    // Method to count sum of time 
    // for a single job on each machines
    void setTimeSum() {
        for (auto& time : job_time) {
            time_sum += time;
        }
    }

    // bool operator used to compare two object
    bool operator < (const Job& other_job) const {
        if (time_sum == other_job.time_sum) {
            return index < other_job.index;
        }
        return time_sum > other_job.time_sum;
    }

    friend std::ostream& operator<<(std::ostream& os, const Job& j);
};

std::ostream& operator<<(std::ostream& os, const Job& j) {
    // os << "---------------" << std::endl
    os << j.index << " ";
    os << j.time_sum << " ";
    for (int i = 0; i < j.number_of_machines; ++i) {
        os << j.job_time[i] << " ";
    }
    os << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Job>& j) {
    for (int i = 0; i < j.size(); ++i) {
        os << j[i].index + 1 << " ";
    }
    os << std::endl;
    return os;
}

using Jobs = std::vector<Job>;
using Jobs_vec = std::vector<Jobs>;

// Return cmax for the specified p (defined by task_number and machine_number)
int cmax(std::vector<Job> jobs, const int task_number, const int machine_number) {
    int** array_of_cmax = new int* [task_number + 1];
    for (int i = 0; i < task_number + 1; ++i) {
        array_of_cmax[i] = new int[machine_number + 1];
    }

    for (int i = 0; i < task_number; ++i) {
        array_of_cmax[i][0] = 0;
    }
    for (int j = 0; j < machine_number; ++j) {
        array_of_cmax[0][j] = 0;
    }

    for (int i = 1; i <= task_number; ++i) {
        for (int j = 1; j <= machine_number; ++j) {
            array_of_cmax[i][j] = std::max(array_of_cmax[i - 1][j], array_of_cmax[i][j - 1]) + jobs[i - 1].job_time[j - 1];
        }
    }

    auto result = array_of_cmax[task_number][machine_number];
    for (int i = 0; i < task_number + 1; ++i) {
        delete[] array_of_cmax[i];
    }
    delete[] array_of_cmax;

    return result;
}

// Method to load each structure of the data to test to one huge vector
Jobs_vec load_Jobs(const size_t& count, const std::string& filename) {
    std::ifstream data;
    data.open(filename, std::ios::in);
    if (!data) {
        std::cout << "I can't locate file " << filename
            << " in program's directory" << std::endl;
        exit(-1);
    }
    Jobs_vec jobs_vec(count);
    int current = 0, number_of_tasks, number_of_machines, time;
    std::string tmp;

    do {
        data >> tmp;
        if (tmp.find("data.") != std::string::npos) {
            data >> number_of_tasks >> number_of_machines;
            jobs_vec[current].reserve(number_of_tasks);
            for (int i = 0; i < number_of_tasks; ++i) {
                jobs_vec[current].emplace_back(i, data, number_of_machines);
            }
            current++;
        }
    } while (current != count);

    return jobs_vec;
}

// implementation of NEH algorithm without accelaration
void neh_algorithm(std::vector<Job> &jobs, int task_number, int machine_number) {
    int shortext_cmax, actual_cmax, swap_number, shortext_cmax_number;

    std::sort(jobs.begin(), jobs.end());
    for (int current_job = 0, current_size = 2; current_job < task_number - 1; ++current_job, ++current_size) {
        actual_cmax = cmax(jobs, current_size, machine_number);
        shortext_cmax = actual_cmax;
        shortext_cmax_number = 0;
        swap_number = 0;

        for (int x = current_size - 1; x >= 1; --x) {
            ++swap_number;
            std::swap(jobs[x], jobs[x - 1]);
            actual_cmax = cmax(jobs, current_size, machine_number);

            if (actual_cmax <= shortext_cmax) {
                shortext_cmax = actual_cmax;
                shortext_cmax_number = swap_number;
            }
        }

        for (int i = swap_number, index = 0; i > shortext_cmax_number; --i) {
            std::swap(jobs[index], jobs[index + 1]);
            ++index;
        }
    }
    auto result = cmax(jobs, task_number, machine_number);
    std::cout << "Neh: " << std::endl << result << std::endl;
}

