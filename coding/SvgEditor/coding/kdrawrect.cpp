#include "kdrawrect.h"

KDrawRect::KDrawRect()
{
}

KDrawRect::KDrawRect(QWidget* parent)
{
	this->setParent(parent);
	m_shapeStyle = ShapeStyle::RECTS;
}

void KDrawRect::paint(QPainter &painter)
{
	QPen pen;
	pen.setColor(m_penColor);
	pen.setWidth(m_strokeWidth);
	pen.setJoinStyle(Qt::MiterJoin);
	painter.setPen(pen);
	painter.setBrush(m_brushColor);
	painter.drawRect(m_xmlRect);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
}

void KDrawRect::setShapeRect()
{
	m_xmlRect = QRectF(m_startPoint, m_endPoint);
	if (m_xmlRect.width() == 0 || m_xmlRect.height() == 0)
		m_xmlRect.adjust(-1, -1, 1, 1);
	m_shapeRect = QRectF(m_startPoint, m_endPoint);
	if (m_shapeRect.width() == 0 || m_shapeRect.height() == 0)
		m_shapeRect.adjust(-1, -1, 1, 1);

	m_multiSelectRect = QRectF(m_startPoint , m_endPoint ).normalized();
	m_multiSelectRect.adjust(-(5 + m_strokeWidth), -(5 + m_strokeWidth), 5 + m_strokeWidth, 5 + m_strokeWidth);
	adjustShapeRect();
}

bool KDrawRect::containsPoint(const QPointF& point)
{
	m_xmlRect = QRectF(m_startPoint,m_endPoint);
	m_xmlRect.adjust(-m_strokeWidth, -m_strokeWidth, +m_strokeWidth, +m_strokeWidth);
	return m_xmlRect.contains(point / m_stretchRate);
}

void KDrawRect::reSetShapePosition(const double& width, const double& high)
{
	m_startPoint = QPoint(m_startPoint.x() + width, m_startPoint.y() + high);
	m_endPoint = QPoint(m_endPoint.x() + width, m_endPoint.y() + high);
    setShapeRect();
}

void KDrawRect::writerShapeXml(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement("rect");
	if (m_xmlRect.height() < 0)
		xmlWriter.writeAttribute("height", QString::number(abs(m_xmlRect.height())));
	else
		xmlWriter.writeAttribute("height", QString::number(m_xmlRect.height()));
	if (m_xmlRect.width() < 0)
		xmlWriter.writeAttribute("width", QString::number(abs(m_xmlRect.width())));
	else
		xmlWriter.writeAttribute("width", QString::number(m_xmlRect.width()));
	
	if (m_xmlRect.height() == 0)
		xmlWriter.writeAttribute("height", QString::number(1));
	if (m_xmlRect.width() == 0)
		xmlWriter.writeAttribute("width", QString::number(1));

	xmlWriter.writeAttribute("y", QString::number(m_xmlRect.topLeft().y()));
	xmlWriter.writeAttribute("x", QString::number(m_xmlRect.topLeft().x()));
	QString penColor = getRgbaFromQColor(m_penColor);
	xmlWriter.writeAttribute("stroke", penColor);
	xmlWriter.writeAttribute("stroke-width", QString::number(m_strokeWidth));
	QString brushColor = getRgbaFromQColor(m_brushColor);
	xmlWriter.writeAttribute("fill", brushColor);
	xmlWriter.writeEndElement();
}



