#include "include/shapes/polyline.h"



// Getter
QPoint Polyline::getLastPoint(){
	return polyline.last();
}


// Constructor
Polyline::Polyline(QPen pen, QPoint firstPoint) : AbstractShape(pen){
	this->polyline.append(firstPoint);
}



// Destructor
Polyline::~Polyline(){

}



// Methods

void Polyline::draw(QPainter *painter){
	painter->setPen(pen);

	// If it's just a point...
	if(polyline.count() == 1)
	{
		painter->drawPoint(polyline.first());
	}
	// If it's a line...
	else
	{
		painter->drawPolyline(polyline);
	}
}

void Polyline::addPoint(QPoint newPoint)
{
	polyline.append(newPoint);
}
