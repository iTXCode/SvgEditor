#ifndef DRAWELLIPSE_H
#define DRAWELLIPSE_H
#pragma once
#include "shapebase.h"

class KDrawEllipse : public ShapeBase
{
public:
    KDrawEllipse();
    KDrawEllipse(QWidget* parent);
    void setShapeRect();
    void paint(QPainter &painter);
    bool containsPoint(const QPointF&);
    void writerShapeXml(QXmlStreamWriter& xmlWriter);
    void reSetShapePosition(const double& width, const double& high);
private:
    float m_width;
    float m_high;
    QRectF m_xmlRect = QRectF();
};

#endif // DRAWELLIPSE_H
