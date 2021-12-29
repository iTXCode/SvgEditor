#ifndef DRAWLINE_H
#define DRAWLINE_H
#pragma once
#include "shapebase.h"

class KDrawLine : public ShapeBase
{
public:
    KDrawLine();
    KDrawLine(QWidget* parent);
    void setShapeRect();
    void paint(QPainter &painter);
    bool containsPoint(const QPointF&);
    void writerShapeXml(QXmlStreamWriter& xmlWriter);
    void reSetShapePosition(const double& width, const double& high);
};
#endif // DRAWLINE_H
