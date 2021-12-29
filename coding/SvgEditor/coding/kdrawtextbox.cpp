#include "kdrawtextbox.h"

KDrawTextBox::KDrawTextBox()
{
}

KDrawTextBox::KDrawTextBox(QWidget* parent)
{
	this->setParent(parent);
	m_shapeStyle = ShapeStyle::TEXTBOX;
	m_textLine = new QLineEdit(parent);
	m_textLine->setAlignment(Qt::AlignCenter);//在可见的范围内居中显示
	m_textLine->setMaxLength(10);
	m_textLine->setFont(QFont("PingFang SC", 20));
	m_textLine->setContextMenuPolicy(Qt::NoContextMenu);
	m_textLine->setStyleSheet(QString("border:2px #ce7975;"
		"background-color: rgba(124,124,124,0);"
		"padding:1px 2px;border-style: dashed;color:" + m_penColor.name()));
	m_textFontStatus = m_textLine->font();
	m_textLine->resize(m_textLine->fontMetrics().averageCharWidth(),m_textLine->fontMetrics().height());
	m_lineTextWidth = m_textLine->width();
	m_lineTextHigh = m_textLine->height();
	connect(m_textLine, SIGNAL(editingFinished()), this, SLOT(setLineEditVisible()));
}

KDrawTextBox::~KDrawTextBox()
{
	delete m_textLine;
	m_textLine = nullptr;
}

void KDrawTextBox::paint(QPainter& painter)
{
	//为进行缩放的字体特征,主要目的设置文本的终点坐标等
	QFont noScaleFont = m_textFontStatus;
	m_textLine->setFont(m_textFontStatus);
	QFontMetrics noScaleFm = m_textLine->fontMetrics();
	m_endPoint.setX(m_startPoint.x() + (noScaleFm.width(m_textLine->text()) + noScaleFm.averageCharWidth()));
	m_endPoint.setY(m_startPoint.y() + noScaleFm.height());
	setShapeRect();
	//缩放后的文本框特性
	QFont font = m_textFontStatus;
	font.setPointSize(font.pointSize() * m_stretchRate);
	m_textLine->setFont(font);
	QFontMetrics scaleFm = m_textLine->fontMetrics();
	m_textLine->move(m_startPoint.x() * m_stretchRate,m_startPoint.y() * m_stretchRate);
	m_textLine->resize((scaleFm.width(m_textLine->text()) + scaleFm.averageCharWidth()+ 20 * m_stretchRate), scaleFm.height()+10);

	if (m_textLine && !m_textLine->isVisible())
	{
		QPen pen;
		pen.setColor(m_penColor);
		pen.setWidth(m_strokeWidth);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.setFont(noScaleFont);
		painter.drawText(QRectF(m_startPoint ,m_endPoint), m_textLine->text());
	}
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
}

bool KDrawTextBox::containsPoint(const QPointF& ev)
{
	if (m_shapeRect.isValid())
		return m_shapeRect.contains(ev/m_stretchRate);
	return false;
}

void KDrawTextBox::setShapeRect()
{
	m_shapeRect = QRectF(m_startPoint, m_endPoint);
	adjustShapeRect();
	m_multiSelectRect = QRectF(m_startPoint ,m_endPoint).normalized();
	m_multiSelectRect.adjust(-5, -5, 5, 5);
}

void KDrawTextBox::reSetShapePosition(const double& width, const double& high)
{
	m_startPoint = QPoint(m_startPoint.x() + width, m_startPoint.y() + high);
	setShapeRect();
}

void KDrawTextBox::writerShapeXml(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement("text");
	xmlWriter.writeAttribute("font-family","PingFang SC");
	xmlWriter.writeAttribute("font-size", QString::number(m_textFontStatus.pointSize()));
	xmlWriter.writeAttribute("y", QString::number(m_startPoint.y()+20));
	xmlWriter.writeAttribute("x", QString::number(m_startPoint.x() + 20));
	QString penColor = getRgbaFromQColor(m_penColor);
	xmlWriter.writeAttribute("stroke", penColor);
	xmlWriter.writeAttribute("fill", penColor);
	xmlWriter.writeCharacters(m_textLine->text());
	xmlWriter.writeEndElement();
}

void KDrawTextBox::copyOtherTextLine(ShapeBase* other)
{
	KDrawTextBox* tmpPtr = dynamic_cast<KDrawTextBox*>(other);
	this->m_textLine->setText(tmpPtr->getTextBoxObject()->text());
	this->m_lineTextHigh = tmpPtr->m_lineTextHigh;
	this->m_lineTextWidth = tmpPtr->m_lineTextWidth;

}

QLineEdit* KDrawTextBox::getTextBoxObject()
{
	if (m_textLine)
		return m_textLine;
	return nullptr;
}

void KDrawTextBox::setLineEditVisible()
{
	//QLineEdit回车触发两次editingFinished信号，
	//editingFinished()槽第一次响应enter键，此时焦点在QLineEdit上,响应完失去焦点
	//第二次响应焦点离开事件，此时焦点已经不在QLineEdit上
	if (m_textLine->hasFocus())
	{
		if (m_textLine->isVisible())
			m_textLine->setVisible(false);
		else
			m_textLine->setVisible(true);
	}
	else
	{
		m_textLine->setVisible(false);
		if (m_textLine->text().isEmpty())
		{
			emit toDeleteTheObject(this);
		}
	}
}



