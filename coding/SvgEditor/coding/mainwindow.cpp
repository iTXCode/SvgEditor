#include "mainwindow.h"
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QColorDialog>


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent)
{
	initUI();
	this->setWindowTitle("SVG Space");
	this->setWindowIcon(QIcon(":res/image/window_svg.svg"));
	this->setContextMenuPolicy(Qt::NoContextMenu);
	Q_INIT_RESOURCE(res);
}

MainWindow::~MainWindow()
{
	delete m_mainTitleBar;
	m_mainTitleBar = nullptr;

	delete m_menuFile;
	m_menuFile = nullptr;

	delete m_leftToolBar;
	m_leftToolBar = nullptr;

	delete m_rightToolBar;
	m_rightToolBar = nullptr;

	delete m_drawBoard;
	m_drawBoard = nullptr;

	delete m_scrollWidget;
	m_scrollWidget = nullptr;

	delete m_botWidget;
	m_botWidget = nullptr;

	delete m_centerWidget;
	m_centerWidget = nullptr;

	Q_CLEANUP_RESOURCE(res);
}

void MainWindow::initUI()
{
	this->setMinimumWidth(935);
	this->setMinimumHeight(700);
	initTitleBar();
	initLeftToolBar();
	initRightToolBar();
}

void MainWindow::initTitleBar()
{
	m_mainTitleBar = new QMenuBar(this);
	m_mainTitleBar->setStyleSheet("background-color:#707070");
	this->setMenuBar(m_mainTitleBar);

	m_menuFile = new QMenu(this);
	m_menuFile->setTitle("文件");
	m_menuFile->setStyleSheet("background-color:white;border-radius:10px;");
	m_mainTitleBar->addSeparator();
	m_mainTitleBar->addAction(m_menuFile->menuAction());
	QAction* openFile = new QAction("打开文件", this);
	QAction* saveFile = new QAction("保存文件", this);
	QAction* newFile = new QAction("新建文件", this);

	m_menuFile->addSeparator();
	m_menuFile->addAction(openFile);
	m_menuFile->addSeparator();
	m_menuFile->addAction(saveFile);
	m_menuFile->addSeparator();
	m_menuFile->addAction(newFile);
	connect(saveFile, &QAction::triggered, this, &MainWindow::actionSaveSvg);
	connect(openFile, &QAction::triggered, this, &MainWindow::openSvgFile);
	connect(newFile, &QAction::triggered, this, &MainWindow::newSvgFile);
}

void MainWindow::initLeftToolBar()
{
	//工具栏设置
	m_leftToolBar = new QToolBar(this);
	m_leftToolBar->setMovable(false);
	m_leftToolBar->setStyleSheet("background-color:#707070;border:2px;");
	this->addToolBar(Qt::LeftToolBarArea, m_leftToolBar);

	//箭头A
	QToolButton* arrow = new QToolButton(this);
	arrow->setFixedSize(45, 45);
	arrow->setIcon(QIcon(":/res/image/arrow_svg.svg"));
	//arrow->setStyleSheet(
	//"QToolButton::hover{background-color:#FFFFFF;}"
	//"QToolButton::pressed{background-color:#00CCFF;}"
	//"QToolButton{background-color:#707070;}");
	arrow->setStyleSheet("QToolButton{border-image: url(:/res/image/arrow_svg.svg;}"
		"QToolButton:hover{border-image: url(:/res/image/arrow_hover.svg);}"
		"QToolButton:pressed{border-image: url(:/res/image/arrow_pressed.svg);}");
	arrow->setIconSize(QSize(45, 45));

	//画笔
	QToolButton* paintPen = new QToolButton(this);
	paintPen->setFixedSize(45, 45);
	paintPen->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	paintPen->setIcon(QIcon(":res/image/draw_pen.svg"));
	paintPen->setIconSize(QSize(45, 45));

	//划线
	QToolButton* linePainter = new QToolButton(this);
	linePainter->setFixedSize(45, 45);
	linePainter->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	linePainter->setIcon(QIcon(":/res/image/line_svg.svg"));
	linePainter->setIconSize(QSize(45, 45));

	//矩形
	QToolButton* rectPainter = new QToolButton(this);
	rectPainter->setFixedSize(45, 45);
	rectPainter->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	rectPainter->setIcon(QIcon(":/res/image/rect_svg.svg"));
	rectPainter->setIconSize(QSize(45, 45));

	//椭圆
	QToolButton* ellipsePainter = new QToolButton(this);
	ellipsePainter->setFixedSize(45, 45);
	ellipsePainter->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	ellipsePainter->setIcon(QIcon(":/res/image/ellipse_svg.svg"));
	ellipsePainter->setIconSize(QSize(45, 45));

	//三角形
	QToolButton* trianglePainter = new QToolButton(this);
	trianglePainter->setFixedSize(45, 45);
	trianglePainter->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	trianglePainter->setIcon(QIcon(":/res/image/triangles_svg.svg"));
	trianglePainter->setIconSize(QSize(45, 45));

	//六边形
	QToolButton* hexagonPainter = new QToolButton(this);
	hexagonPainter->setFixedSize(45, 45);
	hexagonPainter->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	hexagonPainter->setIcon(QIcon(":/res/image/hexagon_svg.svg"));
	hexagonPainter->setIconSize(QSize(45, 45));

	//文本框
	QToolButton* textBoxPainter = new QToolButton(this);
	textBoxPainter->setFixedSize(45, 45);
	textBoxPainter->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	textBoxPainter->setIcon(QIcon(":/res/image/textbox_svg.svg"));
	textBoxPainter->setIconSize(QSize(45, 45));

	//清除
	QToolButton* clearPainter = new QToolButton(this);
	clearPainter->setFixedSize(45, 45);
	clearPainter->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	clearPainter->setIcon(QIcon(":/res/image/clear_svg.svg"));
	clearPainter->setIconSize(QSize(45, 45));

	KSwitchColorFill* switchFill = new KSwitchColorFill(this);
	switchFill->setStyleSheet(
		"QToolButton::hover{background-color:#FFFFFF;}"
		"QToolButton::pressed{background-color:#00CCFF;}"
		"QToolButton{background-color:#707070;}");
	m_leftToolBar->addWidget(arrow);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(paintPen);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(linePainter);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(rectPainter);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(ellipsePainter);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(trianglePainter);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(hexagonPainter);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(textBoxPainter);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(switchFill);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addWidget(clearPainter);

	initPaletteStyle();
	connect(arrow, SIGNAL(clicked(bool)), this, SLOT(drawMouseActionTriggred()));
	connect(paintPen, SIGNAL(clicked(bool)), this, SLOT(drawFreeActionTriggred()));
	connect(linePainter, SIGNAL(clicked(bool)), this, SLOT(drawLineActionTriggered()));
	connect(rectPainter, SIGNAL(clicked(bool)), this, SLOT(drawRectActionTriggered()));
	connect(ellipsePainter, SIGNAL(clicked(bool)), this, SLOT(drawEllipseActionTriggered()));
	connect(hexagonPainter, SIGNAL(clicked(bool)), this, SLOT(drawHexaGonActionTrigged()));
	connect(trianglePainter, SIGNAL(clicked(bool)), this, SLOT(drawTriangleActionTrigged()));
	connect(textBoxPainter, SIGNAL(clicked(bool)), this, SLOT(drawTextBoxActionTrigged()));
	connect(clearPainter, SIGNAL(clicked()), this, SLOT(newSvgFile()));
	connect(switchFill, SIGNAL(setShapeBorderColor(bool)), m_drawBoard, SLOT(setSwitchObject(bool)));
}


void MainWindow::initRightToolBar()
{
	m_rightToolBar = new QToolBar(this);
	m_rightToolBar->setStyleSheet("background-color:#707070;width:90;border:2px");
	m_rightToolBar->setMovable(false);
	this->addToolBar(Qt::RightToolBarArea, m_rightToolBar);

	//设置右侧工具栏的内容
	m_stackedWidget = new QStackedWidget(this);
	QWidget* toolWidget = new QWidget(m_rightToolBar);
	QVBoxLayout* mainLayout = new QVBoxLayout(toolWidget);

	QVBoxLayout* widgetLayout = new QVBoxLayout(toolWidget);
	m_highWidget = new KAdjustWidget(this);
	m_highWidget->setWidgetTitle("画布高度");
	m_highWidget->setWidgetSize(500);
	m_highWidget->setDisPlayNumRange(500, 4000);

	m_widthWidget = new KAdjustWidget(this);
	m_widthWidget->setWidgetTitle("画布宽度");
	m_widthWidget->setWidgetSize(500);
	m_widthWidget->setDisPlayNumRange(500, 4000);
	QHBoxLayout* adjustLayout = new QHBoxLayout(toolWidget);
	adjustLayout->addWidget(m_highWidget);
	adjustLayout->addWidget(m_widthWidget);

	QHBoxLayout* pen_Color_Layout = new QHBoxLayout(toolWidget);
	m_penStroke = new KAdjustWidget(this);
	m_penStroke->setWidgetTitle("画笔粗细");
	m_penStroke->setWidgetSize(1);
	m_penStroke->setDisPlayNumRange(1, 50);

	m_drawBoadrScale = new KAdjustScale(this);
	m_drawBoadrScale->setWidgetTitle("缩放比例");
	m_drawBoadrScale->setWidgetSize(1);
	m_drawBoadrScale->setDisPlayNumRange(0.1, 10);

	pen_Color_Layout->addWidget(m_penStroke);
	pen_Color_Layout->addWidget(m_drawBoadrScale);

	m_selectColor = new QToolButton(toolWidget);
	m_selectColor->setFixedSize(100, 100);
	m_selectColor->setStyleSheet("background-color:#FFFFFF;border-radius: 10px;border: 1px solid #B3B3B3;");

	widgetLayout->addLayout(adjustLayout);
	widgetLayout->addLayout(pen_Color_Layout);
	mainLayout->addLayout(widgetLayout);
	mainLayout->addWidget(m_selectColor);
	mainLayout->addStretch();

	//m_rightToolBar->addWidget(toolWidget);
	connect(m_widthWidget, &KAdjustWidget::currentNumChange, this, &MainWindow::setPaletteWidth);
	connect(m_highWidget, &KAdjustWidget::currentNumChange, this, &MainWindow::setPaletteHigh);
	connect(m_penStroke, &KAdjustWidget::currentNumChange, this, &MainWindow::setPenSize);
	connect(m_selectColor, &QToolButton::clicked, this, &MainWindow::setColor);
	connect(m_drawBoard, SIGNAL(toLaunchWidgetHigh(int)), m_highWidget, SLOT(setCurrentWidgetNum(int)));
	connect(m_drawBoard, SIGNAL(toLaunchWidgetWidth(int)), m_widthWidget, SLOT(setCurrentWidgetNum(int)));
	connect(m_drawBoard, SIGNAL(toLaunchStretchRate(double)), m_drawBoadrScale, SLOT(setCurrentLableRate(double)));

	


	//第二个窗口的建立
	QWidget* objectInfo = new QWidget(this);
	//此窗口展示的是对象起点坐标的X值
	QVBoxLayout* objectLayout = new QVBoxLayout(objectInfo);

	QHBoxLayout* xAndYLayout = new QHBoxLayout(this);
	KAdjustWidget* objectX = new KAdjustWidget(objectInfo);
	objectX->setWidgetTitle("    X");
	objectX->setWidgetSize(500);
	objectX->setDisPlayNumRange(0, 4000);

	KAdjustWidget* objectY = new KAdjustWidget(objectInfo);
	objectY->setWidgetTitle("    Y");
	objectY->setWidgetSize(500);
	objectY->setDisPlayNumRange(0, 4000);
	xAndYLayout->addWidget(objectX);
	xAndYLayout->addWidget(objectY);

	QHBoxLayout* wAndHLayout = new QHBoxLayout(this);
	KAdjustWidget* objectWidth = new KAdjustWidget(objectInfo);
	objectWidth->setWidgetTitle("   宽度");
	objectWidth->setWidgetSize(500);
	objectWidth->setDisPlayNumRange(0,4000);

	KAdjustWidget* objectHigh = new KAdjustWidget(objectInfo);
	objectHigh->setWidgetTitle("   高度");
	objectHigh->setWidgetSize(500);
	objectHigh->setDisPlayNumRange(0, 4000);
	wAndHLayout->addWidget(objectWidth);
	wAndHLayout->addWidget(objectHigh);

	KAdjustWidget*  penStroke = new KAdjustWidget(this);
	penStroke->setWidgetTitle("画笔粗细");
	penStroke->setWidgetSize(1);
	penStroke->setDisPlayNumRange(1, 50);

	
	m_stackedWidget->addWidget(toolWidget);
	m_stackedWidget->addWidget(objectInfo);
	objectLayout->addLayout(xAndYLayout);
	objectLayout->addLayout(wAndHLayout);
	objectLayout->addWidget(penStroke);
	objectLayout->addStretch();
	m_rightToolBar->addWidget(m_stackedWidget);

	connect(m_drawBoard,SIGNAL(changeCurrentIndex(int)),m_stackedWidget,SLOT(setCurrentIndex(int)));
	connect(objectWidth, &KAdjustWidget::currentNumChange, this, &MainWindow::setPaletteWidth);

	//画布上的图形数据需要和窗口上的数据展示点进行关联。同步展示相关的数据
	// 相互绑定才行
}

void MainWindow::initPaletteStyle()
{
	m_centerWidget = new QWidget(this);
	m_centerWidget->setStyleSheet("background-color:#3F3F3C");
	this->setCentralWidget(m_centerWidget);
	m_scrollWidget = new QScrollArea(m_centerWidget);
	m_scrollWidget->setAlignment(Qt::AlignCenter);
	m_scrollWidget->resize(m_centerWidget->width(), m_centerWidget->height());

	m_drawBoard = new KDrawingBoard(this);
	m_scrollWidget->setWidget(m_drawBoard);
	m_botWidget = new KButtomWidget(m_centerWidget);
	m_botWidget->move(0, this->height() - 90);
	QVBoxLayout* mainLayout = new QVBoxLayout(m_centerWidget);

	mainLayout->addWidget(m_scrollWidget);
	mainLayout->addWidget(m_botWidget);
	if (!m_backGroundColor.isValid())
		m_drawBoard->setStyleSheet("background-color:#FFFFFF;border: 1px solid #B3B3B3;");
	else
		m_drawBoard->setStyleSheet("background-color:" + m_backGroundColor.name() + ";border: 1px solid #B3B3B3;");
	m_drawBoard->setMinimumSize(QSize(50, 50));
	m_drawBoard->resize(500, 500);

	connect(this, SIGNAL(changeCurrentShape(ShapeStyle)),
		m_drawBoard, SLOT(setCurrentShape(ShapeStyle)));
	connect(m_botWidget, SIGNAL(selectedOneColor(QColor)), this, SLOT(reSetBrushColor(QColor)));//选择了颜色，可以进行改正了
	connect(this, SIGNAL(saveCurrentSvgDocument(QString&)), m_drawBoard, SLOT(saveWidgetContent(QString&)));
	connect(this, SIGNAL(openSelectedSvgFile(QString&)), m_drawBoard, SLOT(openSvgContentInTheWidget(QString&)));
}

void MainWindow::drawMouseActionTriggred()
{
	emit changeCurrentShape(ShapeStyle::MOUSE);
}

void MainWindow::drawLineActionTriggered()
{
	emit changeCurrentShape(ShapeStyle::LINE);
}

void MainWindow::drawRectActionTriggered()
{
	emit changeCurrentShape(ShapeStyle::RECTS);
}

void MainWindow::drawEllipseActionTriggered()
{
	emit changeCurrentShape(ShapeStyle::ELLIPSE);
}

void MainWindow::drawFreeActionTriggred()
{
	emit changeCurrentShape(ShapeStyle::FREELINE);
}

void MainWindow::drawHexaGonActionTrigged()
{
	emit changeCurrentShape(ShapeStyle::HEXAGON);
}

void MainWindow::drawTriangleActionTrigged()
{
	emit changeCurrentShape(ShapeStyle::TRIANGLE);
}

void MainWindow::drawTextBoxActionTrigged()
{
	emit changeCurrentShape(ShapeStyle::TEXTBOX);
}

void MainWindow::actionSaveSvg()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save SVG",

		"", "SVG FILES (*.svg)");

	emit saveCurrentSvgDocument(filePath);
	//当触发这个指令的时候需要将信号发给画板，将画板上的内容写到文件中
}

void MainWindow::openSvgFile()
{
	if (m_drawBoard != nullptr && (m_drawBoard->isDrawingBoardEmpty() || m_drawBoard->svgReaderIsValid()))
	{
		QMessageBox box;
		QMessageBox::StandardButton result = box.information(NULL, "OPEN NEW FILE", "打开新的文件是否需要清空画布？",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if (result == QMessageBox::Yes)
			m_drawBoard->clearCanvas();
	}
	QString filePath = QFileDialog::getOpenFileName(this, "选择单个文件", "./", "SVG FILES (*.svg)");
	if (filePath.isEmpty())
		return;
	emit openSelectedSvgFile(filePath);
}

void MainWindow::newSvgFile()
{
	if (m_drawBoard != nullptr && (m_drawBoard->isDrawingBoardEmpty() || m_drawBoard->svgReaderIsValid()))
	{
		QMessageBox box;
		QMessageBox::StandardButton result = box.information(NULL, "清空画布?", "此操作将清空当前画布，请确定是否要进行该操作.",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if (result == QMessageBox::No)
			return;
	}

	//将记录画布上图形的容器清空
	m_drawBoard->clearCanvas();
	m_drawBoard->resize(m_widthWidget->getWidgetSize(), m_highWidget->getWidgetSize());
	m_drawBoard->setDrawBoardSize(QSize(m_widthWidget->getWidgetSize(), m_highWidget->getWidgetSize()));
}

void MainWindow::setColor()
{
	QColor getColor = QColorDialog::getColor();
	if (!getColor.isValid())
		return;

	m_backGroundColor = getColor;
	m_selectColor->setStyleSheet("background-color:" + getColor.name() + ";border-radius: 10px; border: 1px solid #B3B3B3;");
	m_drawBoard->setDrawingBoardColor(getColor);
}

void MainWindow::setPaletteHigh(int value)
{
	int width = m_widthWidget->getWidgetSize();
	m_drawBoard->setDrawBoardSize(QSize(width, value));
}
void MainWindow::setPaletteWidth(int value)
{
	int high = m_highWidget->getWidgetSize();
	m_drawBoard->setDrawBoardSize(QSize(value, high));
}

void MainWindow::reSetBrushColor(QColor color)
{
	m_drawBoard->setBrushColor(color);
}

void MainWindow::setPenSize(int size)
{
	m_drawBoard->setStrokeWidth(size);
}