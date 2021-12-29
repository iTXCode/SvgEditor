#ifndef QRAWINGBOARD_H
#define QRAWINGBOARD_H
#pragma once
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QPaintEvent>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QKeyEvent>
#include <QXmlStreamReader>
#include "shapebase.h"
#include "kdrawline.h"
#include "kdrawellipse.h"
#include "kdrawrect.h"
#include "kdrawfreeline.h"
#include "kdrawhexagon.h"
#include "kdrawtriangle.h"
#include "kdrawtextbox.h"

class KDrawingBoard : public QWidget
{
	Q_OBJECT
public:
	KDrawingBoard(QWidget* parent = 0);
	~KDrawingBoard();
	void setDrawingBoardColor(const QColor&);
	void setBrushColor(QColor);
	void setStrokeWidth(int& size);
	void clearCanvas();
	bool isDrawingBoardEmpty();
	void setDrawBoardSize(const QSize size);
	bool svgReaderIsValid();

signals:
	void toRightButtonIsVisible(bool flag);
	void toDeleteItemVisible(bool falg);
	void toCopyItemVisbile(bool falg);
	void toCutItemVisbile(bool falg);
	void toPasteItemVisible(bool flag);
	void toLaunchStretchRate(double rate);
	void toLaunchWidgetWidth(int width);
	void toLaunchWidgetHigh(int high);
	void changeCurrentIndex(int index);

private slots:
	void setCurrentShape(ShapeStyle s);
	void loadSvgDocument(QFileInfo& info, QString& fileName);
	void saveWidgetContent(QString& filePath);
	void openSvgContentInTheWidget(QString& filePath);
	void setDeleteItem();
	void setTopItem();
	void setBotItem();
	void setCopyItem();
	void setCutItem();
	void setPasteItem();
	void setSwitchObject(bool flag);
	void deleteTheObject(ShapeBase*);

protected:
	void paintEvent(QPaintEvent* ev);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);
	void wheelEvent(QWheelEvent* ev);
	void contextMenuEvent(QContextMenuEvent* ev);
	void mouseDoubleClickEvent(QMouseEvent* ev);

private:
	void clearSelectedStatus();
	void pointInMultiSelectRegion(const QPoint& point);
	void isMultiSelectProcess(QPoint point);
	void isMouseClickMove(QPoint point);
	void pointInMouseClickRegion(const QPoint& point);
	void cancelMultiSelectionState();
	void drawMultiSelectedRect(ShapeBase* picture, QPainter& painter);
	void createShapeBase(const ShapeStyle& shape, QPoint& point);
	HitOutRect hitWhichOutRect(QPoint pos);
	void changeShapeByMouse(const QPoint& pos);
	void writeXmlToCurrentWidget(int& shapeID, QXmlStreamReader& xmlReader);
	void initRightClickMenu();
	void drawSelectedGraph(QPainter& painter);
	void drawMultiSelectRect(QPainter& painter);
	void encapsulateMouseClickEvents(const QPoint& point,QMouseEvent* ev);
	void encapsulateMouseMoveEvents(QMouseEvent* ev);
	void encapsulateMouseReleaseEvents(QMouseEvent* ev);
	void fromRgbaToArgb(QString& str);
private:
	double m_strokeWidth = 0.0;
	double m_stretchRate = 1.0;
	double m_stretchRateStage = 0.0;

	ShapeBase* m_shape = nullptr;
	ShapeStyle m_currShapeCode = ShapeStyle::MOUSE;
	QMenu* m_righClickMenu = nullptr; //右键菜单
	ShapeBase* m_selectedShape = nullptr;
	QLineEdit* m_textBoxSpecial = nullptr;  //用于记录文本框地址的指针
	QSvgRenderer* m_svgRead = nullptr;
	QColor m_theWidgetBGColor = QColor("#FFFFFF");
	QColor m_brushColor = Qt::white;

private:
	HitOutRect m_clickOutRect;	//用于记录外围的方框图形
	bool m_mouseClickOutRect = false; //记录鼠标是否点击了选中矩形的小矩形
	bool m_switchColor = true; //用于转换填充色和画笔色的开关
	bool m_showMultiRect = true;  //区分是否需要显示框选图
	bool m_mouseMultiSelection = false;//判断此次点击是框选过程还是移动过程
	bool m_needEndPointFlag = false; //为true 的时候是需要需要设置粘贴点
	bool m_mouseMultiSelect = false;
	bool m_mouseClickSelect = false;
	bool m_isFreeLineDraw = false;

private:
	QRectF m_multiSelectRect = QRectF(); //多选框
	QPoint m_startMousePoint = QPoint();//记录框选起点
	QPoint m_endMousePoint = QPoint(); //记录框选终点
	QPointF m_storageMovePosition = QPointF(); //记录拖动移动的起点
	QSize m_currentWidgetSize = QSize(500, 500);

private:
	QList<ShapeBase*> m_shapeList;
	QList<ShapeBase*> m_copyContains;
	QList<ShapeBase*> m_selectedContainer;
	QList<ShapeBase*> m_lineTextContain; //用于存储文本框图形
};

#endif // QRAWINGBOARD_H