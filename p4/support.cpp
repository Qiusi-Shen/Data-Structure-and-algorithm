#include "provided.h"
#include "support.h"

bool operator< (const GeoCoord &g1, const GeoCoord &g2)
{
	if (g1.latitude != g2.latitude)
		return (g1.latitude < g2.latitude);
	else
		return (g1.longitude < g2.longitude);
}

bool operator!= (const GeoCoord &g1, const GeoCoord &g2)
{
	return !(g1 == g2);
}

bool operator== (const GeoCoord &g1, const GeoCoord &g2)
{
	if (g1.latitude == g2.latitude
		&& g1.longitude == g2.longitude)
		return true;
	else
		return false;
}