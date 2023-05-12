using System;

class BankerAlgorithm
{
    static int numProcesses, numResources;
    static int[] available, totalResources;
    static int[,] max, allocation, need;

    static void Main(string[] args)
    {
        Console.WriteLine("Enter the number of processes: ");
        numProcesses = int.Parse(Console.ReadLine());

        Console.WriteLine("Enter the number of resources: ");
        numResources = int.Parse(Console.ReadLine());

        // Initialize arrays
        available = new int[numResources];
        totalResources = new int[numResources];
        max = new int[numProcesses, numResources];
        allocation = new int[numProcesses, numResources];
        need = new int[numProcesses, numResources];

        // Read in total resources
        Console.WriteLine("Enter total resources: ");
        for (int i = 0; i < numResources; i++)
        {
            totalResources[i] = int.Parse(Console.ReadLine());
        }

        // Read in allocation
        Console.WriteLine("Enter current allocation: ");
        for (int i = 0; i < numProcesses; i++)
        {
            for (int j = 0; j < numResources; j++)
            {
                allocation[i, j] = int.Parse(Console.ReadLine());
            }
        }

        // Read in maximum need
        Console.WriteLine("Enter maximum need: ");
        for (int i = 0; i < numProcesses; i++)
        {
            for (int j = 0; j < numResources; j++)
            {
                max[i, j] = int.Parse(Console.ReadLine());
                need[i, j] = max[i, j] - allocation[i, j];
            }
        }

        // Calculate available resources
        for (int j = 0; j < numResources; j++)
        {
            int sum = 0;
            for (int i = 0; i < numProcesses; i++)
            {
                sum += allocation[i, j];
            }
            available[j] = totalResources[j] - sum;
        }

        // Print initial state
        Console.WriteLine("Initial state:");
        PrintState();

        // Prompt for resource request
        while (true)
        {
            Console.WriteLine("Enter a process id and a resource request (e.g. '0 2 1 0' means process 0 requests 2 units of resource 1 and 0 units of resource 2): ");
            string[] input = Console.ReadLine().Split(' ');
            int pid = int.Parse(input[0]);
            if (pid < 0 || pid >= numProcesses)
            {
                Console.WriteLine("Invalid process id.");
                continue;
            }
            int[] request = new int[numResources];
            for (int i = 0; i < numResources; i++)
            {
                request[i] = int.Parse(input[i + 1]);
            }

            // Check if request is valid
            if (IsSafeState(pid, request))
            {
                // Grant the request
                for (int i = 0; i < numResources; i++)
                {
                    allocation[pid, i] += request[i];
                    need[pid, i] -= request[i];
                    available[i] -= request[i];
                }
                Console.WriteLine("Request granted.");
                PrintState();
            }
            else
            {
                Console.WriteLine("Request denied.");
            }
        }
    }

    static bool IsSafeState(int pid, int[] request)
    {
        int[] work = new int[numResources];
        bool[] finish = new bool[numProcesses];

        // Copy available resources
        Array.Copy(available, work, numResources);

        // Pretend to allocate requested resources
        for (int i = 0; i < numResources; i++)
        {
            work[i] -= request[i];
            allocation[pid, i] += request[i];
            need[pid, i] -= request[i];
        }

        // Mark all processes as unfinished
        for (int i = 0; i < numProcesses; i++)
        {
            finish[i] = false;
        }

        // Find a safe sequence
        int[] safeSequence = new int[numProcesses];
        int count = 0;
        while (count < numProcesses)
        {
            bool found = false;
            for (int i = 0; i < numProcesses; i++)
            {
                if (!finish[i])
                {
                    int j;
                    for (j = 0; j < numResources; j++)
                    {
                        if (need[i, j] > work[j])
                        {
                            break;
                        }
                    }
                    if (j == numResources)
                    {
                        for (int k = 0; k < numResources; k++)
                        {
                            work[k] += allocation[i, k];
                        }
                        safeSequence[count++] = i;
                        finish[i] = true;
                        found = true;
                    }
                }
            }
            if (!found)
            {
                break;
            }
        }

        // Undo pretend allocation
        for (int i = 0; i < numResources; i++)
        {
            work[i] += request[i];
            allocation[pid, i] -= request[i];
            need[pid, i] += request[i];
        }

        // Check if a safe sequence was found
        if (count < numProcesses)
        {
            return false;
        }
        else
        {
            Console.Write("Safe sequence: ");
            for (int i = 0; i < numProcesses; i++)
            {
                Console.Write(safeSequence[i] + " ");
            }
            Console.WriteLine();
            return true;
        }
    }

    static void PrintState()
    {
        Console.WriteLine("Available resources:");
        for (int i = 0; i < numResources; i++)
        {
            Console.Write(available[i] + " ");
        }
        Console.WriteLine();

        Console.WriteLine("Allocation:");
        for (int i = 0; i < numProcesses; i++)
        {
            for (int j = 0; j < numResources; j++)
            {
                Console.Write(allocation[i, j] + " ");
            }
            Console.WriteLine();
        }

        Console.WriteLine("Maximum need:");
        for (int i = 0; i < numProcesses; i++)
        {
            for (int j = 0; j < numResources; j++)
            {
                Console.Write(max[i, j] + " ");
            }
            Console.WriteLine();
        }

        Console.WriteLine("Need:");
        for (int i = 0; i < numProcesses; i++)
        {
            for (int j = 0; j < numResources; j++)
            {
                Console.Write(need[i, j] + " ");
            }
            Console.WriteLine();
        }
    }
}