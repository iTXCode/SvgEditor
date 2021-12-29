#include "kdrawellipse.h"

KDrawEllipse::KDrawEllipse()
{
}

KDrawEllipse::KDrawEllipse(QWidget* parent)
{
	this->setParent(parent);
	m_shapeStyle = ShapeStyle::ELLIPSE;
}

void KDrawEllipse::paint(QPainter &painter)
{
	if (m_startPoint.x() == m_endPoint.x())
		m_endPoint.setX(m_endPoint.x() + 1);
	if (m_startPoint.y() == m_endPoint.y())
		m_endPoint.setY(m_endPoint.y() + 1);

	QPen pen;
	pen.setColor(m_penColor);
	pen.setWidth(m_strokeWidth);
	pen.setJoinStyle(Qt::MiterJoin);
	painter.setPen(pen);
	painter.setBrush(m_brushColor);
	painter.drawEllipse(m_startPoint.x(), m_startPoint.y(),
		m_endPoint.x() - m_startPoint.x(), m_endPoint.y() - m_startPoint.y());
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
}

bool KDrawEllipse::containsPoint(const QPointF& point)
{
	QPointF pt = point / m_stretchRate;
    m_width = abs(m_endPoint.x() - m_startPoint.x())/ 2;
    m_high = abs(m_endPoint.y() - m_startPoint.y()) /2;
    QPoint centerPoint = QPoint((m_startPoint.x() + m_endPoint.x())/2,(m_startPoint.y() + m_endPoint.y())/2);
    return pow(pt.x() - centerPoint.x(), 2) / pow(m_width, 2) + pow(pt.y() - centerPoint.y(), 2) / pow(m_high, 2) <= 1;
}

void KDrawEllipse::reSetShapePosition(const double& width, const double& high)
{   
	m_startPoint = QPoint(m_startPoint.x() + width, m_startPoint.y() + high);
	m_endPoint = QPoint(m_endPoint.x() + width, m_endPoint.y() + high);
}

void KDrawEllipse::setShapeRect()
{
	m_xmlRect = QRectF(m_startPoint, m_endPoint);
	if (m_xmlRect.width() == 0 || m_xmlRect.height() == 0)
		m_xmlRect.adjust(-1, -1, 1, 1);
	m_shapeRect = QRectF(m_startPoint, m_endPoint);
	if (m_shapeRect.width() == 0 || m_shapeRect.height() == 0)
		m_shapeRect.adjust(-1, -1, 1, 1);
	m_multiSelectRect = QRectF(m_startPoint, m_endPoint).normalized();
	m_multiSelectRect.adjust(-(5 + m_strokeWidth), -(5 + m_strokeWidth), 5 + m_strokeWidth, 5 + m_strokeWidth);
	adjustShapeRect();
}

void KDrawEllipse::writerShapeXml(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement("ellipse");
	xmlWriter.writeAttribute("ry", QString::number(abs(m_endPoint.y() - m_startPoint.y()) / 2));
	xmlWriter.writeAttribute("rx", QString::number(abs(m_endPoint.x() - m_startPoint.x()) / 2));
	xmlWriter.writeAttribute("cy", QString::number(m_xmlRect.center().y()));
	xmlWriter.writeAttribute("cx", QString::number(m_xmlRect.center().x()));
	QString penColor = getRgbaFromQColor(m_penColor);
	xmlWriter.writeAttribute("stroke", penColor);
	xmlWriter.writeAttribute("stroke-width", QString::number(m_strokeWidth));
	QString brushColor = getRgbaFromQColor(m_brushColor);
	xmlWriter.writeAttribute("fill", brushColor);
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("point_station");
	xmlWriter.writeAttribute("y2", QString::number(m_startPoint.y()));
	xmlWriter.writeAttribute("x2", QString::number(m_startPoint.x()));
	xmlWriter.writeAttribute("y1", QString::number(m_endPoint.y()));
	xmlWriter.writeAttribute("x1", QString::number(m_endPoint.x()));
	xmlWriter.writeAttribute("stroke", penColor);
	xmlWriter.writeAttribute("stroke-width", QString::number(m_strokeWidth));
	xmlWriter.writeAttribute("fill", brushColor);
	xmlWriter.writeEndElement();
}
