// springworks.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "springworks.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

using namespace rapidjson;
using namespace std;

SPRINGWORKS_API double calc_gps_distance(waypoint& wp_prev, waypoint& wp) {
	double dLat = (wp.position.latitude - wp_prev.position.latitude) * PI / 180;
	double dLon = (wp.position.longitude - wp_prev.position.longitude) * PI / 180;
	double lat1 = wp_prev.position.latitude * PI / 180;
	double lat2 = wp.position.latitude * PI / 180;
	double a = pow(sin(dLat / 2),2) + pow(sin(dLon / 2),2) * cos(lat1) * cos(lat2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return R * c;
}

SPRINGWORKS_API int importJSONFile(Document& d, const char * fname) {
	FILE* fp = fopen(fname, "rb"); // non-Windows use "r"
	if (fp == NULL) {
		cout << "Could not open file " << fname << endl;
		return EXIT_FAILURE;
	}
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);
	fclose(fp);
	return EXIT_SUCCESS;
}

SPRINGWORKS_API int buildGraphFromWaypointsFile(const char * fname) {

	Document d;
	if (importJSONFile(d, fname) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	//create time parser
	boost::posix_time::time_input_facet *tif = new boost::posix_time::time_input_facet();
	tif->set_iso_extended_format();
	std::istringstream iss;
	std::locale ssloc = locale(locale::classic(), tif);
	iss.imbue(ssloc);

	assert(d.IsArray());
	Value::ConstValueIterator it, it_end, it_prev;
	it = d.Begin(); it_prev = it; it_end = d.End();
	Route::vertex_descriptor vd, vd_prev;
	waypoint wp, wp_prev;
	for (; it != it_end; ++it) {
		assert(it->HasMember("timestamp"));
		assert((*it)["timestamp"].IsString());
		assert(it->HasMember("position"));
		assert((*it)["position"].IsObject());
		assert((*it)["position"].HasMember("latitude"));
		assert((*it)["position"]["latitude"].IsDouble());
		assert((*it)["position"].HasMember("longitude"));
		assert((*it)["position"]["longitude"].IsDouble());

		iss.str((*it)["timestamp"].GetString());
		iss >> wp.timestamp;
		wp.position.latitude = (*it)["position"]["latitude"].GetDouble();
		wp.position.longitude = (*it)["position"]["longitude"].GetDouble();

		vd = add_vertex(wp, route);

		if (it_prev != it) {
			segment seg;
			assert(it_prev->HasMember("speed"));
			assert((*it_prev)["speed"].IsDouble());
			assert(it_prev->HasMember("speed_limit"));
			assert((*it_prev)["speed_limit"].IsDouble());
			seg.speed = (*it_prev)["speed"].GetDouble();
			seg.speed_limit = (*it_prev)["speed_limit"].GetDouble();
			if (seg.speed_limit < seg.speed) {
				seg.isSpeeding = true;
			}
			else {
				seg.isSpeeding = false;
			}
			//could check if GPS signal not available use speed * duration etc etc.
			//double distanceFromSpeed = seg.duration.total_seconds() * seg.speed;
			seg.distance = calc_gps_distance(wp_prev, wp);
			seg.duration = wp.timestamp - wp_prev.timestamp;
			Route::edge_descriptor ed = add_edge(vd_prev, vd, seg, route).first;
		}


		vd_prev = vd;
		it_prev = it;
		wp_prev = wp;
	}
	return EXIT_SUCCESS;
}

SPRINGWORKS_API size_t getNumWaypoints() {
	return num_vertices(route);
}

SPRINGWORKS_API void reportEdgeProperties(Route::edge_descriptor ed) {
		cout << endl;
		cout << "=============================" << endl;
		cout << "This is the " << ed << " edge of the graph" << endl;
		cout << "=============================" << endl;
		cout << "speed_limit: " << route[ed].speed_limit << endl;
		cout << "speed: " << route[ed].speed << endl;
		cout << "is_speeding: " << (route[ed].isSpeeding ? true : false) << endl;
		cout << "distance: " << route[ed].distance << endl;
		cout << "duration: " << route[ed].duration.total_seconds() << " s" << endl;
		cout << "rolling_total_distance: " << route[ed].reductions.total_distance << endl;
		cout << "rolling_total_duration: " << route[ed].reductions.total_duration.total_seconds() << " s" << endl;
		cout << "duration_since_started_speeding: " << route[ed].reductions.duration_since_started_speeding.total_seconds() << " s" << endl;
		cout << "duration_speeding: " << route[ed].reductions.duration_speeding.total_seconds() << " s" << endl;
}

SPRINGWORKS_API void reportVertexProperties(Route::vertex_descriptor vd) {
		cout << endl;
		cout << "=============================" << endl;
		cout << "This is the " << vd << "th vertex of the graph" << endl;
		cout << "=============================" << endl;
		cout << "timestamp: " << boost::posix_time::to_simple_string(route[vd].timestamp) << endl;
		cout << "latitude: " << route[vd].position.latitude << endl;
		cout << "longitude: " << route[vd].position.longitude << endl;
}
SPRINGWORKS_API int calcReductions() {
	Route::vertex_iterator vi, vi_end;
	for (boost::tie(vi, vi_end) = vertices(route); vi != vi_end; ++vi) {
		reportVertexProperties(*vi);
	}

	double total_distance = 0;
	boost::posix_time::time_duration total_duration;
	Route::edge_iterator ei, ei_end;
	double distance_since_started_speeding = 0;
	boost::posix_time::time_duration duration_speeding;
	boost::posix_time::time_duration duration_since_started_speeding;

	for (boost::tie(ei, ei_end) = edges(route); ei != ei_end; ++ei) {
		//add edge distance to total distance for summing total 
		total_distance += route[(*ei)].distance;
		route[(*ei)].reductions.total_distance = total_distance;
		//set edge total distance (for destination vertex)
		total_duration += route[(*ei)].duration;
		route[(*ei)].reductions.total_duration = total_duration;
		//add edge duration to rolling total if speeding (for destination vertex)
		if (route[(*ei)].isSpeeding) {
			duration_speeding += route[(*ei)].duration;
			duration_since_started_speeding += route[(*ei)].duration;
			//add edge duration to rolling total if speeding (for destination vertex)
		}
		else {
			//if no longer speeding, set to zero
			duration_since_started_speeding = boost::posix_time::milliseconds(0);
		}
		route[(*ei)].reductions.duration_since_started_speeding = duration_since_started_speeding;
		route[(*ei)].reductions.duration_speeding = duration_speeding;
		
		reportEdgeProperties(*ei);
	}
	return EXIT_SUCCESS;
}

SPRINGWORKS_API double getLatitude(const int index) {
	Route::vertex_descriptor vd = index;
	return route[vd].position.latitude;
}

SPRINGWORKS_API double getLongitude(const int index) {
	Route::vertex_descriptor vd = index;
	return route[vd].position.longitude;
}

SPRINGWORKS_API double getSpeed(const int index) {
	Route::vertex_descriptor vd = index;
	return route[*out_edges(vd, route).first].speed;
}

SPRINGWORKS_API double getSpeedLimit(const int index) {
	Route::vertex_descriptor vd = index;
	return route[*out_edges(vd, route).first].speed_limit;
}

SPRINGWORKS_API std::vector<int> getSpeedingWaypointIndices() {
	std::vector<int> ret;
	Route::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(route); ei != ei_end; ++ei) {
		const int index = source(*ei, route);
		if (route[*ei].isSpeeding) { ret.push_back(index); }
	}
	return ret;
}

SPRINGWORKS_API std::string getTimestamp(const int index) {
	Route::vertex_descriptor vd = index;
	return boost::posix_time::to_iso_extended_string(route[vd].timestamp);
}

SPRINGWORKS_API int getSecondsBetweenWaypoints(const int start, const int end) {
	Route::vertex_descriptor vd_start = start;
	Route::vertex_descriptor vd_end = end;
	return (route[vd_end].timestamp - route[vd_start].timestamp).total_seconds();
}
