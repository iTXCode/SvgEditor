#ifndef KDRAWHEXAGON_H
#define KDRAWHEXAGON_H
#pragma once
#include <QPainterPath>
#include <QPolygon>
#include "shapebase.h"

class KDrawHexagon : public ShapeBase
{
public:
	KDrawHexagon();
	KDrawHexagon(QWidget* parent);
	void paint(QPainter& painter);
	bool containsPoint(const QPointF&);
	void setShapeRect();
	void reSetShapePosition(const double& width, const double& high);
	void writerShapeXml(QXmlStreamWriter& xmlWriter);

private:
	QVector<QPoint> m_hexContainPoint{6,QPoint()};
	QRectF m_xmlRect = QRectF();
};

#endif // KDRAWHEXAGON_H
