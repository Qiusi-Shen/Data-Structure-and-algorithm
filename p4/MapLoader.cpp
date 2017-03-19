#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "provided.h"
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment*> m_segments;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	size_t size = m_segments.size();
	for (size_t i = 0; i < size; i++)
		delete m_segments[i];
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream myfile(mapFile);
	if (!myfile)
		return false;

	string sName;

	while (getline(myfile, sName))
	{
		string slat, slon, elat, elon;

		getline(myfile, slat, ',');
		getline(myfile, slon, ' ');
		getline(myfile, elat, ',');
		getline(myfile, elon);

		GeoSegment gSeg(GeoCoord(slat, slon), GeoCoord(elat, elon));

		StreetSegment* ss = new StreetSegment;
		ss->streetName = sName;
		ss->segment = gSeg;

		int count; 
		myfile >> count;
		myfile.ignore(10000, '\n');
		while (count != 0)
		{
			string attr, alat, alon;
			getline(myfile, attr, '|');
			getline(myfile, alat, ',');
			getline(myfile, alon);

			Attraction a; 
			a.name = attr;
			a.geocoordinates = GeoCoord(alat, alon);

			ss->attractions.push_back(a);
			count--;
		}
		m_segments.push_back(ss);
	}
	return true;  // This compiles, but may not be correct
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_segments.size(); // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum > m_segments.size() - 1)
		return false;

	seg = (*m_segments[segNum]);
	return true;
	// This compiles, but may not be correct
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
