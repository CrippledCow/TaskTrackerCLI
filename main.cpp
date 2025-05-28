#include <iostream>
#include <time.h>
#include <vector>
#include <unordered_map>
#include "json.hpp"
#include "fstream"
#include <string.h>

using json = nlohmann::json;
using namespace std;

class Task{

    private:
        int id;
        string description;
        string status;
        string createdAt;
        string updatedAt;
    public:
        static int objectCount;
        Task(string description)
        {
            objectCount++;
            this->id = objectCount;
            this->description = description;
            this->status = "todo";
            createdAt = __DATE__ " " __TIME__;
            updatedAt = __DATE__ " " __TIME__;
        }
        void printDetails() const
        {
            cout << "\nID: " << id << "\nDescription: " << description << "\nStatus: " << status <<  "\ncreatedAt: " << createdAt << "\nupdatedAt: " << updatedAt << endl;
        }
        void markInProgress()
        {
            this->status = "in-progress";
        }
        void markDone()
        {
            this->status = "done";
        }
        string getStatus() const 
        {
            return status;
        }
        int getId() const 
        {
            return id;
        }
        string getDescription() const 
        {
            return description;
        }
        string getCreatedAt() const
        {
            return createdAt;
        }
        string getUpdatedAt() const
        {
            return updatedAt;
        }
        void updateTime()
        {
            updatedAt = __DATE__ " " __TIME__;
        }
};

class TaskManager{
    private:
        unordered_map <int, Task> taskContainer;
        string json_filename = "tasks.json";
    public:
        void saveToFile() const
        {
            json tasksJson = json::array(); // Create a JSON array

            // Convert task object in the unordered_map to a JSON object
            for(const auto& pair : taskContainer)
            {
                const Task& task = pair.second;
                json taskObj;
                taskObj["id"] = task.getId();
                taskObj["description"] = task.getDescription();
                taskObj["status"] = task.getStatus();
                taskObj["createdAt"] = task.getCreatedAt();
                taskObj["updatedAt"] = task.getUpdatedAt();
                tasksJson.push_back(taskObj);
            }
            // Write the JSON array to the file
            ofstream file(json_filename);
            file << tasksJson.dump(4);
            file.close();
            cout << "-> State saved to " << json_filename << endl;
        }

        void addTask(string description)
        {
            Task addedTask(description);
            taskContainer.emplace(addedTask.getId(), addedTask);
            cout << "\nTask added successfully (ID: " << addedTask.getId() << ")" << endl;
            saveToFile();
        }
        void printTaskDetails(int id) const
        {
            taskContainer.at(id).printDetails();
        }
        void markInProgress(int id)
        {
            taskContainer.at(id).markInProgress();
            saveToFile();
        }
        void markDone(int id)
        {
            taskContainer.at(id).markDone();
            saveToFile();
        }
        void deleteTask(int id)
        {
            taskContainer.erase(id);
            saveToFile();
        }
        void updateTaskTime(int id)
        {
            taskContainer.at(id).updateTime();
            saveToFile();
        }
        void listTodo() const
        {
            for(auto &pair : taskContainer)
            {
                if(pair.second.getStatus() == "todo")
                {
                    cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
                }
            }
        }
        void listInProgress() 
        {
            for(auto &pair : taskContainer)
            {
                if(pair.second.getStatus() == "in-progress")
                {
                    cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
                    updateTaskTime(pair.first);
                }
            }
        }
        void listDone() 
        {
            for(auto &pair : taskContainer)
            {
                if(pair.second.getStatus() == "done")
                {
                    cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
                    updateTaskTime(pair.first);
                }
            }
        }
        void list() const
        {
            for(auto &pair : taskContainer)
            {
                cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
            }
        }
        void processPositionalArgument(string posArg)
        {
            string numberString;
            int j = 0;

            for(int i = 0; i < posArg.length(); i++)
            {
                if(posArg[i] <= '0' && posArg[i] <= '9')
                {
                    numberString[j] = posArg[i];
                    j++;
                }
            }

            int foundId = stoi(numberString);

            if(posArg.contains("task-cli"));
            {
                if(posArg.contains("add"))
                {
                    int quotationIndex1, quotationIndex2;
                    quotationIndex1 = posArg.find('\"');
                    quotationIndex2 = posArg.find('\"', quotationIndex1+1);
                    string subStringAdded;
                    for(int i = quotationIndex1; i < quotationIndex2+1; i++)
                    {
                        subStringAdded[i] = posArg[quotationIndex1 + i];
                    }
                    addTask(subStringAdded);
                }
                if(posArg.contains("mark-in-progress"))
                {
                    markInProgress(foundId);
                }
                if(posArg.contains("mark-done"))
                {
                    markInProgress(foundId);
                }
                if(posArg.contains("list"))
                {
                    list();
                }
                if(posArg.contains("list todo"))
                {
                    listTodo();
                }
                if(posArg.contains("list in-progress"))
                {
                    listInProgress();
                }
                if(posArg.contains("list done"))
                {
                    listDone();
                }
            }
        }
};

int Task::objectCount = 0;

int main()
{
    TaskManager TaskManager1;
    string userArgument;
    
    while(1)
    {
        cout << "Enter your command" << endl;
        cin >> userArgument;
        TaskManager1.processPositionalArgument(userArgument);
    }
}