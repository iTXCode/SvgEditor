#include "kcustomwindow.h"

KButtomWidget::KButtomWidget()
{
}

KButtomWidget::KButtomWidget(QWidget* parent)
{
	this->setParent(parent);
	this->setStyleSheet("background-color:#1E1E1E;");
	m_leftTop = QRect(0, 0, 30, 30);
	m_leftBottom = QRect(0, 60, 30, 30);
	m_leftMid = QRect(0, 30, 30, 30);
	this->setFixedSize(790, 90);
	this->setMouseTracking(true);
	m_colorList = QStringList{ "#444444", "#482816", "#422C10", "#3B2F0E", "#32320F", "#293414", "#1F361B", "#153723","#0C372C",
		"#083734", "#0E353B","#1A333F", "#273141", "#332D40", "#3E2A3C", "#462735", "#4B252D", "#4D2425", "#4C261D", "#666666", 
		"#845335", "#7B572D","#6F5C2A", "#62612C", "#546433", "#46673D", "#396849", "#306856", "#2D6862", "#33666C", "#426373",
		"#535F75", "#645A73","#74556D", "#805064", "#884D58", "#8B4D4B", "#894F3F", "#999999", "#C48157", "#B8874D", "#A98E49",
		"#97944B", "#849854","#729C62", "#619E73", "#559E84", "#529D94", "#5B9BA2", "#6D97AB", "#8391AE", "#9A8AAB", "#AF84A3", 
		"#BF7E96", "#C97A86","#CE7975", "#CC7C65", "#BBBBBB", "#FFB27C", "#FABA6F", "#E6C36A", "#CFCA6D", "#B8D078", "#A0D58A",
		"#8CD79F", "#7DD8B5","#7AD6CA", "#84D3DB", "#9ACEE6", "#B6C7EA", "#D3BEE7", "#EDB6DC", "#FFAFCC", "#FFAAB8", "#FFA9A2",
		"#FFAC8D", "#DDDDDD","#FFE7A2", "#FFF093", "#FFFA8D", "#FFFF91", "#EEFF9F", "#D1FFB4", "#B9FFCE", "#A8FFE9", "#A4FFFF",
		"#B1FFFF", "#CBFFFF","#EDFFFF", "#FFF5FF", "#FFEBFF", "#FFE2FF", "#FFDCEC", "#FFDBD2", "#FFDFB8" };
	initAllRect();
}


KButtomWidget::~KButtomWidget()
{
}


QColor KButtomWidget::getSelectedColor()
{
	if (m_selectColor.isValid())
		return m_selectColor;
	return QColor();
}

void KButtomWidget::paintTopRect(QPainter& painter)
{
	painter.setBrush(Qt::white);
	painter.drawRect(m_leftTop);
	painter.setPen(QPen(Qt::red, 4));
	painter.drawLine(QPoint(3,3), QPoint(28, 28));
	painter.setPen(Qt::NoPen);
}

void KButtomWidget::paintMidRect(QPainter& painter)
{
	painter.setBrush(Qt::black);
	painter.drawRect(m_leftMid);
}

void KButtomWidget::paintBottomRect(QPainter& painter)
{
	painter.setBrush(Qt::white);
	painter.drawRect(m_leftBottom);
}

void KButtomWidget::initAllRect()
{
	int X;
	int Y = 0;
	for (int i = 0; i < 5; i++)
	{
		X = 30;
		for (int j = 0; j < 19; j++)
		{
			m_colorRect << QRect(X, Y, 40, 18);
			X += 40;
		}
		Y += 18;
	}
}

void KButtomWidget::paintAllRect(QPainter& painter)
{
	for (int i = 0; i < m_colorList.size(); i++)
	{
		painter.setBrush(QColor(m_colorList[i]));
		painter.drawRect(m_colorRect[i]);
	}
}


void KButtomWidget::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	paintTopRect(painter);
	paintMidRect(painter);
	paintBottomRect(painter);
	paintAllRect(painter);
}

void KButtomWidget::mousePressEvent(QMouseEvent* ev)
{
	if (m_leftTop.contains(ev->pos()))
	{
		emit selectedOneColor(QColor("#00FFFFFF"));
		return;
	}
	if (m_leftMid.contains(ev->pos()))
	{
		emit selectedOneColor(Qt::black);
		return;
	}
	if (m_leftBottom.contains(ev->pos()))
	{
		emit selectedOneColor(Qt::white);
		return;
	}

	for (int i = 0; i < m_colorList.size(); i++)
	{
		if (m_colorRect[i].contains(ev->pos()))
		{
			m_selectColor = m_colorList[i];
			emit selectedOneColor(m_selectColor);
			break;
		}
	}
}

void KButtomWidget::mouseMoveEvent(QMouseEvent* ev)
{
	QPixmap pixmap(":res/image/color_svg.svg");
	this->setCursor(QCursor(pixmap));
}

void KButtomWidget::mouseReleaseEvent(QMouseEvent* ev)
{
	if (!m_leftTop.contains(ev->pos())
		&& !m_leftMid.contains(ev->pos())
		&& !m_leftBottom.contains(ev->pos()))
		return;
}

KSwitchColorFill::KSwitchColorFill(QWidget* parent)
{
	this->setParent(parent);
	this->setFixedSize(45, 45);
	m_rectTop = QRect(0,0,30,30);
	m_rectBottm = QRect(15, 15, 30, 30);
	m_rectCenter = QRect(25, 25, 10, 10);
}

void KSwitchColorFill::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 2));
	if (m_fristPaint)
	{
		painter.setBrush(QColor("#5B9BA2"));
		painter.drawRect(m_rectBottm);
		painter.setBrush(QColor("#999999"));
		painter.drawRect(m_rectCenter);
		painter.setBrush(QColor("#CCCCCC"));
		painter.drawRect(m_rectTop);
	}
	else
	{
		painter.setBrush(QColor("#CCCCCC"));
		painter.drawRect(m_rectTop);
		painter.setBrush(QColor("#5B9BA2"));
		painter.drawRect(m_rectBottm);
		painter.setBrush(QColor("#999999"));
		painter.drawRect(m_rectCenter);
	}
}

void KSwitchColorFill::mousePressEvent(QMouseEvent* ev)
{
	if (m_rectTop.contains(ev->pos()))
		//绘制填充图形颜色的矩形
	{
		m_fristPaint = true;
		emit setShapeBorderColor(false);
	}
	else
	{ 
		m_fristPaint = false;
		emit setShapeBorderColor(true);
	}
	update();
}

KAdjustWidget::KAdjustWidget()
{}

KAdjustWidget::KAdjustWidget(QWidget* parent)
{
	this->setParent(parent);

	this->setFixedSize(100, 100);
	m_myTitle = new QLabel(this);
	m_myLabel = new QLineEdit(this);
	m_myTitle->setFixedSize(100,20);
	m_myLabel->setFixedSize(100, 80);
	m_myTitle->move(0, 0);
	m_myLabel->move(0, 20);

	m_myLabel->setText(QString::number(500));
	m_currentNum = m_myLabel->text().toInt();
	this->setAttribute(Qt::WA_StyledBackground);
	this->setStyleSheet("background-color:#3F3F3C;border: 4px solid #B3B3B3;");
	m_myTitle->setStyleSheet("border:none; font-size: 20px;font-weight:bold;font - family: PingFangSC - Semibold, PingFang SC;color:#FFFFFF;");
	m_myLabel->setStyleSheet("border:none;font-size: 45px;font-weight:bold;font - family: PingFangSC - Semibold, PingFang SC;color:#007acc");
	connect(m_myLabel,SIGNAL(editingFinished()),this,SLOT(setCurrentLabelNum()));
	connect(m_myLabel, SIGNAL(editingFinished()), this, SLOT(setTextLineVisible()));
	m_myLabel->setEnabled(false);
}

KAdjustWidget::~KAdjustWidget()
{
}

void KAdjustWidget::setCurrentLabelNum()
{
	m_currentNum = m_myLabel->text().toInt();
	if (m_currentNum < m_minValue)
		m_currentNum = m_minValue;
	if (m_currentNum > m_maxValue)
		m_currentNum = m_maxValue;
	m_myLabel->setText(QString::number(m_currentNum));
	emit currentNumChange(m_currentNum);
}
void KAdjustWidget::setCurrentWidgetNum(int num)
{
	m_currentNum = num;
	if (m_currentNum < m_minValue)
		m_currentNum = m_minValue;
	if (m_currentNum > m_maxValue)
		m_currentNum = m_maxValue;
	m_myLabel->setText(QString::number(m_currentNum));
	emit currentNumChange(m_currentNum);
}
void KAdjustWidget::setWidgetTitle(QString title)
{
	m_myTitle->setText(title);
}

void KAdjustWidget::setWidgetSize(int num)
{
	m_myLabel->setText(QString::number(num));
	m_currentNum = num;
}

void KAdjustWidget::setDisPlayNumRange(int min, int max)
{
	m_minValue = min;
	m_maxValue = max;
}

int KAdjustWidget::getWidgetSize()
{
	return m_myLabel->text().toInt();
}

void KAdjustWidget::setTextLineVisible()
{
	m_myLabel->setEnabled(false);
}

void KAdjustWidget::mousePressEvent(QMouseEvent* ev)
{
	m_myLabel->setEnabled(false);
	m_startPoint = ev->pos();
}

void KAdjustWidget::mouseMoveEvent(QMouseEvent* ev)
{
	m_endPoint = ev->pos();
	m_currentNum += (m_startPoint.y() - m_endPoint.y());
	if (m_currentNum > m_maxValue)
		m_currentNum = m_maxValue;
	if (m_currentNum < m_minValue)
		m_currentNum = m_minValue;
	m_myLabel->setText(QString::number(m_currentNum));
	m_startPoint = ev->pos();
	emit currentNumChange(m_currentNum);
}
void KAdjustWidget::wheelEvent(QWheelEvent* ev)
{
	QPoint numDegrees;
	numDegrees = ev->angleDelta();

	double step = 0;
	if (!numDegrees.isNull())
	{
		step = numDegrees.y() / 120;
	}

	ev->accept();
	m_currentNum +=  step;
	if (m_currentNum > m_maxValue)
		m_currentNum = m_maxValue;
	if (m_currentNum < m_minValue)
		m_currentNum = m_minValue;
	m_myLabel->setText(QString::number(m_currentNum));
	emit currentNumChange(m_currentNum);
}

void KAdjustWidget::mouseDoubleClickEvent(QMouseEvent* ev)
{
	m_myLabel->setEnabled(true);
}

KAdjustScale::KAdjustScale()
{

}

KAdjustScale::KAdjustScale(QWidget* parent)
{
	this->setParent(parent);
	this->setFixedSize(100, 100);
	m_myTitle = new QLabel(this);
	m_myLabel = new QLineEdit(this);
	m_myTitle->setFixedSize(100, 20);
	m_myLabel->setFixedSize(100, 80);
	m_myTitle->move(0, 0);
	m_myLabel->move(0, 20);

	m_myLabel->setText(QString::number(500));
	m_currentNum = m_myLabel->text().toInt();
	this->setAttribute(Qt::WA_StyledBackground);
	this->setStyleSheet("background-color:#3F3F3C;border: 4px solid #B3B3B3;");
	m_myTitle->setStyleSheet("border:none; font-size: 20px;font-weight:bold;font - family: PingFangSC - Semibold, PingFang SC;color:#FFFFFF;");
	m_myLabel->setStyleSheet("border:none;font-size: 30px;font-weight:bold;font - family: PingFangSC - Semibold, PingFang SC;color:#007acc");
	m_myLabel->setEnabled(false);
}
void KAdjustScale::setWidgetSize(int num)
{
	m_myLabel->setText(QString::number(num*100)+QString("%"));
	m_currentNum = num;
}
void KAdjustScale::setCurrentLableRate(double rate)
{
	m_currentNum = rate;
	if (m_currentNum < m_minValue)
		m_currentNum = m_minValue;
	if (m_currentNum > m_maxValue)
		m_currentNum = m_maxValue;
	m_myLabel->setText(QString::number(m_currentNum*100) + QString("%"));
	emit currentNumChange(m_currentNum);
}

void KAdjustScale::mousePressEvent(QMouseEvent* ev)
{
	ev->ignore();
}

void KAdjustScale::mouseMoveEvent(QMouseEvent* ev)
{
	ev->ignore();
}
void KAdjustScale::wheelEvent(QWheelEvent* ev)
{
	ev->ignore();
}

void KAdjustScale::mouseDoubleClickEvent(QMouseEvent* ev)
{
	m_myLabel->setEnabled(false);
}

