#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
#include <vector>
#include <stack>
#include <algorithm> 
#include<queue>
#include<functional>

using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	string getTurnSide(GeoSegment g1, GeoSegment g2) const;
	string getDirection(GeoCoord A, GeoCoord B) const;
	MapLoader m_ml;
};

bool operator>(const pair<double, GeoCoord> &operand1, const pair<double, GeoCoord> &oprand2)
{
	return operand1.first > oprand2.first;
}

string NavigatorImpl::getTurnSide(GeoSegment g1, GeoSegment g2) const
{
	if (angleBetween2Lines(g1, g2) < 180)
	{
		return "left";
	}
	else
		return "right";
	
}

string NavigatorImpl::getDirection(GeoCoord A, GeoCoord B) const
{
	string s;
	double angle = angleOfLine(GeoSegment(A, B));
	if (angle <= 22.5) return "east";
	if (angle <= 67.5) return "northeast";
	if (angle <= 112.5) return "north";
	if (angle <= 157.5) return "northwest";
	if (angle <= 202.5) return "west";
	if (angle <= 247.5) return "southwest";
	if (angle <= 292.5) return "south";
	if (angle <= 337.5) return "southeast";
	if (angle <= 360)   return "east";

	return "ERROR: BAD ANGLE";
}
NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	if (m_ml.load(mapFile))
		return true;

	return false;
}


NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	MyMap<GeoCoord, GeoCoord> path;
	priority_queue <pair<double, GeoCoord>, vector<pair<double, GeoCoord>>, greater<pair<double, GeoCoord>> > pq;
	MyMap<GeoCoord, NavSegment>  trackSeg;   //GC2NavSegment; tells us which navigation segment led us to this node
	stack<GeoCoord> pathInStack;
	// helps with trackback since trackback will give
	// us GeoCoord's in reverse order than what we want
	directions.clear();
	SegmentMapper sMap;
	sMap.init(m_ml);

	AttractionMapper aMap;
	aMap.init(m_ml);

	GeoCoord gcStart, gcEnd;
	if (!aMap.getGeoCoord(start, gcStart)) return NAV_BAD_SOURCE;
	if (!aMap.getGeoCoord(end, gcEnd)) return NAV_BAD_DESTINATION;

	pq.push(make_pair(distanceEarthKM(gcStart, gcEnd), gcStart));

	path.associate(gcStart, gcStart);  // so we know it is visited!

	while (pq.size()>0)
	{
		GeoCoord exploring = pq.top().second;
		pq.pop();

		// Check if the exploring is the gcEnd
		if (exploring == gcEnd) 
		{
			while (exploring != gcStart)
			{
				pathInStack.push(exploring);
				exploring = *(path.find(exploring));
			}
			while (!pathInStack.empty())
			{
				NavSegment nextSeg = *(trackSeg.find(pathInStack.top()));
				if (directions.size() > 0
					&& directions[directions.size() - 1].m_streetName != nextSeg.m_streetName)
				{
					directions.push_back(NavSegment(getTurnSide(directions[directions.size() - 1].m_geoSegment,
						nextSeg.m_geoSegment),
						nextSeg.m_streetName));
				}
				directions.push_back(nextSeg);
				pathInStack.pop();
			}
			return NAV_SUCCESS;
		}  // end of done.
		   //
		vector<StreetSegment> connections = sMap.getSegments(exploring);
		for (unsigned int counter = 0; counter < connections.size(); counter++)
		{
			vector<string> toDoStreatNames;
			vector<GeoCoord> toDoGeoCoords;

			// is the end visited?
			if (path.find(connections[counter].segment.end) == nullptr && connections[counter].segment.end != exploring)
			{
				toDoGeoCoords.push_back(connections[counter].segment.end);
				toDoStreatNames.push_back(connections[counter].streetName);
			}

			// is the start visited?
			if (path.find(connections[counter].segment.start) == nullptr && connections[counter].segment.start != exploring)
			{
				toDoGeoCoords.push_back(connections[counter].segment.start);
				toDoStreatNames.push_back(connections[counter].streetName);
			}

			// which attractions were not visited?
			for (unsigned int i = 0; i < connections[counter].attractions.size(); i++)
				if (path.find(connections[counter].attractions[i].geocoordinates) == nullptr
					&& connections[counter].attractions[i].geocoordinates != exploring)
				{
					toDoGeoCoords.push_back(connections[counter].attractions[i].geocoordinates);
					toDoStreatNames.push_back(connections[counter].streetName);
				}

			// add all unvisited GeoCoord's to heap
			for (unsigned int i = 0; i < toDoGeoCoords.size(); i++)
			{
				pq.push(make_pair(distanceEarthKM(toDoGeoCoords[i], gcEnd), toDoGeoCoords[i]));
				path.associate(toDoGeoCoords[i], exploring);

				trackSeg.associate(toDoGeoCoords[i],
					NavSegment(getDirection(exploring, toDoGeoCoords[i]), toDoStreatNames[i],
						distanceEarthMiles(toDoGeoCoords[i], exploring), GeoSegment(exploring, toDoGeoCoords[i])));  
			}
		}
	}
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}