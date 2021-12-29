#ifndef KBUTTOMWIDGET_H
#define KBUTTOMWIDGET_H
#pragma once

#include <QPen>
#include <QList>
#include <QRect>
#include <QColor>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QLineEdit>
#include <QPaintEvent>
#include <QStringList>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QMouseEvent>

class KButtomWidget :public QWidget
{
	Q_OBJECT
public:
	KButtomWidget();
	~KButtomWidget();
	KButtomWidget(QWidget* parent);
	QColor getSelectedColor();

signals:
	void selectedOneColor(QColor color);

private:
	void paintTopRect(QPainter&);
	void paintMidRect(QPainter&);
	void paintBottomRect(QPainter&);
	void initAllRect();
	void paintAllRect(QPainter&);

protected:
	void paintEvent(QPaintEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);

private:
	QRect m_leftTop = QRect();
	QRect m_leftMid = QRect();
	QRect m_leftBottom = QRect();
	QColor m_selectColor = QColor();
	QList<QRect> m_colorRect = QList<QRect>();
	QStringList m_colorList = QStringList();
};

class KSwitchColorFill : public QWidget
{
	Q_OBJECT
public:
	KSwitchColorFill(QWidget* parent);
signals:
	void setShapeBorderColor(bool);
protected:
	void paintEvent(QPaintEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
private:
	QRect m_rectTop = QRect();
	QRect m_rectBottm = QRect();
	QRect m_rectCenter = QRect();
	bool  m_fristPaint = false;
};

class KAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	KAdjustWidget();
	KAdjustWidget(QWidget* parent);
	~KAdjustWidget();
	void setWidgetTitle(QString title);
	void setWidgetSize(int num);
	void setDisPlayNumRange(int min, int max);
	int getWidgetSize();
signals:
	void currentNumChange(int num);
private slots:
	void setTextLineVisible();
	void setCurrentLabelNum();
	void setCurrentWidgetNum(int);
protected:
	void wheelEvent(QWheelEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mouseDoubleClickEvent(QMouseEvent* ev);
protected:
	QLabel* m_myTitle = nullptr;
	QLineEdit* m_myLabel = nullptr;
	QPoint m_startPoint = QPoint();
	QPoint m_endPoint = QPoint();
	int m_currentNum = 0;
	int m_minValue = 0;
	int m_maxValue = 10000;
	bool m_lineIsVisible = false;
};

class KAdjustScale :public KAdjustWidget
{
	Q_OBJECT
public:
	KAdjustScale();
	KAdjustScale(QWidget* parent);
	void setWidgetSize(int num);
private slots:
	void setCurrentLableRate(double rate);
protected:
	void wheelEvent(QWheelEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mouseDoubleClickEvent(QMouseEvent* ev);
private:
	double m_currentNum = 0.0;
	QPointF m_startPoint = QPointF();
	QPointF m_endPoint = QPointF();
};
#endif //KBUTTOMWIDGET_H