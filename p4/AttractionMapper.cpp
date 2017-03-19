#include "provided.h"
#include "MyMap.h"
#include <algorithm>
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> m_am;

};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	for (unsigned int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment temp;
		if (ml.getSegment(i, temp))
		{
			int count = temp.attractions.size();
			for (int j = 0; j < count; j++)
			{
				string temp_name = temp.attractions[j].name;
				transform(temp_name.begin(), temp_name.end(), temp_name.begin(), ::toupper);
				m_am.associate(temp_name, temp.attractions[j].geocoordinates);
			}
		}
	}

}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	string temp_name = attraction;
	transform(temp_name.begin(), temp_name.end(), temp_name.begin(), ::toupper);
	const GeoCoord*temp = m_am.find(temp_name);
	if (temp == nullptr)
		return false;
	gc = *temp;
	return true;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}