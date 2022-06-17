/*
Copyright 2022 Dagan Poulin, Justice Guillory

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef IDMAN_H
#define IDMAN_H

#include <unordered_map>
#include <stack>

using namespace std;

class IdManager
{
private:
    int leadCount;
    unordered_map<int, bool> used;
    stack<int> unused;

public:
    //Constructors
    //----------------------------------
    IdManager()
    {
        used = unordered_map<int, bool>(255);
        unused = stack<int>();
        leadCount = -1;
    }

    ~IdManager()
    {
        while(leadCount>=0)
        {
            used[leadCount] = false;
            leadCount--;
        }
        
        while(unused.size()>0)
        {
            unused.pop();
        }
    }

    //Allocation Systems
    //----------------------------------
    int allocate()
    {
        if(unused.size()>0)
        {
            used[unused.top()] = true;
            int toReturn = unused.top();
            unused.pop();

            return toReturn;
        }
        
        leadCount++;
        used[leadCount] = true;

        return leadCount;
    }

    void free(int toFree)
    {
        unused.push(toFree);
        used[toFree]=false;
    }

};

#endif