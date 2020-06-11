#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include "AVLTree.hpp"
#include "PriorityQueue.hpp "

struct ProfileResult
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    InstrumentationSession* currentSession;
    std::ofstream outputStream;
    int profileCount;
public:
    Instrumentor()
            : currentSession(nullptr), profileCount(0)
    {
    }

    void BeginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        outputStream.open(filepath);
        WriteHeader();
        currentSession = new InstrumentationSession{name };
    }

    void EndSession()
    {
        WriteFooter();
        outputStream.close();
        delete currentSession;
        currentSession = nullptr;
        profileCount = 0;
    }

    void WriteProfile(const ProfileResult& result)
    {
        if (profileCount++ > 0)
            outputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        outputStream << "{";
        outputStream << "\"cat\":\"function\",";
        outputStream << "\"dur\":" << (result.End - result.Start) << ',';
        outputStream << "\"name\":\"" << name << "\",";
        outputStream << "\"ph\":\"X\",";
        outputStream << "\"pid\":0,";
        outputStream << "\"tid\":" << result.ThreadID << ",";
        outputStream << "\"ts\":" << result.Start;
        outputStream << "}";

        outputStream.flush();
    }

    void WriteHeader()
    {
        outputStream << "{\"otherData\": {},\"traceEvents\":[";
        outputStream.flush();
    }

    void WriteFooter()
    {
        outputStream << "]}";
        outputStream.flush();
    }

    static Instrumentor& Get()
    {
        static Instrumentor *instance = new Instrumentor();
        return *instance;
    }
};




class InstrumentationTimer
{
public:
    explicit InstrumentationTimer(const char* name)
            : name(name), stopped(false)
    {
        startTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        //uint32_t threadID = std::hash<std::async::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({name, start, end });

        //std::cout << name << ": " << (end - start) << "ms\n";

        stopped = true;
    }
private:
    const char* name;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
    bool stopped;
};



void benchmarkInsert(){
    InstrumentationTimer timer("benchmarkInsert");
    AVLTree<int, int> Tree;
    for (int i = 0; i < 10000; i++){
        Tree.insert(i,i);
    }
    Tree.clear();
};

void benchmarkEnqueue(){
    InstrumentationTimer timer("benchmarkEnqueue");
    PriorityQueue<int, int> Queue;
    for (int i = 0; i < 10000; i++){
        Queue.enqueue(i,i);
    }
    Queue.clear();
}

void runBenchmarks(){
   // InstrumentationTimer timer("runBenchamarks");

    std::cout << "Running benchmarks...\n";

    benchmarkInsert();
    benchmarkEnqueue();

    std::cout << "Check json file with results\n";
}

int main(){

    Instrumentor::Get().BeginSession("Profile");
    runBenchmarks();
    Instrumentor::Get().EndSession();
    return 0;
};
