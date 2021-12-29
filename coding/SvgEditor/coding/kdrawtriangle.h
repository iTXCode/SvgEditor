#ifndef KDRAWTRIANGLE_H
#define KDRAWTRIANGLE_H
#pragma once
#include "shapebase.h"

class KDrawTriangle : public ShapeBase
{
public:
	KDrawTriangle();
	KDrawTriangle(QWidget* parent);
	void paint(QPainter& painter);
	bool containsPoint(const QPointF&);
	void setShapeRect();
	void reSetShapePosition(const double& width, const double& high);
	void writerShapeXml(QXmlStreamWriter& xmlWriter);
private:
	QVector<QPoint> m_triangeContainPoint{ 3,QPoint() };
	QRectF m_xmlRect = QRectF();
};

#endif // KDRAWTRIANGLE_H
