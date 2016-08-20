// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SPRINGWORKS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SPRINGWORKS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SPRINGWORKS_EXPORTS
#define SPRINGWORKS_API __declspec(dllexport)
#else
#define SPRINGWORKS_API __declspec(dllimport)
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;

enum SPRINGWORKS_API EdgeType {CENTRAL, SPEEDING};
enum SPRINGWORKS_API VertexType {MOVING, IDLE};

static const int R = 6371e3;//meters
static const int PI = 3.14159;

struct SPRINGWORKS_API waypoint {
	boost::posix_time::ptime timestamp;
	struct {
		double latitude;
		double longitude;
	} position;
	bool isIdle;
};

struct SPRINGWORKS_API accumulators {
	double distance_speeding;
	boost::posix_time::time_duration duration_speeding;
	boost::posix_time::time_duration duration_since_started_speeding;
	double total_distance;
	boost::posix_time::time_duration total_duration;
};
struct SPRINGWORKS_API segment {
	boost::posix_time::time_duration duration;
	double distance;
	double speed_limit;
	double speed;
	accumulators reductions;
	bool isSpeeding;
};
//starts and terminates with vertex, while parsing and building graph, skip waypoint if same position and both 0 speeds.

typedef SPRINGWORKS_API boost::adjacency_list<
	boost::vecS, boost::vecS, boost::directedS,
	waypoint, segment>
	Route;

static Route route;

SPRINGWORKS_API double calc_gps_distance(waypoint& wp_prev, waypoint& wp);

SPRINGWORKS_API int buildGraphFromWaypointsFile(const char * fname);

SPRINGWORKS_API int calcReductions();

SPRINGWORKS_API int importJSONFile(const char * fname);

SPRINGWORKS_API void reportEdgeProperties(Route::edge_descriptor ei);

SPRINGWORKS_API void reportVertexProperties(Route::vertex_descriptor vi);

//just for testing really...
SPRINGWORKS_API size_t getNumWaypoints();

SPRINGWORKS_API double getLatitude(const int index);

SPRINGWORKS_API double getLongitude(const int index);

SPRINGWORKS_API double getSpeed(const int index);

SPRINGWORKS_API double getSpeedLimit(const int index);

SPRINGWORKS_API std::vector<int> getSpeedingWaypointIndices();

SPRINGWORKS_API string getTimestamp(const int index);

SPRINGWORKS_API int getSecondsBetweenWaypoints(const int start, const int end);