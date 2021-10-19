#pragma once

#include "math.h"

/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

inline bool equivalent(float lhs,float rhs,float epsilon=1e-6)
  { float delta = rhs-lhs; return delta<0.0f?delta>=-epsilon:delta<=epsilon; }

/** return true if double lhs and rhs are equivalent,
  * meaning that the difference between them is less than an epsilon value
  * which defaults to 1e-6.
*/
inline bool equivalent(double lhs,double rhs,double epsilon=1e-6)
  { double delta = rhs-lhs; return delta<0.0?delta>=-epsilon:delta<=epsilon; }



const double WGS_84_RADIUS_EQUATOR = 6378137.0;
const double WGS_84_RADIUS_POLAR = 6356752.3142;

/** EllipsoidModel encapsulates the ellipsoid used to model astronomical bodies,
 * such as sun, planets, moon etc.
 * All distance quantities (i.e. heights + radius) are in meters,
 * and latitude and longitude are in radians.*/
class EllipsoidModel
{
    public:

        /** WGS_84 is a common representation of the earth's spheroid */
        EllipsoidModel(double radiusEquator = WGS_84_RADIUS_EQUATOR,
                       double radiusPolar = WGS_84_RADIUS_POLAR):
            _radiusEquator(radiusEquator),
            _radiusPolar(radiusPolar) { computeCoefficients(); }

        EllipsoidModel(const EllipsoidModel& et):
    
            _radiusEquator(et._radiusEquator),
            _radiusPolar(et._radiusPolar) { computeCoefficients(); }

       

        void setRadiusEquator(double radius) { _radiusEquator = radius; computeCoefficients(); }
        double getRadiusEquator() const { return _radiusEquator; }

        void setRadiusPolar(double radius) { _radiusPolar = radius; computeCoefficients(); }
        double getRadiusPolar() const { return _radiusPolar; }

        inline void convertLatLongHeightToXYZ(double latitude, double longitude, double height,
                                              double& X, double& Y, double& Z) const;

        inline void convertXYZToLatLongHeight(double X, double Y, double Z,
                                              double& latitude, double& longitude, double& height) const;


        // Convenience method for determining if EllipsoidModel is a stock WGS84 ellipsoid
        inline bool isWGS84() const {return(_radiusEquator == WGS_84_RADIUS_EQUATOR && _radiusPolar == WGS_84_RADIUS_POLAR);}

        // Compares two EllipsoidModel by comparing critical internal values.
        // Ignores _eccentricitySquared since it's just a cached value derived from
        // the _radiusEquator and _radiusPolar members.
        friend bool operator == ( const EllipsoidModel & e1, const EllipsoidModel& e2) {return(e1._radiusEquator == e2._radiusEquator && e1._radiusPolar == e2._radiusPolar);}

		// 根据两点经纬高计算两点之间的距离 单位--deg
		double distance( double lon1, double lat1, double alt1, double lon2, double lat2, double alt2);
    protected:

        void computeCoefficients()
        {
            double flattening = (_radiusEquator-_radiusPolar)/_radiusEquator;
            _eccentricitySquared = 2*flattening - flattening*flattening;
        }

        double _radiusEquator;
        double _radiusPolar;
        double _eccentricitySquared;

};

