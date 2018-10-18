#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <fstream>

#define EPSYLONE 0.01f

void openModel();
void closedModel();

int main()
{
    closedModel();

	return 0;
}

void openModel()
{
    //data collecting
    int totalCustomers = 0;
    int discVisitors[4] = { 0 };
    int maximumConcurrent = 0;
    int totalCompleted = 0;
    ////////////////////////


    int maxDownloadTime = 0;

    const float cpuTime = 39.4f;
    bool cpuProcessing = false;
    float cpuProgress = 0;
    int cpuQueue = 0;

    const float discTime[4] = { 77.1f, 123.8f, 80.4f, 235.f };
    int discQueue[4] = { 0 };
    bool discProcessing[4] = { false };
    float discProgress[4] = { 0.f };

    float arrival = 0;
    float time = 0;

    printf("type arrival rate (ms): ");
    std::cin >> arrival;
    std::cin.ignore();

    if (arrival <= 0)
    {
        cpuQueue = INT_MAX;
        arrival = -1;
    }

    printf("type timeframe (ms): ");
    std::cin >> time;
    std::cin.ignore();

    //Testa att ha "hur m�nga requests av massa hinner den g�ra p� T tid".

    //converting time to microseconds for better precision
    time *= 10;

    int arrivalCounter = 0;
    for (int i = 0; i < time; i++)
    {
        arrivalCounter++;


        if (abs((arrivalCounter / 10.f) - arrival) < EPSYLONE)
        {
            cpuQueue++;
            arrivalCounter = 0;
            totalCustomers++;
            //printf("Boy arrived! (%f)\r\n", (i / 10.f));
        }

        if (!cpuProcessing && cpuQueue > 0)
        {
            cpuQueue--;
            cpuProcessing = true;
        }

        if (cpuProcessing)
        {
            cpuProgress += 0.1f;

            if (abs(cpuProgress - cpuTime) < EPSYLONE)
            {
                cpuProcessing = false;
                cpuProgress = 0.f;

                //find most efficient queue
                int shortest = 0;
                for (int i = 0; i < 4; i++)
                {
                    //accounting for if the disc is working or not
                    if (discQueue[i] + (int)discProcessing[i] < discQueue[shortest] + (int)discProcessing[shortest])
                        shortest = i;
                }

                //printf("Skicka till disk %d!\r\n", shortest);
                discQueue[shortest]++;
                discVisitors[shortest]++;
            }
        }

        //disk processing
        int currentlyWorking = 0;
        for (int i = 0; i < 4; i++)
        {
            if (!discProcessing[i] && discQueue[i] > 0)
            {
                discQueue[i]--;
                discProcessing[i] = true;
            }

            if (discProcessing[i])
            {
                currentlyWorking++;
                discProgress[i] += 0.1f;

                if (abs(discProgress[i] - discTime[i]) < EPSYLONE)
                {
                    // printf("Disk %d klar!\r\n", i);
                    discProcessing[i] = false;
                    discProgress[i] = 0.f;
                    totalCompleted++;
                }
            }
        }
        maximumConcurrent = std::max(maximumConcurrent, currentlyWorking);

    }


    std::cout << "Cpu queue: " << cpuQueue << " --- Total customers: " << totalCustomers << std::endl;

    for (int i = 0; i < 4; i++)
    {
        std::cout << "Disc " << i << " - Queue size: " << discQueue[i] << " --- Total requests: " << discVisitors[i] << std::endl;
    }

    std::cout << "Maximum concurrent workers measured: " << maximumConcurrent << std::endl;
    std::cout << "Total amount of jobs completed: " << totalCompleted << std::endl;

    system("pause");
}

void closedModel()
{
    //R[n] = D(1 + Q[n - 1])
    //X[n] = n / (sum(i = 0, m, R'[i]))
    //Q[n] = X[n] * R[n]
    //m = queueing nodes i.e. disks & cpu

    const int NODES = 5;                                                    //m
    std::vector<float> systemThroughput(0, 0);                              //X
    std::vector<float> residenceTime[NODES];                                //R'i
    std::vector<float> utilization[NODES];                                  //Ui
    std::vector<float> queueLength[NODES];                                  //Qi
    float demand[NODES] = { 0.0394f, 0.0771f, 0.1238f, 0.0804f, 0.235f};    //Di
    int customers = 0;                                                      //N     //iterations <-- commentcomment

    systemThroughput.push_back(0);

    for (int i = 0; i < NODES; i++)
    {
        residenceTime[i].push_back(0);
        utilization[i].push_back(0);
        queueLength[i].push_back(0);
    }


    std::cout << "Enter population (int): ";
    std::cin >> customers;
    std::cin.ignore();

    for (int i = 1; i < customers; i++)
    {
        for (int j = 0; j < NODES; j++)
        {
            residenceTime[j].push_back(demand[j] * (1 + queueLength[j][i - 1]));
        }

        float throughput = 0.f;
        for (int j = 0; j < NODES; j++)
        {
            throughput += residenceTime[j][i];
        }

        systemThroughput.push_back(i / throughput);

        for (int j = 0; j < NODES; j++)
        {
            queueLength[j].push_back(systemThroughput[i] * residenceTime[j][i]);
            utilization[j].push_back(systemThroughput[i] * demand[j] * 100);
        }
    }

    //Printing big data
    std::string info[NODES] = { "CPU           --- ","Disk 1        --- ","Disk 2        --- ","Disk 3        --- ","Disk 4        --- " };

    std::string answer = "";
    std::cout << "Display data? (Y/N) ";
    std::cin >> answer;

    int from = 0;
    std::cout << "Display how many from end(int): ";
    std::cin >> from;
    std::cin.ignore();

    if (from <= 0)
    {
        from = customers;
    }

    if (answer == "Y" || answer == "y")
    {

        std::cout << std::fixed << std::showpoint << std::setprecision(3);

        for (int i = customers - from; i < customers; i++)
        {
            std::cout << "Customer " << std::setw(4) << i << " --- Throughput: " << std::setw(8) << systemThroughput[i] << "\r\n";

            for (int j = 0; j < NODES; j++)
            {
                std::cout << info[j] << "Queue: " << std::setw(13) << queueLength[j][i] <<
                    " --- Residence time: " << std::setw(9) << residenceTime[j][i] <<       //also response time
                    " --- Utilization: " << std::setw(9) << utilization[j][i] << "% \r\n";

            }

            std::cout << std::endl;
        }
    }

    answer = "";
    std::cout << "save data? (Y/N) ";
    std::cin >> answer;

    if (answer == "Y" || answer == "y")
    {
        std::string fileName("");
        std::cout << "Enter file name WITH EXTENSION: ";
        std::cin >> fileName;

        std::ofstream file(fileName);

        for (int i = 0; i < customers; i++)
        {
            file << "Customer " << std::setw(4) << i << " --- Throughput: " << std::setw(8) << systemThroughput[i] << std::endl;

            for (int j = 0; j < NODES; j++)
            {
                file << info[j] << "Queue: " << std::setw(13) << queueLength[j][i] <<
                    " --- Residence time: " << std::setw(9) << residenceTime[j][i] << //also response time
                    " --- Utilization: " << std::setw(9) << utilization[j][i] << "%" << std::endl;

            }

            file << std::endl;
        }

        file.close();
    }

    system("pause");
}
