#ifndef DRAWRECT_H
#define DRAWRECT_H
#pragma once
#include "shapebase.h"

class KDrawRect : public ShapeBase
{
public:
    KDrawRect();
    KDrawRect(QWidget* parent);
    void paint(QPainter& painter);
    bool containsPoint(const QPointF&);
    void setShapeRect();
    void reSetShapePosition(const double& width, const double& high);
    void writerShapeXml(QXmlStreamWriter& xmlWriter);
private:
    QRectF m_xmlRect = QRectF();
};

#endif // DRAWRECT_H
