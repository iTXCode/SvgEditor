#ifndef SHAPEBASE_H
#define SHAPEBASE_H
#pragma once
#include <QtGui>
#include <QPoint>
#include <math.h>
#include <QLineEdit>
#include <QFontMetrics>
#include <QRectF>
#include <QWidget>
#include <QXmlStreamWriter>

enum HitOutRect
{
	MOUSESTATUS,
	TOPLEFTRECT,
	LEFTMIDDLERECT,
	BOTTOMLEFTRECT,
	BOTTOMMIDDLERECT,
	BOTTOMRIGHTRECT,
	RIGHTMIDDLERECT,
	TOPRIGHTRECT,
	TOPMIDDLERECT
};

enum ShapeStyle {
	MOUSE,
	LINE,
	RECTS,
	ELLIPSE,
	FREELINE,
	HEXAGON,
	TRIANGLE,
	TEXTBOX
};

class ShapeBase :public QObject
{
	Q_OBJECT
public:
	ShapeBase();
	ShapeBase(QWidget* parent);
	virtual ~ShapeBase();
	void initOutRect();
	void setStart(QPointF s);
	void setEnd(QPointF e);
	void setStrokeWidth(int);
	void destoryOutRect();
	void adjustShapeRect();
	void setShapeColor(QColor);
	void setPenColor(QColor color);
	void setSelectedStatus(const bool);
	void paintOutRect(QPainter& painter);
	void paintTopOutRect(QPainter& painter);
	void paintLeftOutRect(QPainter& painter);
	void setStretchRate(const double& rate);
	void copyOtherCharacter(ShapeBase* other);
	void reSetStartPoint(const int& width, const int& high);
	void reSetEndPoint(const int& width, const int& high);

	QColor getPenColor();//用来测试所用
	QPointF getStartPoint();
	QPointF getEndPoint();
	bool getSelectedStatus();
	ShapeStyle getGraphicalShapes();
	QRectF getMultiSelectContainRect();
	HitOutRect getSelectedBoxNum(QPointF pos);

	virtual void setShapeRect() = 0;
	virtual void  paint(QPainter& painter) = 0;
	virtual bool containsPoint(const QPointF&) = 0;
	virtual void writerShapeXml(QXmlStreamWriter& xmlWriter) = 0;
	virtual void reSetShapePosition(const double& width, const double& high) = 0;
protected:
	QString getRgbaFromQColor(QColor& color);
protected:
	double m_strokeWidth = 1.0;
	double m_stretchRate = 1.0;
	QRectF m_shapeRect = QRectF();
	QRectF m_multiSelectRect = QRectF();
	QPointF m_startPoint = QPointF();
	QPointF m_endPoint = QPointF();
	QColor m_penColor = QColor("#FF000000");
	QColor m_brushColor = QColor("#00FFFFFF");
	bool m_selectedStatus = false;
	ShapeStyle m_shapeStyle = ShapeStyle::MOUSE;

private:
	QRectF m_leftTopRect = QRectF(0.0,0.0,0.0,0.0);
	QRectF m_leftMiddleRect = QRectF(0.0, 0.0, 0.0, 0.0);
	QRectF m_leftBottomRect = QRectF(0.0, 0.0, 0.0, 0.0);
	QRectF m_rightTopRect = QRectF(0.0, 0.0, 0.0, 0.0);
	QRectF m_rightBottomRect = QRectF(0.0, 0.0, 0.0, 0.0);
	QRectF m_rightMiddleRect = QRectF(0.0, 0.0, 0.0, 0.0);
	QRectF m_middleTopRect = QRectF(0.0, 0.0, 0.0, 0.0);
	QRectF m_middleBottomRect = QRectF(0.0, 0.0, 0.0, 0.0);
};

#endif // SHAPEBASE_H
