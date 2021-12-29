#include "shapebase.h"

ShapeBase::ShapeBase()
{
	m_shapeStyle = ShapeStyle::MOUSE;
}

ShapeBase::ShapeBase(QWidget* parent)
{
	this->setParent(parent);
}

ShapeBase::~ShapeBase()
{
	qDebug() << "~ShapeBase()";
}

void ShapeBase::setStart(QPointF s)
{
	m_startPoint = s;
}

void ShapeBase::reSetStartPoint(const int& width, const int& high)
{
	m_startPoint.rx() = m_startPoint.x() + width;
	m_startPoint.ry() = m_startPoint.y() + high;
}

void ShapeBase::reSetEndPoint(const int& width, const int& high)
{
	m_endPoint.rx() = m_endPoint.x() + width;
	m_endPoint.ry() = m_endPoint.y() + high;
}

void ShapeBase::setEnd(QPointF e)
{
	m_endPoint = e;
}

void ShapeBase::setShapeColor(QColor color)
{
	m_brushColor = color;
}

void ShapeBase::setSelectedStatus(const bool staus)
{
	m_selectedStatus = staus;
}

bool ShapeBase::getSelectedStatus()
{
	return m_selectedStatus;
}

void ShapeBase::setStrokeWidth(int size)
{
	if (this!=nullptr)
		m_strokeWidth = size;
}

QRectF ShapeBase::getMultiSelectContainRect()
{
	if (m_multiSelectRect.isValid())
		return m_multiSelectRect;
	else
		return QRectF();
}

ShapeStyle ShapeBase::getGraphicalShapes()
{
	return m_shapeStyle;
}

void ShapeBase::setPenColor(QColor color)
{
	m_penColor = color;
}

QPointF ShapeBase::getStartPoint()
{
	return m_startPoint;
}

QPointF ShapeBase::getEndPoint()
{
	return m_endPoint;
}

void ShapeBase::copyOtherCharacter(ShapeBase* other)
{
	this->m_shapeStyle = other->m_shapeStyle;
	this->m_startPoint = other->m_startPoint;
	this->m_endPoint = other->m_endPoint;
	this->m_penColor = other->m_penColor;
	this->m_strokeWidth = other->m_strokeWidth;
	this->m_stretchRate = other->m_stretchRate;
	this->m_brushColor = other->m_brushColor;
}

void ShapeBase::paintOutRect(QPainter& painter)
{
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(m_shapeRect);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::blue);
	painter.drawRect(m_leftTopRect);
	painter.drawRect(m_rightTopRect);
	painter.drawRect(m_leftBottomRect);
	painter.drawRect(m_rightBottomRect);
	painter.drawRect(m_middleTopRect);
	painter.drawRect(m_middleBottomRect);
	painter.drawRect(m_leftMiddleRect);
	painter.drawRect(m_rightMiddleRect);
	painter.setBrush(Qt::NoBrush);
}

void ShapeBase::paintTopOutRect(QPainter& painter)
{
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(m_shapeRect);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::blue);
	painter.drawRect(m_middleTopRect);
	painter.drawRect(m_middleBottomRect);
	painter.setBrush(Qt::NoBrush);
}

void ShapeBase::paintLeftOutRect(QPainter& painter)
{
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(m_shapeRect);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::blue);
	painter.drawRect(m_leftMiddleRect);
	painter.drawRect(m_rightMiddleRect);
	painter.setBrush(Qt::NoBrush);
}

HitOutRect ShapeBase::getSelectedBoxNum(QPointF pos)
{
	if (m_leftTopRect.contains(pos))
		return HitOutRect::TOPLEFTRECT;
	if (m_middleTopRect.contains(pos))
		return HitOutRect::TOPMIDDLERECT;
	if (m_rightTopRect.contains(pos))
		return HitOutRect::TOPRIGHTRECT;
	if (m_leftMiddleRect.contains(pos))
		return HitOutRect::LEFTMIDDLERECT;
	if (m_rightMiddleRect.contains(pos))
		return HitOutRect::RIGHTMIDDLERECT;
	if (m_leftBottomRect.contains(pos))
		return HitOutRect::BOTTOMLEFTRECT;
	if (m_middleBottomRect.contains(pos))
		return HitOutRect::BOTTOMMIDDLERECT;
	if (m_rightBottomRect.contains(pos))
		return HitOutRect::BOTTOMRIGHTRECT;
	return HitOutRect::MOUSESTATUS;
}

QString ShapeBase::getRgbaFromQColor(QColor& color)
{
	int a = color.alpha();
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	QString retColor = "#" + QString("%1").arg(r, 2, 16, QLatin1Char('0')) +
		QString("%1").arg(g, 2, 16, QLatin1Char('0')) +
		QString("%1").arg(b, 2, 16, QLatin1Char('0')) +
		QString("%1").arg(a, 2, 16, QLatin1Char('0'));
	if (!retColor.isEmpty())
		return retColor;
	return QString();
}

void ShapeBase::adjustShapeRect()
{
	if (m_startPoint.x() < m_endPoint.x() && m_startPoint.y() < m_endPoint.y())
		m_shapeRect.adjust(-(5 + m_strokeWidth), -(5 + m_strokeWidth), +(5 + m_strokeWidth), +(5 + m_strokeWidth));
	else if (m_startPoint.x() > m_endPoint.x() && m_startPoint.y() > m_endPoint.y())
		m_shapeRect.adjust((5 + m_strokeWidth), (5 + m_strokeWidth), -(5 + m_strokeWidth), -(5 + m_strokeWidth));
	else if (m_startPoint.x() > m_endPoint.x() && m_startPoint.y() < m_endPoint.y())
		m_shapeRect.adjust((5 + m_strokeWidth), -(5 + m_strokeWidth), -(5 + m_strokeWidth), (5 + m_strokeWidth));
	else if (m_startPoint.x() < m_endPoint.x() && m_startPoint.y() > m_endPoint.y())
		m_shapeRect.adjust(-(5 + m_strokeWidth), (5 + m_strokeWidth), (5 + m_strokeWidth), -(5 + m_strokeWidth));
}

void ShapeBase::setStretchRate(const double& rate)
{
	m_stretchRate = rate;
}

QColor ShapeBase::getPenColor()
{
	if (m_penColor.isValid())
		return m_penColor;
	return QColor();
}

void ShapeBase::initOutRect()
{
	m_leftTopRect = QRect(m_shapeRect.topLeft().x() - 6, m_shapeRect.topLeft().y() - 6, 10, 10);
	m_leftMiddleRect = QRect(m_shapeRect.topLeft().x() - 6, m_shapeRect.topLeft().y() + (m_shapeRect.height() / 2) - 6, 10, 10);
	m_leftBottomRect = QRect(m_shapeRect.bottomLeft().x() - 6, m_shapeRect.bottomLeft().y() - 6, 10, 10);
	m_rightTopRect = QRect(m_shapeRect.topRight().x() - 4, m_shapeRect.topRight().y() - 6, 10, 10);
	m_rightMiddleRect = QRect(m_shapeRect.topRight().x() - 4, m_shapeRect.topLeft().y() + (m_shapeRect.height() / 2) - 6, 10, 10);
	m_rightBottomRect = QRect(m_shapeRect.bottomRight().x() - 4, m_shapeRect.bottomRight().y() - 5, 10, 10);
	m_middleTopRect = QRect(m_shapeRect.topLeft().x() + (m_shapeRect.width() / 2) - 6, m_shapeRect.topLeft().y() - 6, 10, 10);
	m_middleBottomRect = QRect(m_shapeRect.bottomLeft().x() + (m_shapeRect.width() / 2) - 6, m_shapeRect.bottomLeft().y() - 4, 10, 10);	
}

void ShapeBase::destoryOutRect()
{
	m_leftTopRect = QRectF(-10, -10, 0.0, 0.0);
	m_rightTopRect = QRectF(-10, -10, 0.0, 0.0);
	m_leftBottomRect = QRectF(-10, -10, 0.0, 0.0);
	m_rightBottomRect = QRectF(-10, -10, 0.0, 0.0);
	m_middleTopRect = QRectF(-10, -10, 0.0, 0.0);
	m_middleBottomRect = QRectF(-10, -10, 0.0, 0.0);
	m_leftMiddleRect = QRectF(-10, -10, 0.0, 0.0);
	m_rightMiddleRect = QRectF(-10, -10, 0.0, 0.0);
}

