#include "kdrawtriangle.h"

KDrawTriangle::KDrawTriangle()
{

}

KDrawTriangle::KDrawTriangle(QWidget* parent)
{
	this->setParent(parent);
	m_shapeStyle = ShapeStyle::TRIANGLE;
}

void KDrawTriangle::paint(QPainter& painter)
{
	if (m_startPoint.x() == m_endPoint.x())
		m_endPoint.setX(m_endPoint.x() + 1);
	if (m_startPoint.y() == m_endPoint.y())
		m_endPoint.setY(m_endPoint.y() + 1);

	m_triangeContainPoint[0] = QPoint((m_startPoint.x() + m_endPoint.x()) / 2, m_startPoint.y());
	m_triangeContainPoint[1] = QPoint(m_startPoint.x(), m_endPoint.y());
	m_triangeContainPoint[2] = QPoint(m_endPoint.x(), m_endPoint.y());

	QPen pen;
	pen.setColor(m_penColor);
	pen.setWidth(m_strokeWidth);
	pen.setJoinStyle(Qt::MiterJoin);
	painter.setPen(pen);

	QPolygon triangle;
	painter.setBrush(m_brushColor);
	triangle.setPoints(3, m_triangeContainPoint[0].x(), m_triangeContainPoint[0].y()
		, m_triangeContainPoint[1].x(), m_triangeContainPoint[1].y()
		, m_triangeContainPoint[2].x(), m_triangeContainPoint[2].y());
	painter.drawConvexPolygon(triangle);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
}

bool KDrawTriangle::containsPoint(const QPointF& point)
{
	QPolygon hex(m_triangeContainPoint);
	bool isFlag = false;
	if (hex.containsPoint(QPoint(point.x() / m_stretchRate, point.y() / m_stretchRate), Qt::WindingFill))
	{
		isFlag = true;
	}
	return isFlag;
}

void KDrawTriangle::setShapeRect()
{
	m_xmlRect = QRectF(m_startPoint, m_endPoint);
	if (m_xmlRect.width() == 0 || m_xmlRect.height() == 0)
		m_xmlRect.adjust(-1, -1, 1, 1);
	m_shapeRect = QRectF(m_startPoint, m_endPoint);
	if (m_shapeRect.width() == 0 || m_shapeRect.height() == 0)
		m_shapeRect.adjust(-1, -1, 1, 1);
	m_multiSelectRect = QRectF(m_startPoint,  m_endPoint).normalized();
	m_multiSelectRect.adjust(-(5+m_strokeWidth), -(5 + m_strokeWidth), 5 + m_strokeWidth, 5 + m_strokeWidth);
	adjustShapeRect();
}

void KDrawTriangle::reSetShapePosition(const double& width, const double& high)
{
	m_startPoint = QPoint(m_startPoint.x() + width, m_startPoint.y() + high);
	m_endPoint = QPoint(m_endPoint.x() + width, m_endPoint.y() + high);
	setShapeRect();
}

void KDrawTriangle::writerShapeXml(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement("path");
	QString pointPath = QString("M %1 %2").arg(m_triangeContainPoint[0].x()).arg(m_triangeContainPoint[0].y());
	for (int i = 1; i < m_triangeContainPoint.size(); i++)
	{
		pointPath += QString(" L %1 %2").arg(m_triangeContainPoint[i].x()).arg(m_triangeContainPoint[i].y());
	}
	pointPath += QString(" L %1 %2 z").arg(m_triangeContainPoint[0].x()).arg(m_triangeContainPoint[0].y());

	xmlWriter.writeAttribute("d", pointPath);
	QString penColor = getRgbaFromQColor(m_penColor);
	xmlWriter.writeAttribute("stroke", penColor);
	xmlWriter.writeAttribute("stroke-width", QString::number(m_strokeWidth));
	QString brushColor = getRgbaFromQColor(m_brushColor);
	xmlWriter.writeAttribute("fill", brushColor);
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("point_station");
	xmlWriter.writeAttribute("y2", QString::number(m_xmlRect.topLeft().y()));
	xmlWriter.writeAttribute("x2", QString::number(m_xmlRect.topLeft().x()));
	xmlWriter.writeAttribute("y1", QString::number(m_xmlRect.bottomRight().y()));
	xmlWriter.writeAttribute("x1", QString::number(m_xmlRect.bottomRight().x()));
	xmlWriter.writeAttribute("stroke", penColor);
	xmlWriter.writeAttribute("stroke-width", QString::number(m_strokeWidth));
	xmlWriter.writeAttribute("fill", brushColor);
	xmlWriter.writeEndElement();
}
