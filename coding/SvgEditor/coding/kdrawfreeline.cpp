#include "kdrawfreeline.h"

KDrawFreeLine::KDrawFreeLine()
{

}

KDrawFreeLine::KDrawFreeLine(QWidget* parent)
{
	this->setParent(parent);
	m_shapeStyle = ShapeStyle::FREELINE;
}

void KDrawFreeLine::paint(QPainter& painter)
{
	if (m_freeLinePointContainer.size() <= 1)
		return;
	QPen pen;
	pen.setColor(m_penColor);
	pen.setWidth(m_strokeWidth);
	pen.setJoinStyle(Qt::MiterJoin);
	painter.setPen(pen);
	painter.setBrush(m_brushColor);
	for (size_t i = 0; i < m_freeLinePointContainer.size() - 1; i++)
	{
		painter.drawLine(m_freeLinePointContainer[i], m_freeLinePointContainer[i + 1]);
	}
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
}

bool KDrawFreeLine::containsPoint(const QPointF& point)
{
	QPointF pt = point / m_stretchRate;
	for (int i = 0; i < m_freeLinePointContainer.size(); i++)
	{
		if (pt.x() - m_freeLinePointContainer[i].x() < 6
			&& pt.x()  - m_freeLinePointContainer[i].x() > -6
			&& pt.y() - m_freeLinePointContainer[i].y() < 6
			&& pt.y() - m_freeLinePointContainer[i].y() > -6)
			return true;
	}
	return false;
}

void KDrawFreeLine::reSetShapePosition(const double& width, const double& high)
{
	for (int i = 0; i < m_freeLinePointContainer.size(); i++)
	{
		m_freeLinePointContainer[i].rx() = m_freeLinePointContainer[i].x() + width;
		m_freeLinePointContainer[i].ry() = m_freeLinePointContainer[i].y() + high;
	}
}

uint8_t KDrawFreeLine::getSizeStatus()
{
	return m_status;
}

void KDrawFreeLine::reSetEveryPoint(QRectF& oldRect)
{
	QRectF changeRect = QRectF();
	double wRate = 0;
	double hRate = 0;
	if (m_startPoint.y() == m_endPoint.y())
		m_startPoint.setY(m_startPoint.y() + 0.1);

	if (m_startPoint.x() == m_endPoint.x())
		m_startPoint.setX(m_startPoint.x() + 0.1);
	if (m_status == SizeStatus::DEFAULT)
	{
		changeRect = QRectF(m_startPoint, m_endPoint);

		if (oldRect.width() == 0)
			oldRect.setWidth(oldRect.width() + 0.1);
		wRate = changeRect.width() / oldRect.width(); //宽度可能为0
		
		if (oldRect.height() == 0)
			oldRect.setHeight(oldRect.height() + 0.1);
		hRate = changeRect.height() / oldRect.height();

		
		for (int i = 0; i < m_freeLinePointContainer.size(); i++)
		{
			m_freeLinePointContainer[i].setX((m_freeLinePointContainer[i].x() - oldRect.topLeft().x()) * wRate + changeRect.topLeft().x());
			m_freeLinePointContainer[i].setY((m_freeLinePointContainer[i].y() - oldRect.topLeft().y()) * hRate + changeRect.topLeft().y());
			qDebug() << m_freeLinePointContainer[i];
		}
	}
	else if (m_status == SizeStatus::WIDTH)
	{//只改变高
		changeRect = QRectF(m_startPoint, m_endPoint);

		if (oldRect.height() == 0)
			oldRect.setHeight(oldRect.height() + 0.1);
		hRate = changeRect.height() / oldRect.height();

		for (int i = 0; i < m_freeLinePointContainer.size(); i++)
		{
			m_freeLinePointContainer[i].setY((m_freeLinePointContainer[i].y() - oldRect.topLeft().y()) * hRate + changeRect.topLeft().y());
		}
	}
	else if (m_status == SizeStatus::HIGH)
	{//只改变宽
		changeRect = QRectF(m_startPoint, m_endPoint);
		if (oldRect.width() == 0)
			oldRect.setWidth(oldRect.width() + 0.1);
		wRate = changeRect.width() / oldRect.width();
		
		for (int i = 0; i < m_freeLinePointContainer.size(); i++)
		{
			m_freeLinePointContainer[i].setX((m_freeLinePointContainer[i].x() - oldRect.topLeft().x()) * wRate + changeRect.topLeft().x());
		}
	}
	setShapeRect();
}

void KDrawFreeLine::setShapeRect()
{
	m_xmlRect = QRectF(m_startPoint, m_endPoint);
	//if (m_xmlRect.width() == 0)
	//{
	//	//宽度为0
	//	m_status = SizeStatus::WIDTH;
	//}
	//if (m_xmlRect.height() == 0)
	//{
	//	//宽度为0
	//	m_status = SizeStatus::HIGH;
	//}
	m_shapeRect = QRectF(m_startPoint, m_endPoint);
	adjustShapeRect();
	m_multiSelectRect = QRectF(m_startPoint, m_endPoint).normalized();
	m_multiSelectRect.adjust(-(5 + m_strokeWidth), -(5 + m_strokeWidth), 5 + m_strokeWidth, 5 + m_strokeWidth);
}

void KDrawFreeLine::copyOtherFreeLinePoint(ShapeBase* other)
{
	KDrawFreeLine* tmpPtr = dynamic_cast<KDrawFreeLine*> (other);
	this->m_freeLinePointContainer = tmpPtr->m_freeLinePointContainer;
}

void KDrawFreeLine::insertFileLinePoint(const QPointF& pos)
{
	m_freeLinePointContainer.push_back(pos);
}

void KDrawFreeLine::setStartAndEndPoint()
{
	double minX = m_freeLinePointContainer[0].x();
	double minY = m_freeLinePointContainer[0].y();
	double maxX = 0;
	double maxY = 0;

	for (int i = 0; i < m_freeLinePointContainer.size(); i++)
	{
		minX = minX < m_freeLinePointContainer[i].x() ? minX : m_freeLinePointContainer[i].x();
		minY = minY < m_freeLinePointContainer[i].y() ? minY : m_freeLinePointContainer[i].y();
		maxX = maxX > m_freeLinePointContainer[i].x() ? maxX : m_freeLinePointContainer[i].x();
		maxY = maxY > m_freeLinePointContainer[i].y() ? maxY : m_freeLinePointContainer[i].y();
	}
	m_status = SizeStatus::DEFAULT;
	if (minX == maxX)
	{
		//宽度为0
		m_status = SizeStatus::WIDTH;
		maxX++;
	}
	if (minY == maxY)
	{
		//宽度为0
		m_status = SizeStatus::HIGH;
		maxY++;
	}
	m_startPoint = QPoint(minX, minY);
	m_endPoint = QPoint(maxX, maxY);
}

QRectF KDrawFreeLine::getXmlRect()
{
	return m_xmlRect;
}

void KDrawFreeLine::writerShapeXml(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement("path");
	QString pointPath = QString("M %1 %2").arg(m_freeLinePointContainer[0].x()).arg(m_freeLinePointContainer[0].y());
	for (int i = 1; i < m_freeLinePointContainer.size(); i++)
	{
		pointPath += QString(" L %1 %2").arg(m_freeLinePointContainer[i].x()).arg(m_freeLinePointContainer[i].y());
	}
	xmlWriter.writeAttribute("d", pointPath);
	QString penColor = getRgbaFromQColor(m_penColor);
	xmlWriter.writeAttribute("stroke", penColor);
	xmlWriter.writeAttribute("stroke-width", QString::number(m_strokeWidth));
	xmlWriter.writeAttribute("fill", "none");
	xmlWriter.writeEndElement();
}
