#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;

enum EdgeType {CENTRAL, SPEEDING};
enum VertexType {MOVING, IDLE};

static const double R = 6371e3;//meters
static const double PI = 3.14159;

struct waypoint {
	boost::posix_time::ptime timestamp;
	struct {
		double latitude;
		double longitude;
	} position;
	bool isIdle;
};

struct accumulators {
	double distance_speeding;
	boost::posix_time::time_duration duration_speeding;
	boost::posix_time::time_duration duration_since_started_speeding;
	double total_distance;
	boost::posix_time::time_duration total_duration;
};
struct segment {
	boost::posix_time::time_duration duration;
	double distance;
	double speed_limit;
	double speed;
	accumulators reductions;
	bool isSpeeding;
};
//starts and terminates with vertex, while parsing and building graph, skip waypoint if same position and both 0 speeds.

typedef boost::adjacency_list<
	boost::vecS, boost::vecS, boost::directedS,
	waypoint, segment>
	Route;

static Route route;

double calc_gps_distance(waypoint& wp_prev, waypoint& wp);

int buildGraphFromWaypointsFile(const char * fname);

int calcReductions();

int importJSONFile(const char * fname);

void reportEdgeProperties(Route::edge_descriptor ei);

void reportVertexProperties(Route::vertex_descriptor vi);

//just for testing really...
size_t getNumWaypoints();

double getLatitude(const int index);

double getLongitude(const int index);

double getSpeed(const int index);

double getSpeedLimit(const int index);

std::vector<int> getSpeedingWaypointIndices();

string getTimestamp(const int index);

int getSecondsBetweenWaypoints(const int start, const int end);
