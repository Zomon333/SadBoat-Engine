#pragma once
#include "headers.h"
using namespace std;

void error_callback(int error, const char* description)
{
	cout << "Error: " << to_string(error) << " " << description;
}