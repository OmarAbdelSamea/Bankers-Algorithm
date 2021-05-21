#include <iostream>
#include <vector>
#include <queue>

#define SAFETY 0
#define RESOURCE_REQUEST 1

using namespace std;

bool safety_algorithm(int num_of_proccess, int num_of_resource_types, queue<int>& safety,
    vector<int> availble, vector<vector<int>> allocation, vector<vector<int>> need);

int main()
{
    bool type = SAFETY;
    int num_of_proccess = 5, num_of_resource_types = 4, request_process = 1;
    vector<int> availble(num_of_resource_types);
    vector<int> request(num_of_resource_types);
    vector<vector<int>> allocation(num_of_proccess, vector<int>(num_of_resource_types, 0)),
        max(num_of_proccess, vector<int>(num_of_resource_types, 0)),
        need(num_of_proccess, vector<int>(num_of_resource_types, 0));

    // Start of test cases
    request = { 0, 4, 2, 0 };
    availble = { 1, 5, 2, 0 };
    allocation = { {0, 0, 1, 2}, {1, 0, 0, 0}, {1, 3, 5, 4}, {0, 6, 3, 2}, {0, 0, 1, 4} };
    max = { {0, 0, 1, 2}, {1, 7, 5, 0}, {2, 3, 5, 6}, {0, 6, 5, 2}, {0, 6, 5, 6} };
    // End of test cases

    cout << "Enter 0 for safety 1 for resource_request\n";
    cin >> type;
    cout << "Enter number of proccess\n";
    cin >> num_of_proccess;
    cout << "Enter number of resource types\n";
    cin >> num_of_resource_types;

    cout << "Enter availble resource types seperated by spaces\n";
    for (size_t i = 0; i < num_of_resource_types; i++)
        cin >> availble[i];

    for (size_t i = 0; i < num_of_proccess; i++)
    {
        cout << "Enter allocation resource types of P" << i << " seperated by spaces\n";
        for (size_t j = 0; j < num_of_resource_types; j++)
            cin >> allocation[i][j];
    }

    for (size_t i = 0; i < num_of_proccess; i++)
    {
        cout << "Enter max resource types of P" << i << " seperated by spaces\n";
        for (size_t j = 0; j < num_of_resource_types; j++)
            cin >> max[i][j];
    }

    cout << "Need Matrix:" << '\n';
    for (size_t i = 0; i < num_of_proccess; i++)
    {
        for (size_t j = 0; j < num_of_resource_types; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
            cout << need[i][j] << ' ';
        }
        cout << '\n';
    }

    queue<int> safety;
    if (type == SAFETY)
    {
        bool no_deadlock = safety_algorithm(num_of_proccess, num_of_resource_types, safety, availble, allocation, need);
        if (no_deadlock)
        {
            cout << "Yes, Safe State <";
            for (size_t i = 0; i < num_of_proccess; i++)
            {
                cout << 'P' << safety.front();
                if (i < num_of_proccess - 1)
                    cout << ',';

                safety.pop();
            }
            cout << ">\n";
        }
        else
        {
            cout << "No Safe State\n";
        }
    }

    else if (type == RESOURCE_REQUEST)
    {
        cout << "Enter number of immediate proccess\n";
        cin >> request_process;
        cout << "Enter request resources of P" << request_process
            << '\n';
        for (size_t i = 0; i < num_of_resource_types; i++)
            cin >> request[i];
        bool need_pass = true, availble_pass = true;
        for (size_t i = 0; i < num_of_resource_types; i++)
        {
            if (request[i] > need[request_process][i])
                need_pass = false;

            if (request[i] > availble[i])
                availble_pass = false;
        }

        if (need_pass && availble_pass)
        {
            for (size_t i = 0; i < num_of_resource_types; i++)
            {
                availble[i] -= request[i];
                need[request_process][i] -= request[i];
                allocation[request_process][i] += request[i];
            }
            bool no_deadlock = safety_algorithm(num_of_proccess, num_of_resource_types, safety, availble, allocation, need);
            bool procces_done = true;
            if (no_deadlock)
            {
                cout << "Yes, Safe State <";
                cout << 'P' << request_process << "req,";
                for (size_t i = 0; i < num_of_resource_types; i++)
                {
                    if (need[request_process][i] != 0)
                    {
                        procces_done = false;
                        break;
                    }
                }

                for (size_t i = 0; i < num_of_proccess; i++)
                {
                    if (procces_done && safety.front() == request_process)
                    {
                        safety.pop();
                        continue;
                    }

                    cout << 'P' << safety.front();
                    if (i < num_of_proccess - 1)
                        cout << ',';

                    safety.pop();
                }
                cout << ">\n";
            }
            else
            {
                cout << "No Safe State\n";
            }
        }
        else if (!need_pass)
            cout << "Process has exceeded its maximum claim\n";
        else if (!availble_pass)
            cout << "Process must wait since resources are not availble\n";
    }
    system("pause");
    return 0;
}

bool safety_algorithm(int num_of_proccess, int num_of_resource_types, queue<int>& safety,
    vector<int> availble, vector<vector<int>> allocation, vector<vector<int>> need)
{
    vector<bool> finish(num_of_proccess, 0);
    bool pass = false, no_deadlock = true;
    while (!all_of(finish.begin(), finish.end(), [](bool i)
        { return i; }) &&
        no_deadlock)
    {
        no_deadlock = false;
        for (size_t i = 0; i < num_of_proccess; i++)
        {
            if (finish[i] == false)
            {
                pass = true;
                for (size_t j = 0; j < num_of_resource_types; j++)
                {
                    if (need[i][j] > availble[j])
                        pass = false;
                }
                if (pass)
                {
                    safety.push(i);
                    for (size_t j = 0; j < num_of_resource_types; j++)
                        availble[j] += allocation[i][j];

                    finish[i] = true;
                    no_deadlock = true;
                }
            }
        }
    }
    return no_deadlock;
}