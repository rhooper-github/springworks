#include "stdafx.h"
#include "CppUnitTest.h"

#include "springworks.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace springworkstest
{		
	TEST_CLASS(RouteTest)
	{
	public:
		
		TEST_METHOD(RouteBuildingTest)
		{
			//load the waypoints file, and test manually that data from each waypoint are in the graph
			buildGraphFromWaypointsFile("C:\\Users\\rob\\Repos\\waypoints\\waypoints-cli\\x64\\Debug\\waypoints.json");
			
			//check the right number of waypoints was created
			size_t nwp = getNumWaypoints();
			Assert::AreEqual((int)nwp,5, L"Incorrect number of waypoints.");

			//check random indices property values match
			double lat = getLatitude(0);
			Assert::AreEqual(59.334, lat, L"Incorrect latitude value.");

			double lon = getLongitude(1);
			Assert::AreEqual(18.0662, lon, L"Incorrect longitude value.");

			double speed = getSpeed(2);
			Assert::AreEqual(11.1, speed, L"Incorrect speed value.");

			double speedlimit = getSpeedLimit(3);
			Assert::AreEqual(8.33, speedlimit, L"Incorrect speedlimit value.");

			string timestamp = getTimestamp(4);
			Assert::AreEqual("2016-06-21T12:00:20", &timestamp[0], L"Timestamps strings not the same.");

			int total_seconds = getSecondsBetweenWaypoints(0,4);
			Assert::AreEqual(20, total_seconds, L"Number of seconds duration of total trip not equal to 20.");

			std::vector<int> spd = getSpeedingWaypointIndices();
			int size = spd.size();
			//Assert::AreEqual(size, 2, "Incorrect number of speeding waypoints");
			Assert::AreEqual(spd[0], 1, L"Incorrect isspeeding assertion.");
			Assert::AreEqual(spd[1], 2, L"Incorrect isspeeding assertion.");
		}

		TEST_METHOD(GPSDistanceTest)
		{
			//check distance calc is the same is what i found online. I use tolerance of 2 meters here but I suppose it should be smarter.
			buildGraphFromWaypointsFile("waypoints.json");
			double dist1 = 43.78; //from http://www.movable-type.co.uk/scripts/latlong.html 
			waypoint wp1, wp2;
			wp1.position.latitude = 59.3340;
			wp1.position.longitude = 18.0667;
			wp2.position.latitude = 59.3337;
			wp2.position.longitude = 18.0662;
			double dist2 = calc_gps_distance(wp1, wp2);
			Assert::IsTrue(std::abs(dist1 - dist2) < 2, L"Distance 1 caluclations not the same.");

			double dist3 = 44.84; //from http://www.movable-type.co.uk/scripts/latlong.html 
			waypoint wp3, wp4;
			wp3.position.latitude = 59.3327;
			wp3.position.longitude = 18.0665;
			wp4.position.latitude = 59.3323;
			wp4.position.longitude = 18.0666;
			double dist4 = calc_gps_distance(wp3, wp4);
			Assert::IsTrue(std::abs(dist3 - dist4) < 2, L"Distance 2 caluclations not the same.");
		}
	};

}