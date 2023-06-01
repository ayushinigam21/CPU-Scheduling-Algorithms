#include <bits/stdc++.h>
#include <numeric>
#include <math.h>
using namespace std;
#define MAX_PROCESS 10

int num_of_process = 3, count, remain, time_quantum;
int execution_time[MAX_PROCESS], period[MAX_PROCESS], remain_time[MAX_PROCESS], deadline[MAX_PROCESS], remain_deadline[MAX_PROCESS];
int burst_time[MAX_PROCESS], wait_time[MAX_PROCESS], completion_time[MAX_PROCESS], arrival_time[MAX_PROCESS];

struct node
{
    char pname[50];
    int btime;
    int atime;
} a[50];
void insert(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        cin >> a[i].pname;
        cin >> a[i].atime;
        cin >> a[i].btime;
    }
}
bool btimeSort(node a, node b)
{
    return a.btime < b.btime;
}
bool atimeSort(node a, node b)
{
    return a.atime < b.atime;
}
void disp(int n)
{
    sort(a, a + n, btimeSort);
    sort(a, a + n, atimeSort);
    int ttime = 0, i;
    int j, tArray[n];
    for (i = 0; i < n; i++)
    {
        j = i;
        while (a[j].atime <= ttime && j != n)
        {
            j++;
        }
        sort(a + i, a + j, btimeSort);
        tArray[i] = ttime;
        ttime += a[i].btime;
    }
    tArray[i] = ttime;
    float averageWaitingTime = 0;
    float averageResponseTime = 0;
    float averageTAT = 0;
    cout << "\n";
    cout << "P.Name  AT\tBT\tCT\tTAT\tWT\tRT\n";
    for (i = 0; i < n; i++)
    {
        cout << a[i].pname << "\t";
        cout << a[i].atime << "\t";
        cout << a[i].btime << "\t";
        cout << tArray[i + 1] << "\t";
        cout << tArray[i] - a[i].atime + a[i].btime << "\t";
        averageTAT += tArray[i] - a[i].atime + a[i].btime;
        cout << tArray[i] - a[i].atime << "\t";
        averageWaitingTime += tArray[i] - a[i].atime;
        cout << tArray[i] - a[i].atime << "\t";
        averageResponseTime += tArray[i] - a[i].atime;
        cout << "\n";
    }
    cout << "\n";
    cout << "\nGantt Chart\n";
    for (i = 0; i < n; i++)
    {
        cout << "|   " << a[i].pname << "   ";
    }
    cout << "\n";
    for (i = 0; i < n + 1; i++)
    {
        cout << tArray[i] << "\t";
    }
    cout << "\n";
    cout << "Average Response time: " << (float)averageResponseTime / (float)n << endl;
    cout << "Average Waiting time: " << (float)averageWaitingTime / (float)n << endl;
    cout << "Average TA time: " << (float)averageTAT / (float)n << endl;
}

// collecting details of processes
void get_process_info(int selected_algo)
{
    cout << "Enter total number of processes(maximum " << MAX_PROCESS << "): ";
    cin >> num_of_process;
    if (num_of_process < 1)
    {
        cout << "Do you really want to schedule " << num_of_process << " processes? -_-" << endl;
        exit(0);
    }
    if (selected_algo == 2)
    {
        cout << endl
             << "Enter Time Quantum: ";
        cin >> time_quantum;
        if (time_quantum < 1)
        {
            cout << "Invalid Input: Time quantum should be greater than 0" << endl;
            exit(0);
        }
    }

    for (int i = 0; i < num_of_process; i++)
    {
        cout << endl
             << "Process " << i + 1 << ":-" << endl;
        if (selected_algo == 1)
        {
            cout << "==> Burst time: ";
            cin >> burst_time[i];
        }
        else if (selected_algo == 2)
        {
            cout << "=> Arrival Time: ";
            cin >> arrival_time[i];
            cout << "=> Burst Time: ";
            cin >> burst_time[i];
            remain_time[i] = burst_time[i];
        }
        else if (selected_algo > 2)
        {
            cout << "==> Execution time: ";
            cin >> execution_time[i];
            remain_time[i] = execution_time[i];
            if (selected_algo == 4)
            {
                cout << "==> Deadline: ";
                cin >> deadline[i];
            }
            else
            {
                cout << "==> Period: ";
                cin >> period[i];
            }
        }
    }
}

// get maximum of three numbers
int max(int a, int b, int c)
{
    long max, lcom, count, flag = 0;
    if (a >= b && a >= c)
        return max = a;
    else if (b >= a && b >= c)
        return max = b;
    else if (c >= a && c >= b)
        return max = c;
}

// calculating the observation time for scheduling timeline
int get_observation_time(int selected_algo)
{
    if (selected_algo < 3)
    {
        int sum = 0;
        for (int i = 0; i < num_of_process; i++)
        {
            sum += burst_time[i];
        }
        return sum;
    }

    else if (selected_algo == 3)
    {
        return max(period[0], period[1], period[2]);
    }

    else if (selected_algo == 4)
    {
        return max(deadline[0], deadline[1], deadline[2]);
    }
}

// print scheduling sequence
void print_schedule(int process_list[], int cycles)
{
    cout << endl
         << "Scheduling:-" << endl
         << endl;
    cout << "Time: ";
    for (int i = 0; i < cycles; i++)
    {
        if (i < 10)
            cout << "| 0" << i << " ";
        else
            cout << "| " << i << " ";
    }
    cout << "|" << endl;

    for (int i = 0; i < num_of_process; i++)
    {
        cout << "P[" << i + 1 << "]: ";
        for (int j = 0; j < cycles; j++)
        {
            if (process_list[j] == i + 1)
                cout << "|####";
            else
                cout << "|    ";
        }
        cout << "|" << endl;
    }
}

void first_come_first_serve(int time)
{
    int process_list[time];
    int execution_time[num_of_process];

    int accsum, total_wait_time = 0, total_completion_time = 0;
    float average_wait_time = 0.0, average_completion_time = 0.0;

    /* start computing waiting time */
    wait_time[0] = 0; // first process doesn't wait
    for (int i = 1; i < num_of_process; i++)
    {
        wait_time[i] = 0;
        for (int j = 0; j < i; j++)
            wait_time[i] += burst_time[j];
    } /* end computing waiting time */

    // computing completion time of each process
    partial_sum(burst_time, burst_time + num_of_process, completion_time);

    cout << endl
         << "Process\t\tBurst Time\tWaiting Time\tCompletion Time";

    for (int i = 0; i < num_of_process; i++)
    {
        accsum = burst_time[i];
        total_wait_time += wait_time[i];
        total_completion_time += completion_time[i];
        cout << endl
             << " P[" << i + 1 << "]\t\t   " << burst_time[i] << "\t\t    " << wait_time[i] << "\t\t    " << completion_time[i];
    }

    average_wait_time = total_wait_time * 1.0 / num_of_process;
    average_completion_time = total_completion_time * 1.0 / num_of_process;
    cout << endl
         << endl
         << "Average Waiting Time: " << average_wait_time;
    cout << endl
         << "Average Complettion Time: " << average_completion_time << endl;

    int proc = 0;
    // computing process list
    for (int i = 0; i < time; i++)
    {
        if (burst_time[proc]-- < 1)
        {
            proc++;
            burst_time[proc]--;
        }
        process_list[i] = proc + 1; // process' number start from 1 not 0.
    }

    print_schedule(process_list, time);
}

void rate_monotonic(int time)
{
    float utilization = 0;
    for (int i = 0; i < num_of_process; i++)
    {
        utilization += (1.0 * execution_time[i]) / period[i];
    }
    int n = num_of_process;
    if (utilization > n * (pow(2, 1.0 / n) - 1))
    {
        cout << endl
             << "Given problem is not schedulable under said scheduling algorithm." << endl;
        exit(0);
    }

    int process_list[time] = {0}, min = 999, next_process = 0;
    for (int i = 0; i < time; i++)
    {
        min = 1000;
        for (int j = 0; j < num_of_process; j++)
        {
            if (remain_time[j] > 0)
            {
                if (min > period[j])
                {
                    min = period[j];
                    next_process = j;
                }
            }
        }

        if (remain_time[next_process] > 0)
        {
            process_list[i] = next_process + 1; // +1 for catering 0 array index.
            remain_time[next_process] -= 1;
        }

        for (int k = 0; k < num_of_process; k++)
        {
            if ((i + 1) % period[k] == 0)
            {
                remain_time[k] = execution_time[k];
                next_process = k;
            }
        }
    }
    print_schedule(process_list, time);
}

void earliest_deadline_first(int time)
{
    float utilization = 0;
    for (int i = 0; i < num_of_process; i++)
    {
        utilization += (1.0 * execution_time[i]) / deadline[i];
    }
    int n = num_of_process;
    if (utilization > 1)
    {
        cout << endl
             << "Given problem is not schedulable under said scheduling algorithm." << endl;
        exit(0);
    }

    int process[num_of_process];
    int max_deadline, current_process = 0, min_deadline, process_list[time];
    ;
    bool is_ready[num_of_process];

    for (int i = 0; i < num_of_process; i++)
    {
        is_ready[i] = true;
        process[i] = i + 1;
    }

    max_deadline = deadline[0];
    for (int i = 1; i < num_of_process; i++)
    {
        if (deadline[i] > max_deadline)
            max_deadline = deadline[i];
    }

    for (int i = 0; i < num_of_process; i++)
    {
        for (int j = i + 1; j < num_of_process; j++)
        {
            if (deadline[j] < deadline[i])
            {
                int temp = execution_time[j];
                execution_time[j] = execution_time[i];
                execution_time[i] = temp;
                temp = deadline[j];
                deadline[j] = deadline[i];
                deadline[i] = temp;
                temp = process[j];
                process[j] = process[i];
                process[i] = temp;
            }
        }
    }

    for (int i = 0; i < num_of_process; i++)
    {
        remain_time[i] = execution_time[i];
        remain_deadline[i] = deadline[i];
    }

    for (int t = 0; t < time; t++)
    {
        if (current_process != -1)
        {
            --execution_time[current_process];
            process_list[t] = process[current_process];
        }
        else
            process_list[t] = 0;

        for (int i = 0; i < num_of_process; i++)
        {
            --deadline[i];
            if ((execution_time[i] == 0) && is_ready[i])
            {
                deadline[i] += remain_deadline[i];
                is_ready[i] = false;
            }
            if ((deadline[i] <= remain_deadline[i]) && (is_ready[i] == false))
            {
                execution_time[i] = remain_time[i];
                is_ready[i] = true;
            }
        }

        min_deadline = max_deadline;
        current_process = -1;
        for (int i = 0; i < num_of_process; i++)
        {
            if ((deadline[i] <= min_deadline) && (execution_time[i] > 0))
            {
                current_process = i;
                min_deadline = deadline[i];
            }
        }
    }
    print_schedule(process_list, time);
}

int main(int argc, char *argv[])
{
    int option = 0;
    cout << "-----------------------------" << endl;
    cout << "CPU Scheduling Algorithms: " << endl;
    cout << "-----------------------------" << endl;
    cout << "1. First Come First Serve" << endl;
    cout << "2. Rate Monotonic Scheduling" << endl;
    cout << "3. Shortest Job First" << endl;
    cout << "-----------------------------" << endl;
    cout << "Select > ";
    cin >> option;
    cout << "-----------------------------" << endl;

    // get_process_info(option); // collecting processes detail
    // int observation_time = get_observation_time(option);

    if (option == 1)
    {
        get_process_info(option); // collecting processes detail
        int observation_time = get_observation_time(option);
        first_come_first_serve(observation_time);
    }

    // Rate Monotonic
    // 3 20, 2 5 , 2 10

    else if (option == 2)
    {
        get_process_info(3); // collecting processes detail
        int observation_time = get_observation_time(3);
        rate_monotonic(observation_time);
    }

    else if (option == 3)
    {
        int nop, choice, i;
        cout << "Enter number of processes\n";
        cin >> nop;
        insert(nop);
        disp(nop);
    }
    return 0;
}
