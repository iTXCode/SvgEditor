#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QSizeF>
#include <QPaintEvent>
#include <QLabel>
#include <QSvgWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStackedWidget>
#include "shapebase.h"
#include "kdrawingboard.h"
#include "kcustomwindow.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
private:
	void initUI();
	void initTitleBar();
	void initLeftToolBar();
	void initRightToolBar();
	void initPaletteStyle();
signals:
	void openSelectedSvgFile(QString& filPath);
	void saveCurrentSvgDocument(QString& filPath);
	void changeCurrentShape(ShapeStyle newShape);

private slots:
	void setColor();
	void newSvgFile();
	void openSvgFile();
	void actionSaveSvg();
	void setPenSize(int);
	void setPaletteHigh(int value);
	void setPaletteWidth(int value);
	void reSetBrushColor(QColor color);
	void drawMouseActionTriggred();
	void drawLineActionTriggered();
	void drawRectActionTriggered();
	void drawEllipseActionTriggered();
	void drawFreeActionTriggred();
	void drawHexaGonActionTrigged();
	void drawTriangleActionTrigged();
	void drawTextBoxActionTrigged();

private:
	QColor m_backGroundColor = QColor();
	QMenu* m_menuFile = nullptr;
	QMenuBar* m_mainTitleBar = nullptr;
	QToolBar* m_leftToolBar = nullptr;
	QToolBar* m_rightToolBar = nullptr;
	KDrawingBoard* m_drawBoard = nullptr;
	QToolButton* m_selectColor = nullptr;
	QScrollArea* m_scrollWidget = nullptr;
	QWidget* m_centerWidget = nullptr;
	KButtomWidget* m_botWidget = nullptr;
	KAdjustWidget* m_highWidget = nullptr;
	KAdjustWidget* m_widthWidget = nullptr;
	KAdjustWidget* m_penStroke = nullptr;
	KAdjustScale* m_drawBoadrScale = nullptr;
	QStackedWidget* m_stackedWidget = nullptr;
};

#endif // MAINWINDOW_H