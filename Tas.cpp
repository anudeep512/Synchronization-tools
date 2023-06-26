#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <fstream>
#include <atomic> // For atomic flag for TAS ME
#include <random> // For generation of t1 and t2
#include <chrono> // For time calculations
#include <thread>
#include <pthread.h> // Thread library

using namespace std;
using namespace chrono;
class Data
{
public:
  int tno; // Stores the thread number of thread
};
typedef class Data data;

atomic_flag tas_flag = ATOMIC_FLAG_INIT;
// Asssuming that the number of threads considered are less than 60
double times_threads[60][10]; // This array is used to store the times generated in each thread
double max_time = -1;         // Max value of time in the particular thread value
int N = 0;                    // Variable for total number of threads
int K = 0;                    // Variable for storing number of times a thread requests for entry to critical section
int lamb1 = 0;                // Mean value for exponential distribution of t1
int lamb2 = 0;                // Mean value for exponential distribution of t2

void tas_enter()
{
  while (tas_flag.test_and_set())
    ; // Entry section
  return;
}

void tas_exit()
{
  tas_flag.clear();
  return;
}

void *tas(void *param)
{
  ofstream op;
  op.open("tas_output.txt", ios_base::app);
  data *dat = (data *)param;
  time_t raw_time;
  struct tm *timeinfo;

  int seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  exponential_distribution<double> t_1(1.0 / lamb1);
  exponential_distribution<double> t_2(1.0 / lamb2);

  double t1 = t_1(generator);
  double t2 = t_2(generator);
  for (int i = 0; i < K; i++)
  {

    auto start = high_resolution_clock::now(); // Clock starts
    tas_enter();
    time(&raw_time);
    timeinfo = localtime(&raw_time);
    op << i + 1 << "th CS Requested at " << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << " by thread " << dat->tno << endl;
    auto stop = high_resolution_clock::now(); // Time of entry
    time(&raw_time);
    op << i + 1 << "th CS Entered at " << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << " by thread " << dat->tno << endl;
    auto timees = duration_cast<microseconds>(stop - start);

    times_threads[dat->tno][i] = (double)timees.count();
    this_thread::sleep_for(chrono::milliseconds((int)t1)); // Simulation of critical section

    time(&raw_time);
    op << i + 1 << "th CS Exited at " << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << " by thread " << dat->tno << endl;
    tas_exit();
    this_thread::sleep_for(chrono::milliseconds((int)t2)); // Simulation of remainder section
  }
  op.close();
  return NULL;
}

int main()
{

  // The file opening is done to clear the file
  ofstream op;
  op.open("tas_output.txt", ios_base::out);
  op.close();

  ifstream input;
  input.open("inp-params.txt");
  input >> N >> K >> lamb1 >> lamb2;
  cout << N << " " << K << " "
       << " " << lamb1 << " " << lamb2 << endl;
  pthread_t tid[N]; // Taking array of threads
  pthread_attr_t attr;
  pthread_attr_init(&attr); // Setting default value of attribute
  for (int i = 0; i < N; i++)
  {
    data *a = (data *)calloc(1, sizeof(data));
    a->tno = i + 1;
    pthread_create(&tid[i], &attr, tas, a);
  }

  for (int i = 0; i < N; i++)
  {
    pthread_join(tid[i], NULL);
  }

  double sum = 0;
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < K; j++)
    {
      sum += times_threads[i][j];
      if (max_time < times_threads[i][j])
        max_time = times_threads[i][j];
    }
  }

  cout << "The average time for " << N << " threads in microseconds is : " << (double)sum / (N * K) << endl;
  cout << "The max time for " << N << " threads in microseconds is : " << max_time << endl;
  return 0;
}
