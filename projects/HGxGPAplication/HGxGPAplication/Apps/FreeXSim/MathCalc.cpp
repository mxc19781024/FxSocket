#include "MathCalc.h"

// ΆΘ΅½»ʽΆΘ
double DegreeToRadius(double dDegree)
{
	// 
	return dDegree / 180.0 * M_PI;
}

double EllipsoidModel::distance( double lon1, double lat1, double alt1, double lon2, double lat2, double alt2 )
{
	double dX1,dY1,dZ1,dX2,dY2,dZ2;
	
	convertLatLongHeightToXYZ(DegreeToRadius(lat1),DegreeToRadius(lon1),alt1,dX1,dY1,dZ1);
	convertLatLongHeightToXYZ(DegreeToRadius(lat2),DegreeToRadius(lon2),alt2,dX2,dY2,dZ2);

	double dSqut = (dX1-dX2)*(dX1-dX2)+ (dY1 - dY2)*(dY1 - dY2) + (dZ1 - dZ2) * (dZ1 - dZ2);

	return sqrt(dSqut);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implement inline methods.
//
inline void EllipsoidModel::convertLatLongHeightToXYZ(double latitude, double longitude, double height,
	double& X, double& Y, double& Z) const
{
	// for details on maths see http://www.colorado.edu/geography/gcraft/notes/datum/gif/llhxyz.gif
	double sin_latitude = sin(latitude);
	double cos_latitude = cos(latitude);
	double N = _radiusEquator / sqrt( 1.0 - _eccentricitySquared*sin_latitude*sin_latitude);
	X = (N+height)*cos_latitude*cos(longitude);
	Y = (N+height)*cos_latitude*sin(longitude);
	Z = (N*(1-_eccentricitySquared)+height)*sin_latitude;
}


inline void EllipsoidModel::convertXYZToLatLongHeight(double X, double Y, double Z,
	double& latitude, double& longitude, double& height) const
{
	// http://www.colorado.edu/geography/gcraft/notes/datum/gif/xyzllh.gif

	//h00017
	double p = sqrt(X*X + Y*Y);
	double theta = atan2(Z*_radiusEquator , (p*_radiusPolar));
	double eDashSquared = (_radiusEquator*_radiusEquator - _radiusPolar*_radiusPolar)/
		(_radiusPolar*_radiusPolar);

	double sin_theta = sin(theta);
	double cos_theta = cos(theta);

	double pe = p - _eccentricitySquared*_radiusEquator*cos_theta*cos_theta*cos_theta;
	if(equivalent(pe,0.0))
	{
		if(equivalent(Z,0))
			latitude = 0.0;
		else if(Z > 0)
			latitude =  M_PI/ 2.0;
		else
			latitude = -M_PI / 2.0;
	}   
	else
		latitude = atan( (Z + eDashSquared*_radiusPolar*sin_theta*sin_theta*sin_theta) /
		pe );

	longitude = atan2(Y,X);

	double sin_latitude = sin(latitude);
	double N = _radiusEquator / sqrt( 1.0 - _eccentricitySquared*sin_latitude*sin_latitude);

	height = p/cos(latitude) - N;
	//h00017

	//double p = sqrt(X*X + Y*Y);
	//double theta = atan2(Z*_radiusEquator , (p*_radiusPolar));
	//double eDashSquared = (_radiusEquator*_radiusEquator - _radiusPolar*_radiusPolar)/
	//                      (_radiusPolar*_radiusPolar);

	//double sin_theta = sin(theta);
	//double cos_theta = cos(theta);

	//latitude = atan( (Z + eDashSquared*_radiusPolar*sin_theta*sin_theta*sin_theta) /
	//                 (p - _eccentricitySquared*_radiusEquator*cos_theta*cos_theta*cos_theta) );
	//longitude = atan2(Y,X);

	//double sin_latitude = sin(latitude);
	//double N = _radiusEquator / sqrt( 1.0 - _eccentricitySquared*sin_latitude*sin_latitude);

	//height = p/cos(latitude) - N;
}

