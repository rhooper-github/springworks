// springworks-cli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "springworks.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

void printHelp() {
	string desc = "Program for SpringWorks interview 17.08.2016";
	string usage = "Usage: Provide a path string as the first positional argument to this program to load waypoints.json, calculate, and give a printout.";
	string author = "Author: Robert Hooper";
	string notes = "Notes: Waypoints are transformed into a graph DS, edges hold distances, durations, speeds and speed_limits. Speed is considered true until the next waypoint. GPS is used for distance considering the globe as a sphere. Edges should really hold times, but at this point no optimization.";
	cout << desc << endl;
	cout << usage << endl;
	cout << author << endl;
	cout << notes << endl;
}

int main(int argc, char * argv[])
{
	if (argc < 2 || !strcmp(argv[1],"help") || !strcmp(argv[1],"-h") || !strcmp(argv[1], "--h")) {
		printHelp();
		return EXIT_SUCCESS;
	}
	if (buildGraphFromWaypointsFile(argv[1]) != EXIT_SUCCESS) { 
		cout << "Failure building Graph from JSON file." << argv[1] << endl;
		return EXIT_FAILURE;
	};
	if (calcReductions() != EXIT_SUCCESS) {
		cout << "Failure calculating values from graph." << endl;
		return EXIT_FAILURE;
	}
	getchar();
}

