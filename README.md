# springworks

Hi Nicklas,

Here is my answer to your waypoints challenge. Its in c++ and you would need to compile it to test it.
To compile you will need to configure this manually, as it is the visual studio project template. 
The paths for includes and linking are hardcoded. 
This is visual studio 2013 (v120), and you will need boost date-time as the only external dependency.
http://www.boost.org/doc/libs/1_61_0/doc/html/date_time.html
Let me know if you have trouble setting it up. I can also just send you precompiled if you like.

Code-Descriptive Notes:
=======================
I broke the waypoint JSON object into essentially two structs: waypoint and segment.
I used a boost graph with waypoints as vertices and segments as edges. http://www.boost.org/doc/libs/1_61_0/libs/graph/doc/index.html
The waypoint struct holds time and position. The segment struct holds everything else (speed..etc)
I assume speed and speedlimit is maintained constant until the next waypoint. The graph is directed, in sequence (chrono order).

Rationale
=========
Theres a bunch of error and a whole study of statistics that might be applicable to find a very precise answer...
I just took a fairly simple calculation method because I think the problem is not defined enough for the others...
plus they take too much time to code.
You can have more than one edge emenating from a vertex, so the point is to be extensible for various fast operations on the data
(e.g.) distance calculation could skip idling waypoints, etc.

Let me know if you have questions, bon weekend!
