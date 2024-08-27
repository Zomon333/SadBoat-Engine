/*
Copyright 2024 Dagan Poulin, Justice Guillory
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

#include "events/event.hpp"

namespace SBE 
{
    /* 
    
    Looking for something? See the header file for implementation!
    Per "The C++ standard library: a tutorial and handbook";
    "The only portable way of using templates at the moment is to implement
    them in their header files by uding inline functions."

    Due to the highly templated nature of the SadBoat Engine, many
    implementations are inline in their respective header files.
    
    If you're getting linking errors and are looking for a solution, read more here first;
    https://stackoverflow.com/questions/12573816/what-is-an-undefined-reference-unresolved-external-symbol-error-and-how-do-i-fix/12574417#12574417
    
    ~Dagan Poulin

    */

    void Event<void, void>::call()
    {
        ((Event<int, int>*)(this))->call(0);
    }

    void Event<void, void>::launch()
    {
        ((Event<int, int>*)(this))->launch(0);
    }
}