#include "kdrawline.h"
#include <QPainter>
#include <math.h>

KDrawLine::KDrawLine()
{
	
}

KDrawLine::KDrawLine(QWidget* parent)
{
	this->setParent(parent);
	m_shapeStyle = ShapeStyle::LINE;
}

void KDrawLine::paint(QPainter &painter)
{
	QPen pen;
	pen.setWidth(m_strokeWidth);
	pen.setColor(m_penColor);
	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.setBrush(m_brushColor);
	painter.drawLine(m_startPoint, m_endPoint);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
}

bool KDrawLine::containsPoint(const QPointF& point)
{
	QTransform transFrom;
	QPointF pt(point/m_stretchRate);
	QLineF tmpLine(m_startPoint, m_endPoint);
	double angle = -atan2(tmpLine.dy(), tmpLine.dx()) * (45 / atan(1.0));
	transFrom.rotate(angle);
	tmpLine = tmpLine * transFrom;
	QPointF p = pt * transFrom;
	QRectF realArea(tmpLine.p1() + QPointF(-m_strokeWidth, -m_strokeWidth), tmpLine.p2() + QPointF(m_strokeWidth, m_strokeWidth));
	return realArea.contains(p);
}

void KDrawLine::reSetShapePosition(const double& width, const double& high)
{
	m_startPoint = QPoint(m_startPoint.x() + width, m_startPoint.y() + high);
	m_endPoint = QPoint(m_endPoint.x() + width, m_endPoint.y() + high);
}

void KDrawLine::setShapeRect()
{
	m_shapeRect = QRectF(m_startPoint, m_endPoint);
	adjustShapeRect();
	m_multiSelectRect = QRectF(m_startPoint, m_endPoint).normalized();
	m_multiSelectRect.adjust(-(5 + m_strokeWidth), -(5 + m_strokeWidth), 5 + m_strokeWidth, 5 + m_strokeWidth);
}

void KDrawLine::writerShapeXml(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement("line");
	xmlWriter.writeAttribute("y2", QString::number(m_startPoint.y()));
	xmlWriter.writeAttribute("x2", QString::number(m_startPoint.x()));
	xmlWriter.writeAttribute("y1", QString::number(m_endPoint.y()));
	xmlWriter.writeAttribute("x1", QString::number(m_endPoint.x()));
	QString color = getRgbaFromQColor(m_penColor);
	xmlWriter.writeAttribute("stroke", color);
	xmlWriter.writeAttribute("stroke-width", QString::number(m_strokeWidth));
	xmlWriter.writeEndElement();
}
