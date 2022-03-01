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
#ifndef GLOBALS_H
#define GLOBALS_H

#include "pixel.h"
#include "registry.h"
#include <stack>
#include <thread>
#include <irrKlang/irrKlang.h>

using namespace std;

//unordered_map<string, unordered_map<string, pair<string, pair<string, string>>>> allRegistries;

Registry<string, pair<string, pair<string, string>>> *configRegistry;
Registry<string, pair<string, pair<string, string>>> *registriesRegistry;
stack<thread*> callStack;

irrklang::ISoundEngine* engine;

int X_RES;
int Y_RES;

#endif