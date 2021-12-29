#ifndef DRAWFREELINE_H
#define DRAWFREELINE_H
#pragma once
#include "shapebase.h"
#include <QVector>
#include <QList>

enum SizeStatus : uint8_t
{
	DEFAULT= 0,
	WIDTH,
	HIGH
};

class KDrawFreeLine : public ShapeBase
{
public:
	KDrawFreeLine();
	KDrawFreeLine(QWidget* parent);
	QRectF getXmlRect();
	void setShapeRect();
	void setStartAndEndPoint();
	void paint(QPainter& painter);
	bool containsPoint(const QPointF&);
	void reSetEveryPoint(QRectF& oldRect);
	void insertFileLinePoint(const QPointF&);
	void writerShapeXml(QXmlStreamWriter& xmlWriter);
	void copyOtherFreeLinePoint(ShapeBase* other);
	void reSetShapePosition(const double& width, const double& high);
	uint8_t getSizeStatus();
private:
	QRectF m_xmlRect = QRectF();
	QVector<QPointF> m_freeLinePointContainer = QVector<QPointF>();
	SizeStatus m_status = SizeStatus::DEFAULT;
};

#endif // DRAWFREELINE_H


