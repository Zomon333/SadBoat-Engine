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
#ifndef VARGROUP_H
#define VARGROUP_H

using namespace std;

namespace SBE
{
    class VarGroup
    {
    private:
        unordered_map<int, any> backing; 

    public:

        //  Constructors
        //----------------------------------

        VarGroup();
        VarGroup(int addr, any val)
        {
            backing[addr]=val;
        }
        VarGroup(pair<int, any> pairing)
        {
            backing[pairing.first]=pairing.second;
        }

        template <class type>
        VarGroup(void* addr)
        {
            backing[((long long uint)(addr))]=(type)(*addr);
        }

        //  Mutators
        //----------------------------------

        void add(int addr, any val)
        {
            backing[addr]=val;
        }

        void add(pair<int, any> pairing)
        {
            backing[pairing.first]=pairing.second;
        }

        template <class type>
        void add(void* addr)
        {
            backing[((long long uint)(addr))]=(type)(*addr);
        }

        void remove(void* addr)
        {
            backing[((long long uint)(addr))]=0;
        }
        void remove(int addr)
        {
            backing[addr]=0;
        }

        //  Accessors
        //----------------------------------

        any get(void* addr)
        {
            return backing[((long long uint)(addr))];
        }
        any get(int addr)
        {
            return backing[addr];
        }
    };
};
#endif