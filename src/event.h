#ifndef EVENT_H
#define EVENT_H

#include <chrono>
#include <queue>
using namespace std;


template<class R, class P> class Event
{
    private:
        static priority_queue<pair<chrono::milliseconds, Event*>,vector<pair<chrono::milliseconds, Event*>>,greater<pair<chrono::milliseconds, Event*>>> eventScheduler;

        R (*eventFunction)(P parameter);
        
        P parameters;
        R returnable;

        bool multiThreaded;


    public:
        Event() //Creates event with return of R and parameters of P at NOW
        {
            Event<R, P>* toPass;
        }



};









#endif