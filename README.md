# springworks

Hi Nicklas,

Here is my answer to your waypoints challenge. Its in c++ and you would need to compile it to test it.
I changed it from visual studio to use CMake. You will need to do a `brew install cmake` first. 
Dependencies are rapidJSON (linked git submodule, just add --recursive to git clone), boost (if you dont have in standard place, best do a `brew install boost` or a `sudo port install boost`)
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
