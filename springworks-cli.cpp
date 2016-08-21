#include "springworks.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;


bool routeBuildingTest() {
			//load the waypoints file, and test manually that data from each waypoint are in the graph
			buildGraphFromWaypointsFile("waypoints.json");
			
			//check the right number of waypoints was created
			size_t nwp = getNumWaypoints();
            if ((int)nwp < 5) { cout <<  "Test Fail: Incorrect number of waypoints. Exiting..." << endl; return false;}

			//check random indices property values match
			double lat = getLatitude(0);
            if (59.334 != lat) { cout <<  "Test Fail: Incorrect latitude value. Exiting..." << endl; return false;}

			double lon = getLongitude(1);
            if (18.0662 != lon) { cout <<  "Test Fail: Incorrect longitude value. Exiting..." << endl; return false;}

			double speed = getSpeed(2);
            if (11.1 != speed) { cout <<  "Test Fail: Incorrect speed value. Exiting..." << endl; return false;}

			double speedlimit = getSpeedLimit(3);
            if (8.33 != speedlimit) { cout <<  "Test Fail: Incorrect speedlimit value. Exiting..." << endl; return false;}

			string timestamp = getTimestamp(4);
            if (strcmp("2016-06-21T12:00:20", &timestamp[0]) != 0) { cout <<  "Test Fail: Timestamps strings not the same. Exiting..." << endl; return false;}

			int total_seconds = getSecondsBetweenWaypoints(0,4);
            if (20 != total_seconds) { cout <<  "Test Fail: Number of seconds duration of total trip not equal to 20. Exiting..." << endl; return false;}

			std::vector<int> spd = getSpeedingWaypointIndices();
			int size = spd.size();
			//Assert::AreEqual(size, 2, "Incorrect number of speeding waypoints");
            if (1 != spd[0]) { cout <<  "Test Fail: Incorrect isspeeding assertion. Exiting..." << endl; return false;}
            if (2 != spd[1]) { cout <<  "Test Fail: Incorrect isspeeding assertion. Exiting..." << endl; return false;}
            return true;
}

bool GPSDistanceTest() {
			//check distance calc is the same is what i found online. I use tolerance of 1 meters here but I suppose it should be smarter.
			buildGraphFromWaypointsFile("waypoints.json");
			double dist1 = 43.78; //from http://www.movable-type.co.uk/scripts/latlong.html 
			waypoint wp1, wp2;
			wp1.position.latitude = 59.3340;
			wp1.position.longitude = 18.0667;
			wp2.position.latitude = 59.3337;
			wp2.position.longitude = 18.0662;
			double dist2 = calc_gps_distance(wp1, wp2);
            if (std::abs(dist1 - dist2) > 1) { cout <<  "Distance 1 caluclations not the same." << " Actual: " << dist2 << " Expected: " << dist1 << endl; return false;}

			double dist3 = 44.84; //from http://www.movable-type.co.uk/scripts/latlong.html 
			waypoint wp3, wp4;
			wp3.position.latitude = 59.3327;
			wp3.position.longitude = 18.0665;
			wp4.position.latitude = 59.3323;
			wp4.position.longitude = 18.0666;
			double dist4 = calc_gps_distance(wp3, wp4);
            if (std::abs(dist3 - dist4) > 1) { cout <<  "Distance 2 caluclations not the same." << " Actual: " << dist4 << " Expected: " << dist3 << endl; return false;}
            return true;
}
void printHelp() {
	string desc = "Program for SpringWorks interview 17.08.2016";
	string usage = "Usage: springworks-cli [help] [test] [filename] Provide a path string as the first positional argument to this program to load waypoints.json, calculate, and give a printout.";
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
    if (!strcmp(argv[1],"test")) {
       cout << "RUNNING TESTS" << endl;
       cout << "=============" << endl;
       if (routeBuildingTest()) {
           cout << "Test Success: Graph build correctly from waypoint.json. Continuing..." << endl;
       } else {
           cout << "Test Fail: Graph not build correctly from waypoint.json. Exiting..." << endl; return EXIT_FAILURE;
       }
       if ((GPSDistanceTest())) {
           cout << "Test Success: Calculations performed correctly on graph. Continuing..." << endl;
       } else {
           cout << "Test Fail: Graph not build correctly from waypoint.json. Exiting..." << endl; return EXIT_FAILURE;
       }
       cout << "FINISHED TESTS: All tests run successfully. Exiting..." << endl; return EXIT_FAILURE;
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
    return EXIT_SUCCESS;
}
