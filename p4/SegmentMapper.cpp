#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment*>> m_sm;
	vector<StreetSegment*> m_sp;
	void insert(GeoCoord gc, StreetSegment* seg);
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
	unsigned size = m_sp.size();
	for (unsigned i = 0; i < size; i++)
	{
		delete m_sp[i];
	}
}


void SegmentMapperImpl::insert(GeoCoord gc, StreetSegment* seg)
{
	vector<StreetSegment*>* Add;
	Add = m_sm.find(gc);

	if (Add != nullptr)
	{
		Add->push_back(seg);
	}
	else
	{
		m_sm.associate(gc, { seg });
	}
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (unsigned int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment street; 
		ml.getSegment(i, street);
		StreetSegment* segPtr = new StreetSegment(street);
		m_sp.push_back(segPtr);
		insert(street.segment.start, segPtr);
		insert(street.segment.end, segPtr);

		for (unsigned int j = 0; j < street.attractions.size(); j++)
		{
			GeoCoord att = street.attractions[j].geocoordinates;
			if (att != street.segment.start && att != street.segment.end)
			{
				insert(att, segPtr);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> seg;

	const vector<StreetSegment*>* vecPtr = m_sm.find(gc);
	if (vecPtr != nullptr)
	{
		for (unsigned int i = 0; i != vecPtr->size(); i++)
			seg.push_back(*(*vecPtr)[i]);
	}
	return seg;  // This compiles, but may not be correct
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
