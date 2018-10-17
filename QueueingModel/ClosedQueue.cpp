#include <iostream>
#include <vector>
#include <algorithm>

#define EPSYLONE 0.01f

int main()
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

    printf("type timeframe (ms): ");
    std::cin >> time;
    std::cin.ignore();

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

	return 0;
}