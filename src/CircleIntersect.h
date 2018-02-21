/*
 * File:   main.cpp
 * Author: SAMARAS
 *
 * 13/10/13
 */
#ifndef CIRCLEINTERSECT_H
#define CIRCLEINTERSECT_H

#include <cstdlib>
#include <cmath>

#ifndef maxx
#define maxx(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef minn
#define minn(a,b) (((a) < (b)) ? (a) : (b))
#endif

 /*
	* Find the intersection point(s) of two circles,
	* when their centers and radiuses are given (2D).
	*/

class Point2d {
public:
	Point2d() {}
	Point2d(double x, double y)
		: X(x), Y(y) {}

	double x() const { return X; }
	double y() const { return Y; }

	/**
	 * Returns the norm of this vector.
	 * @return the norm
	*/
	double norm() const {
		return sqrt(X * X + Y * Y);
	}

	void setCoords(double x, double y) {
		X = x; Y = y;
	}

	// Print point
	//friend std::ostream& operator << ( std::ostream& s, const Point2d& p )  {
		//s << p.x() << " " << p.y();
		//return s;
	//}
private:
	double X;
	double Y;
};

class Circle {
public:
	/**
	 * @param R - radius
	 * @param C - center
	 */
	Circle(double R, Point2d& C)
		: r(R), c(C) {}

	/**
	 * @param R - radius
	 * @param X - center's x coordinate
	 * @param Y - center's y coordinate
	 */
	Circle(double R, double X, double Y)
		: r(R), c(X, Y) {}

	Circle(double R, double Ctocka[2])
		: r(R), c(Ctocka[0], Ctocka[1]) {}

	Point2d getC() const { return c; }
	double getR() const { return r; }

	size_t intersect(const Circle& C2, Point2d& i1, Point2d& i2) {
		// distance between the centers
		double d = Point2d(c.x() - C2.c.x(),
			c.y() - C2.c.y()).norm();

		// find number of solutions
		if (d > r + C2.r) // circles are too far apart, no solution(s)
		{
			//Serial.println("Circles are too far apart!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			return 0;
		}
		else if (d == 0 && r == C2.r) // circles coincide
		{
			//Serial.println("Circles coincide!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			return 0;
		}
		// one circle contains the other 
		else if (d + minn(r, C2.r) < maxx(r, C2.r))
		{
			//Serial.println("One circle contains the other!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			return 0;
		}
		else
		{
			double a = (r*r - C2.r*C2.r + d*d) / (2.0*d);
			double h = sqrt(r*r - a*a);

			// find p2
			Point2d p2(c.x() + (a * (C2.c.x() - c.x())) / d,
				c.y() + (a * (C2.c.y() - c.y())) / d);

			// find intersection points p3
			i1.setCoords(p2.x() + (h * (C2.c.y() - c.y()) / d),
				p2.y() - (h * (C2.c.x() - c.x()) / d)
			);
			i2.setCoords(p2.x() - (h * (C2.c.y() - c.y()) / d),
				p2.y() + (h * (C2.c.x() - c.x()) / d)
			);

			if (d == r + C2.r)
				//std::cout << "they touch each other!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
				return 1;
			return 2;
		}
	}

	// Print circle
	//friend std::ostream& operator << ( std::ostream& s, const Circle& C )  {
		//s << "Center: " << C.getC() << ", r = " << C.getR();
		//return s;
	//}
private:
	// radius
	double r;
	// center
	Point2d c;

};
#endif
