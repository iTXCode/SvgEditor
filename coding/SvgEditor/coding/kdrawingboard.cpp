#include "kdrawingboard.h"
#include <QSvgRenderer>
#include <QtMath>

KDrawingBoard::KDrawingBoard(QWidget* parent)
	: QWidget(parent)
{
	m_strokeWidth = 1;
	m_svgRead = new QSvgRenderer();
	this->setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);
	setAttribute(Qt::WA_StyledBackground);
	initRightClickMenu();
}

void KDrawingBoard::initRightClickMenu()
{
	m_righClickMenu = new QMenu(this);
	m_righClickMenu->setStyleSheet(
		"QMenu::item:selected {background-color : #00CCFF;}");
	QAction* deleteAct = new QAction(QIcon(":/res/image/delete_svg.svg"), " 删除", this);
	QAction* setTopAct = new QAction(QIcon(":/res/image/top_svg.svg"), " 置顶", this);
	QAction* setBotAct = new QAction(QIcon(":/res/image/bottom_svg.svg"), " 置底", this);
	QAction* copyAct = new QAction(QIcon(":/res/image/copy_svg.svg"), " 复制", this);
	QAction* cutAct = new QAction(QIcon(":/res/image/cut_svg.svg"), " 剪切", this);
	QAction* pasteAct = new QAction(QIcon(":/res/image/paste_svg.svg"), " 粘贴", this);

	m_righClickMenu->addAction(deleteAct);
	m_righClickMenu->addSeparator();
	m_righClickMenu->addAction(setTopAct);
	m_righClickMenu->addSeparator();
	m_righClickMenu->addAction(setBotAct);
	m_righClickMenu->addSeparator();
	m_righClickMenu->addAction(copyAct);
	m_righClickMenu->addSeparator();
	m_righClickMenu->addAction(cutAct);
	m_righClickMenu->addSeparator();
	m_righClickMenu->addAction(pasteAct);

	connect(cutAct, SIGNAL(triggered()), this, SLOT(setCutItem()));
	connect(setBotAct, SIGNAL(triggered()), this, SLOT(setBotItem()));
	connect(copyAct, SIGNAL(triggered()), this, SLOT(setCopyItem()));
	connect(setTopAct, SIGNAL(triggered()), this, SLOT(setTopItem()));
	connect(pasteAct, SIGNAL(triggered()), this, SLOT(setPasteItem()));
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(setDeleteItem()));
	connect(this, SIGNAL(toCutItemVisbile(bool)), cutAct, SLOT(setEnabled(bool)));
	connect(this, SIGNAL(toCopyItemVisbile(bool)), copyAct, SLOT(setEnabled(bool)));
	connect(this, SIGNAL(toPasteItemVisible(bool)), pasteAct, SLOT(setEnabled(bool)));
	connect(this, SIGNAL(toDeleteItemVisible(bool)), deleteAct, SLOT(setEnabled(bool)));
	connect(this, SIGNAL(toRightButtonIsVisible(bool)), setBotAct, SLOT(setEnabled(bool)));
	connect(this, SIGNAL(toRightButtonIsVisible(bool)), setTopAct, SLOT(setEnabled(bool)));
}

KDrawingBoard::~KDrawingBoard()
{
	if (!m_selectedContainer.isEmpty())
		clearSelectedStatus();

	if (m_shapeList.isEmpty() && !m_copyContains.isEmpty())
	{
		//画板上没有图形而存储拷贝图形的容器中还有对象存在
		for (auto& e : m_copyContains)
			if (e)  delete e;
		m_copyContains.clear();
	}

	if (!m_shapeList.empty())
		for (auto& e : m_shapeList)
			delete e;
}

void KDrawingBoard::setCurrentShape(ShapeStyle s)
{
	if (s != m_currShapeCode)
		m_currShapeCode = s;
}

void KDrawingBoard::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	if (m_svgRead && m_svgRead->isValid())
		m_svgRead->render(&painter, this->rect());
	painter.scale(m_stretchRate, m_stretchRate);
	painter.setRenderHints(QPainter::Antialiasing, true);
	drawSelectedGraph(painter);
	if (m_showMultiRect && !m_startMousePoint.isNull() && !m_endMousePoint.isNull())
		drawMultiSelectRect(painter);
}

void KDrawingBoard::drawSelectedGraph(QPainter& painter)
{
	for (int i = 0; i < m_shapeList.size() && !m_shapeList.isEmpty(); i++)
	{
		m_shapeList[i]->setStretchRate(m_stretchRate);
		m_shapeList[i]->setShapeRect();
		m_shapeList[i]->paint(painter);
		if (m_shapeList[i]->getSelectedStatus()) //只绘制选中的那部分图形
		{
			if (m_selectedContainer.isEmpty() || m_selectedContainer.size() == 1)
			{
				m_shapeList[i]->initOutRect();//初始化对象选中状态的外侧小举行
				m_shapeList[i]->paintOutRect(painter);
				m_selectedShape = m_shapeList[i];
			}
			else if (!m_selectedContainer.isEmpty())
			{
				m_shapeList[i]->destoryOutRect();//需要将对象外侧的小矩形销毁
				drawMultiSelectedRect(m_shapeList[i], painter);
			}
		}//end of if
	}//end of for
}

void KDrawingBoard::drawMultiSelectRect(QPainter& painter)
{
	QPen pen = QPen(Qt::blue, 3);
	pen.setStyle(Qt::DotLine);
	m_multiSelectRect = QRectF(m_startMousePoint, m_endMousePoint);
	painter.setPen(pen);
	painter.drawRect(m_multiSelectRect);
}

void KDrawingBoard::drawMultiSelectedRect(ShapeBase* picture, QPainter& painter)
{
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setWidth(3);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(picture->getMultiSelectContainRect());
	painter.setPen(Qt::NoPen);
}

void KDrawingBoard::loadSvgDocument(QFileInfo& info, QString& fileName)
{
	if (m_svgRead == nullptr)
		m_svgRead = new QSvgRenderer();
	if (info.exists() && !fileName.isEmpty() && m_svgRead)
		m_svgRead->load(fileName);
}

void KDrawingBoard::saveWidgetContent(QString& filePath)
{
	if (filePath.isEmpty())
		return;

	qDebug() << "svg";
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "Error", "Write Error");
		return;
	}
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("svg");
	xmlWriter.writeAttribute("subset", QString("wps-wsx"));
	xmlWriter.writeAttribute("height", QString::number(m_currentWidgetSize.height()));
	xmlWriter.writeAttribute("width", QString::number(m_currentWidgetSize.width()));
	xmlWriter.writeAttribute("xmlns", QString("http://www.w3.org/2000/svg"));

	xmlWriter.writeStartElement("g");
	xmlWriter.writeAttribute("Shape_ID", "0");
	xmlWriter.writeStartElement("rect");
	xmlWriter.writeAttribute("fill", m_theWidgetBGColor.name());
	xmlWriter.writeAttribute("id", "canvas_background");
	xmlWriter.writeAttribute("height", QString::number(m_currentWidgetSize.height()));
	xmlWriter.writeAttribute("width", QString::number(m_currentWidgetSize.width()));
	xmlWriter.writeAttribute("stroke", QString::number(m_stretchRate));
	xmlWriter.writeAttribute("rateStage", QString::number(m_stretchRateStage));
	xmlWriter.writeAttribute("y", QString::number(this->rect().topLeft().y()));
	xmlWriter.writeAttribute("x", QString::number(this->rect().topLeft().x()));
	xmlWriter.writeEndElement();
	xmlWriter.writeEndElement();

	for (int i = 0; i < m_shapeList.size(); i++)
	{
		xmlWriter.writeStartElement("g");
		xmlWriter.writeAttribute("Shape_ID", QString::number(m_shapeList[i]->getGraphicalShapes()));
		m_shapeList[i]->writerShapeXml(xmlWriter);
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndDocument();
	file.close();
}

void KDrawingBoard::openSvgContentInTheWidget(QString& filePath)
{
	if (filePath.isEmpty())
		return;

	QFile file(filePath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;
	QXmlStreamReader xmlReader(&file);
	xmlReader.setDevice(&file);

	while (xmlReader.name() != "svg")
	{
		xmlReader.readNext();
	}

	if (xmlReader.attributes().value("subset") != "wps-wsx")
	{
		QFileInfo fileInfo(filePath);
		this->loadSvgDocument(fileInfo, filePath);
		return;
	}

	while (!xmlReader.atEnd() && !xmlReader.hasError())
	{
		if (xmlReader.isEndElement())
		{
			xmlReader.readNext();
			continue;
		}
		if (xmlReader.isStartElement())
		{
			if (xmlReader.name() == "g" && xmlReader.attributes().value("Shape_ID").toString() == "0")
			{
				xmlReader.readNext();
				xmlReader.readNext();
				this->m_stretchRate = xmlReader.attributes().value("stroke").toDouble();
				this->setStyleSheet("background-color:" + xmlReader.attributes().value("fill").toString() + ";border: 1px solid #B3B3B3;");
				m_stretchRateStage = xmlReader.attributes().value("rateStage").toDouble();
				m_currentWidgetSize = QSize(xmlReader.attributes().value("width").toDouble(), xmlReader.attributes().value("height").toDouble());
				this->resize(m_currentWidgetSize* m_stretchRate);
				emit toLaunchWidgetWidth(xmlReader.attributes().value("width").toDouble());
				emit toLaunchWidgetHigh(xmlReader.attributes().value("height").toDouble());
				emit toLaunchStretchRate(m_stretchRate);
			}
			else if (xmlReader.name() == "g" && xmlReader.attributes().value("Shape_ID").toInt() > 0)
			{
				int shapeID = xmlReader.attributes().value("Shape_ID").toInt();
				xmlReader.readNext();
				xmlReader.readNext();
				writeXmlToCurrentWidget(shapeID, xmlReader);
			}
		}
		xmlReader.readNext();
	}
	file.close();
}

bool KDrawingBoard::isDrawingBoardEmpty()
{
	return !m_shapeList.isEmpty();
}

void KDrawingBoard::setDrawBoardSize(const QSize size)
{
	m_currentWidgetSize =size ;
	this->resize(size * m_stretchRate);
}

bool KDrawingBoard::svgReaderIsValid()
{
	if (m_svgRead && m_svgRead->isValid())
		return true;
	return false;
}

void KDrawingBoard::writeXmlToCurrentWidget(int& shapeID, QXmlStreamReader& xmlReader)
{
	QPoint end;
	switch (shapeID)
	{
		case ShapeStyle::LINE:
		{
			m_shape = new KDrawLine(this);
			m_shape->setStart(QPoint((xmlReader.attributes().value("x2").toDouble()), (xmlReader.attributes().value("y2").toDouble())));
			m_shape->setEnd(QPoint(xmlReader.attributes().value("x1").toDouble(), xmlReader.attributes().value("y1").toDouble()));
			QString color = xmlReader.attributes().value("stroke").toString();
			fromRgbaToArgb(color);
			m_shape->setPenColor(color);
			m_shape->setStrokeWidth(xmlReader.attributes().value("stroke-width").toDouble());
			break;
		}
		case ShapeStyle::RECTS:
		{
			m_shape = new KDrawRect(this);
			m_shape->setStart(QPoint((xmlReader.attributes().value("x").toDouble()), (xmlReader.attributes().value("y").toDouble())));
			end = QPoint((xmlReader.attributes().value("x").toDouble()) + xmlReader.attributes().value("width").toDouble(), (xmlReader.attributes().value("y").toDouble()) + xmlReader.attributes().value("height").toDouble());
			m_shape->setEnd(end);
			m_shape->setStrokeWidth(xmlReader.attributes().value("stroke-width").toDouble());
			QString color = xmlReader.attributes().value("stroke").toString();
			fromRgbaToArgb(color);
			m_shape->setPenColor(color);
			color = xmlReader.attributes().value("fill").toString();
			fromRgbaToArgb(color);
			m_shape->setShapeColor(color);
			break;
		}
		case ShapeStyle::ELLIPSE:
		{
			while (xmlReader.name() != "point_station")
				xmlReader.readNext();

			m_shape = new KDrawEllipse(this);
			m_shape->setStart(QPoint(xmlReader.attributes().value("x2").toDouble(), xmlReader.attributes().value("y2").toDouble()));
			m_shape->setEnd(QPoint(xmlReader.attributes().value("x1").toDouble(), xmlReader.attributes().value("y1").toDouble()));
			QString color = xmlReader.attributes().value("stroke").toString();
			fromRgbaToArgb(color);
			m_shape->setPenColor(color);
			color = xmlReader.attributes().value("fill").toString();
			fromRgbaToArgb(color);
			m_shape->setShapeColor(color);
			m_shape->setStrokeWidth(xmlReader.attributes().value("stroke-width").toDouble());
			break;
		}
		case ShapeStyle::FREELINE:
		{
			QString str = xmlReader.attributes().value("d").toString();
			str.replace(0, 1, "L");
			str.remove("L");
			QStringList strContain = str.split(" ");
			m_shape = new KDrawFreeLine(this);
			m_isFreeLineDraw = true;
			m_shape->setStrokeWidth(xmlReader.attributes().value("stroke-width").toDouble());
			QString color = xmlReader.attributes().value("stroke").toString();
			fromRgbaToArgb(color);
			m_shape->setPenColor(color);
			m_shape->setStart(QPoint(strContain[1].toDouble(), strContain[2].toDouble()));
			KDrawFreeLine* tmpPtr = dynamic_cast<KDrawFreeLine*> (m_shape);
			m_shape->setEnd(QPoint(strContain[strContain.size() - 2].toDouble(), strContain[strContain.size() - 1].toDouble()));
			for (int i = 0; i < strContain.size() - 2; i++)
			{
				++i;
				tmpPtr->insertFileLinePoint(QPoint(strContain[i].toDouble(), strContain[(i + 1)].toDouble()));
				++i;
			}
			tmpPtr->setStartAndEndPoint();
			break;
		}
		case ShapeStyle::TRIANGLE:
		{
			while (xmlReader.name() != "point_station")
				xmlReader.readNext();

			m_shape = new KDrawTriangle(this);
			m_shape->setStart(QPoint(xmlReader.attributes().value("x2").toDouble(), xmlReader.attributes().value("y2").toDouble()));
			m_shape->setEnd(QPoint(xmlReader.attributes().value("x1").toDouble(), xmlReader.attributes().value("y1").toDouble()));
			QString color = xmlReader.attributes().value("stroke").toString();
			fromRgbaToArgb(color);
			m_shape->setPenColor(color);
			color = xmlReader.attributes().value("fill").toString();
			fromRgbaToArgb(color);
			m_shape->setShapeColor(color);
			m_shape->setStrokeWidth(xmlReader.attributes().value("stroke-width").toDouble());
			break;
		}
		case ShapeStyle::HEXAGON:
		{
			while (xmlReader.name() != "point_station")
				xmlReader.readNext();

			m_shape = new KDrawHexagon(this);
			m_shape->setStart(QPoint(xmlReader.attributes().value("x2").toDouble(), xmlReader.attributes().value("y2").toDouble()));
			m_shape->setEnd(QPoint(xmlReader.attributes().value("x1").toDouble(), xmlReader.attributes().value("y1").toDouble()));
			QString color = xmlReader.attributes().value("stroke").toString();
			fromRgbaToArgb(color);
			m_shape->setPenColor(color);
			color = xmlReader.attributes().value("fill").toString();
			fromRgbaToArgb(color);
			m_shape->setShapeColor(color);
			m_shape->setStrokeWidth(xmlReader.attributes().value("stroke-width").toDouble());
			break;
		}
		case ShapeStyle::TEXTBOX:
		{
			while (xmlReader.name() != "text")
				xmlReader.readNext();
			
			m_shape = new KDrawTextBox(this);
			m_shape->setStart(QPointF(xmlReader.attributes().value("x").toDouble()-20, xmlReader.attributes().value("y").toDouble()-20));
			//设置文本框中字体字号和X，Y坐标
			KDrawTextBox* tmpPtr = dynamic_cast<KDrawTextBox*>(m_shape);
			m_textBoxSpecial = tmpPtr->getTextBoxObject();
			m_shape->setStrokeWidth(m_strokeWidth);
			m_shape->setShapeColor(m_brushColor);
			QString color = xmlReader.attributes().value("stroke").toString();
			fromRgbaToArgb(color);
			m_shape->setPenColor(color);
			color = xmlReader.attributes().value("fill").toString();
			fromRgbaToArgb(color);
			m_shape->setShapeColor(color);
			m_textBoxSpecial->setText(xmlReader.readElementText());
			m_lineTextContain << m_shape;
			connect(tmpPtr, &KDrawTextBox::toDeleteTheObject, this, &KDrawingBoard::deleteTheObject);
			break;
		}
	}
	m_shapeList << m_shape;
}

void KDrawingBoard::mousePressEvent(QMouseEvent* ev)
{
	if (!m_shapeList.contains(m_selectedShape) && m_selectedShape)
	{
		delete m_selectedShape;
		m_selectedShape = nullptr;
	}

	QPoint tmpPoint = ev->pos();
	createShapeBase(m_currShapeCode, tmpPoint);//根据图形ID创建出不同的图形
	if (m_needEndPointFlag 
		&& !m_selectedContainer.isEmpty()
		&& ev->button() == Qt::RightButton )
			return;

	encapsulateMouseClickEvents(tmpPoint,ev);
	m_endMousePoint = tmpPoint / m_stretchRate;
}

void KDrawingBoard::encapsulateMouseClickEvents(const QPoint& point, QMouseEvent* ev)
{
	if (m_shape == nullptr && !m_shapeList.isEmpty() && m_selectedShape)
	{//点击选择进行二次编辑的外框小矩形
		m_clickOutRect = hitWhichOutRect(point / m_stretchRate);
		if (m_clickOutRect != HitOutRect::MOUSESTATUS)
		{
			this->setCursor(Qt::SizeAllCursor);
			m_mouseClickOutRect = true;
			m_showMultiRect = false;
			m_storageMovePosition = point / m_stretchRate;
			return;
		}
	}

	if (m_shape == nullptr)
	{//当前为鼠标状态可以选中某个图形进行拖拽
		if (m_mouseMultiSelection && !m_mouseMultiSelect)
			pointInMultiSelectRegion(point);
		else if (!m_mouseMultiSelect )
			pointInMouseClickRegion(point);
		update();
	}
	else if (m_shape && m_currShapeCode == ShapeStyle::FREELINE )
	{
		//自由画笔
		cancelMultiSelectionState();
		m_isFreeLineDraw = true;
		m_shapeList << m_shape;
		KDrawFreeLine* tmpPtr = dynamic_cast<KDrawFreeLine*> (m_shape);
		tmpPtr->insertFileLinePoint(point / m_stretchRate);
		m_shape->setStart(point / m_stretchRate);
		update();
		return;
	}
	else if (m_shape != nullptr)
	{
		cancelMultiSelectionState();
		if (m_currShapeCode == ShapeStyle::TEXTBOX)
			m_lineTextContain << m_shape;
		m_isFreeLineDraw = false;
		m_shapeList << m_shape;
		m_shape->setStart(point / m_stretchRate);
		m_shape->setEnd(point / m_stretchRate);
	}
}

void KDrawingBoard::createShapeBase(const ShapeStyle& shape, QPoint& point)
{
	switch (shape)
	{
		case ShapeStyle::MOUSE:
		{
			m_shape = nullptr;
			m_showMultiRect = true;
			m_startMousePoint = point / m_stretchRate;
			break;
		}
		case ShapeStyle::LINE:
		{
			m_shape = new KDrawLine(this);
			break;
		}
		case ShapeStyle::RECTS:
		{
			m_shape = new KDrawRect(this);
			break;
		}
		case ShapeStyle::ELLIPSE:
		{
			m_shape = new KDrawEllipse(this);
			break;
		}
		case ShapeStyle::FREELINE:
		{
			m_shape = new KDrawFreeLine(this);
			break;
		}
		case ShapeStyle::HEXAGON:
		{
			m_shape = new KDrawHexagon(this);
			break;
		}
		case ShapeStyle::TRIANGLE:
		{
			m_shape = new KDrawTriangle(this);
			break;
		}
		case ShapeStyle::TEXTBOX:
		{
			m_shape = new KDrawTextBox(this);
			KDrawTextBox* tmpPtr = dynamic_cast<KDrawTextBox*>(m_shape);
			m_textBoxSpecial = tmpPtr->getTextBoxObject();
			m_textBoxSpecial->setVisible(true);
			m_textBoxSpecial->setFocus();
			connect(tmpPtr, &KDrawTextBox::toDeleteTheObject, this , &KDrawingBoard::deleteTheObject);
			break;
		}
	}
	if (m_shape)
		m_shape->setStrokeWidth(m_strokeWidth);
}

HitOutRect KDrawingBoard::hitWhichOutRect(QPoint pos)
{
	return m_selectedShape->getSelectedBoxNum(pos);
}

void KDrawingBoard::mouseMoveEvent(QMouseEvent* ev)
{
	if (m_currShapeCode == ShapeStyle::FREELINE)
	{//为了自由线设置的图形变化
		QPixmap pixmap(":res/image/mouse_pen.svg");
		this->setCursor(QCursor(pixmap,5,35));
	}
	else if(m_mouseClickOutRect)
		this->setCursor(Qt::SizeAllCursor);
	else
		this->setCursor(Qt::ArrowCursor);

	encapsulateMouseMoveEvents(ev);
}

void KDrawingBoard::encapsulateMouseMoveEvents(QMouseEvent* ev)
{
	m_endMousePoint = ev->pos() / m_stretchRate; //不断更新框选的终点
	if (m_mouseClickOutRect && m_clickOutRect != HitOutRect::MOUSESTATUS && m_currShapeCode == ShapeStyle::MOUSE)
	{
		//根据此时的选中矩形来判断外矩形需要进行什么样的操作
		if (m_selectedShape)
			m_selectedShape->setSelectedStatus(true);
		changeShapeByMouse(ev->pos() / m_stretchRate);
		return;
	}
	if (ev->buttons() == Qt::LeftButton && m_shape == nullptr && !m_mouseClickSelect)
	{
		isMultiSelectProcess(ev->pos() / m_stretchRate);
	}
	else if (ev->buttons() == Qt::LeftButton && m_shape == nullptr && m_showMultiRect == false)
	{
		if (m_selectedShape != nullptr && m_selectedShape->getSelectedStatus())
		{
			isMouseClickMove(ev->pos() / m_stretchRate);
		}
	}
	else if (ev->buttons() == Qt::LeftButton && m_isFreeLineDraw == true && m_shape && m_currShapeCode == ShapeStyle::FREELINE)
	{
		//自由画笔
		KDrawFreeLine* tmpPtr = dynamic_cast<KDrawFreeLine*> (m_shape);
		tmpPtr->insertFileLinePoint(ev->localPos() / m_stretchRate);
	}
	else if (ev->buttons() == Qt::LeftButton && m_shape) {
		m_shape->setEnd(ev->pos() / m_stretchRate);
	}
	update();
}

void KDrawingBoard::isMouseClickMove(QPoint point)
{
	//封装了单个选中某个图形进行移动的过程
	double width = point.x() - m_storageMovePosition.x();
	double high = point.y() - m_storageMovePosition.y();
	if (m_selectedShape->getGraphicalShapes() == ShapeStyle::FREELINE)
	{
		KDrawFreeLine* tmpPtr = dynamic_cast<KDrawFreeLine*> (m_selectedShape);
		tmpPtr->setStartAndEndPoint();
	}
	m_selectedShape->reSetShapePosition(width, high);
	m_storageMovePosition = point;
}

void KDrawingBoard::isMultiSelectProcess(QPoint point)
{
	//封装了框选和框选的移动
	if (m_mouseMultiSelect)
	{//此时为框选的移动
		int width = point.x() - m_storageMovePosition.x();
		int high = point.y() - m_storageMovePosition.y();
		for (auto& e : m_selectedContainer)
		{
			e->reSetShapePosition(width, high);
			if (e->getGraphicalShapes() == ShapeStyle::FREELINE)
			{
				KDrawFreeLine* ptr = dynamic_cast<KDrawFreeLine*>(e);
				ptr->setStartAndEndPoint();
			}
		}
		m_storageMovePosition = point;
		update();
		return;  //return的原因是已经框选完毕，现在需要的是移动。
	}
	//此时是框选
	if (!m_selectedContainer.isEmpty())
	{
		clearSelectedStatus();
		//若之前已经框选过，且没有清空记录的容器，再次框选会直接显示框选容器中选中的内容
		//需要进行清空操作,之后再次选择.
	}

	for (int size = 0; size < m_shapeList.size(); size++)
	{
		m_shapeList[size]->setShapeRect();
		if (m_multiSelectRect.intersects(m_shapeList[size]->getMultiSelectContainRect()))
		{
			m_selectedContainer << m_shapeList[size];
			m_shapeList[size]->setSelectedStatus(true);
			m_showMultiRect = true;
			m_mouseMultiSelection = true;
		}
	}
	if (m_selectedContainer.size() > 1  && !m_selectedShape)
		emit toRightButtonIsVisible(false);
}

void KDrawingBoard::changeShapeByMouse(const QPoint& pos)
{
	int width = pos.x() - m_storageMovePosition.x();
	int high = pos.y() - m_storageMovePosition.y();
	m_storageMovePosition = pos;

	ShapeStyle shape = m_selectedShape->getGraphicalShapes();
	KDrawFreeLine* ptr = nullptr;
	QRectF oldRect;
	if (shape == ShapeStyle::FREELINE)
	{
		ptr = dynamic_cast<KDrawFreeLine*> (m_selectedShape);
		oldRect = ptr->getXmlRect();
	}
	switch (m_clickOutRect)
	{
		case HitOutRect::TOPLEFTRECT:
		{//情况1:点击选中图形的左上角
			m_selectedShape->reSetStartPoint(width, high);
			break;
		}
		case HitOutRect::LEFTMIDDLERECT:
		{//情况2:点击选中图形的左中部
			m_selectedShape->reSetStartPoint(width, 0);
			break;
		}
		case HitOutRect::BOTTOMLEFTRECT:
		{//情况3:点击选中图形的左下角
			m_selectedShape->reSetStartPoint(width, 0);
			m_selectedShape->reSetEndPoint(0, high);
			break;
		}
		case HitOutRect::BOTTOMMIDDLERECT:
		{//情况4:点击选中图形的下边中间
			m_selectedShape->reSetEndPoint(0, high);
			break;
		}
		case HitOutRect::BOTTOMRIGHTRECT:
		{//情况5:点击选中图形的右下角
			m_selectedShape->reSetEndPoint(width,high);
			break;
		}
		case HitOutRect::RIGHTMIDDLERECT:
		{//情况6:点击选中图形的you部中间
			m_selectedShape->reSetEndPoint(width, 0);
			break;
		}
		case HitOutRect::TOPRIGHTRECT:
		{//情况7:点击选中图形的右上角
			m_selectedShape->reSetStartPoint(0, high);
			m_selectedShape->reSetEndPoint(width, 0);
			break;
		}
		case HitOutRect::TOPMIDDLERECT:
		{//情况8:点击选中图形的上边中间
			m_selectedShape->reSetStartPoint(0, high);
			break;
		}
	}
	if (shape == ShapeStyle::FREELINE)
	{
		ptr->reSetEveryPoint(oldRect);
	}
	update();
}

void KDrawingBoard::mouseReleaseEvent(QMouseEvent* ev)
{
	m_endMousePoint = ev->pos() / m_stretchRate;
	encapsulateMouseReleaseEvents(ev);
}

void  KDrawingBoard::encapsulateMouseReleaseEvents(QMouseEvent* ev)
{
	if (m_mouseClickOutRect)
	{
		m_mouseClickOutRect = false;
		this->setCursor(Qt::ArrowCursor);
	}

	m_mouseClickSelect = false;
	m_mouseMultiSelect = false;

	if (m_isFreeLineDraw == true && m_shape && m_currShapeCode == ShapeStyle::FREELINE)
	{
		//自由画笔
		m_shape->setEnd(ev->localPos() / m_stretchRate);
		KDrawFreeLine* tmpPtr = dynamic_cast<KDrawFreeLine*> (m_shape);
		tmpPtr->insertFileLinePoint(ev->localPos() / m_stretchRate);
	}

	if (ev->button() == Qt::LeftButton && m_shape)
	{
		m_selectedShape = m_shape;
		m_shape->setSelectedStatus(true);
		emit changeCurrentIndex(1);
		if (m_shape->getGraphicalShapes() == ShapeStyle::TEXTBOX)
		{
			KDrawTextBox* tmpPtr = dynamic_cast<KDrawTextBox*>(m_shape);
			m_textBoxSpecial = tmpPtr->getTextBoxObject();
			if (m_textBoxSpecial->isVisible())
				m_shape->setSelectedStatus(false);
		}

		if (m_shape->getGraphicalShapes() == ShapeStyle::FREELINE)
		{
			KDrawFreeLine* tmpPtr = dynamic_cast<KDrawFreeLine*> (m_shape);
			tmpPtr->setStartAndEndPoint();
		}
	}
	m_showMultiRect = false;
	m_startMousePoint = QPoint();
	m_endMousePoint = QPoint();
	update();
}

void KDrawingBoard::fromRgbaToArgb(QString& str)
{
	QString subTwo = str.mid(7, 2);
	str.remove(7, 2);
	str.insert(1, subTwo);
}
void KDrawingBoard::wheelEvent(QWheelEvent* ev)
{
	QPoint numDegrees;
	numDegrees = ev->angleDelta();
	double step = 0;
	if (!numDegrees.isNull())
	{
		step = numDegrees.y() / 120;
	}

	ev->accept();
	m_stretchRateStage += step;
	m_stretchRate = 1 + m_stretchRateStage / 20;
	if (m_stretchRate < 0.1)
	{
		m_stretchRate = 0.1;
		m_stretchRateStage = -18;
	}
	if (m_stretchRate > 10)
	{
		m_stretchRate = 10;
		m_stretchRateStage = 180;
	}
	emit toLaunchStretchRate(m_stretchRate);
	this->resize(m_currentWidgetSize * m_stretchRate);
}

void KDrawingBoard::contextMenuEvent(QContextMenuEvent* ev)
{
	qDebug() << "当前有对象:" << m_shapeList.size();
	if (m_currShapeCode == ShapeStyle::MOUSE)//当前是鼠标状态才显示右键菜单
	{
		m_righClickMenu->show();
		m_righClickMenu->move(ev->globalPos());

		if (m_copyContains.isEmpty())
			emit toPasteItemVisible(false);
		else
			emit toPasteItemVisible(true);
		if (m_selectedShape == nullptr && m_selectedContainer.isEmpty())
		{
			emit toDeleteItemVisible(false);
			emit toCopyItemVisbile(false);
			emit toCutItemVisbile(false);
		}
		else if (m_selectedShape && !m_selectedContainer.isEmpty())
		{
			emit toDeleteItemVisible(true);
			emit toCopyItemVisbile(true);
			emit toCutItemVisbile(true);
		}
	}
	m_storageMovePosition = ev->pos();
}

void KDrawingBoard::setDeleteItem()
{
	if (m_selectedShape && m_mouseMultiSelection == false)
		m_mouseMultiSelection = true;
	if (m_shapeList.contains(m_selectedShape) &&  !m_selectedContainer.contains(m_selectedShape)
		&& m_selectedShape != nullptr)
		m_selectedContainer << m_selectedShape;
	
	for (int i = 0; i< m_selectedContainer.size(); i ++)
	{
		ShapeBase* item = m_selectedContainer[i];
		m_shapeList.removeOne(item);
		if (m_lineTextContain.contains(item))
			m_lineTextContain.removeOne(item);
		delete item;
	}
	m_selectedContainer.clear();
	m_selectedShape = nullptr;
}

void KDrawingBoard::setTopItem()
{
	if (m_selectedShape)
	{
		m_shapeList.removeOne(m_selectedShape);
		m_shapeList.push_back(m_selectedShape);
	}
}

void KDrawingBoard::setBotItem()
{
	if (m_selectedShape)
	{
		m_shapeList.removeOne(m_selectedShape);
		m_shapeList.push_front(m_selectedShape);
	}
}

void KDrawingBoard::setCopyItem()
{
	if (!m_selectedShape)
		return;
	m_needEndPointFlag = false;

	for (auto& e : m_copyContains)
	{
		ShapeBase* item = e;
		m_copyContains.removeOne(item);
		delete item;
	}

	if (m_shapeList.contains(m_selectedShape) && !m_selectedContainer.contains(m_selectedShape))
		m_selectedContainer << m_selectedShape;

	int countShapeNum = 0;
	for (auto& e : m_shapeList)
	{
		if (m_selectedContainer.contains(e))
		{
			ShapeStyle shape = e->getGraphicalShapes();//获取选中图形的类型
			QPointF tmpPoint = e->getStartPoint();
			createShapeBase(shape, QPoint(tmpPoint.x(), tmpPoint.y()));
			//使用图形ID可以new出不同对象，记录当前对象的属性.
			if (e->getGraphicalShapes() == ShapeStyle::TEXTBOX) //处理文本的情况
			{
				KDrawTextBox* ptr = dynamic_cast<KDrawTextBox*>(m_shape);
				m_textBoxSpecial = ptr->getTextBoxObject();
				m_textBoxSpecial->setVisible(false);
				m_lineTextContain << m_shape;
				ptr->copyOtherTextLine(e);
			}
			else if (e->getGraphicalShapes() == ShapeStyle::FREELINE)//自由线需要进行特殊处理
			{
				KDrawFreeLine* ptr = dynamic_cast<KDrawFreeLine*>(m_shape);
				ptr->copyOtherFreeLinePoint(e);
			}
			m_shape->copyOtherCharacter(e);
			m_copyContains << m_shape;
			countShapeNum++;
		}
		if (countShapeNum == m_selectedContainer.size())
			break;
	}
	clearSelectedStatus();
	m_needEndPointFlag = true;
	m_selectedShape = nullptr;
}

void KDrawingBoard::setCutItem()
{
	if (!m_selectedShape)
		return;
	m_needEndPointFlag = false;
	for (auto& e : m_copyContains)
	{
		ShapeBase* item = e;
		m_copyContains.removeOne(item);
		delete item;
	}
	m_copyContains.clear();

	if (m_shapeList.contains(m_selectedShape) && !m_selectedContainer.contains(m_selectedShape))
		m_selectedContainer << m_selectedShape;

	for (auto& e : m_selectedContainer)
	{
		if (m_shapeList.contains(e))
		{
			m_copyContains << e;
			m_shapeList.removeOne(e);
		}
	}
	m_selectedContainer.clear();
	m_needEndPointFlag = true;
	m_selectedShape = nullptr;
}

void KDrawingBoard::setPasteItem()
{
	if (m_copyContains.isEmpty())
		return; //没有复制到任何的内容，不需要再走下面逻辑

	if (m_selectedShape)
		m_selectedShape->setSelectedStatus(false);
	clearSelectedStatus();
	
	for (auto &e:m_copyContains)
	{
		e->reSetShapePosition(20, 20);
		e->setSelectedStatus(true);
		m_selectedContainer << e;
		if (e->getGraphicalShapes() == ShapeStyle::FREELINE)
		{
			KDrawFreeLine* ptr = dynamic_cast<KDrawFreeLine*>(e);
			ptr->setStartAndEndPoint();
		}
		m_shapeList.push_back(e);//将所选的复制内容全部储存到存储画板图形的容器中
	}
	m_selectedShape = nullptr;
	m_needEndPointFlag = false;
	m_copyContains.clear();//将用来存储复制内容的容器清空
}

void KDrawingBoard::setSwitchObject(bool flag)
{
	m_switchColor = flag;
}

void KDrawingBoard::deleteTheObject(ShapeBase* shape)
{
	m_selectedShape = shape;
	m_shapeList.removeOne(shape);
	m_copyContains.removeOne(shape);
	m_selectedContainer.removeOne(shape);
	m_lineTextContain.removeOne(shape);
}

void KDrawingBoard::mouseDoubleClickEvent(QMouseEvent* ev)
{
	bool flag = false;
	if (m_lineTextContain.isEmpty())
		return;
	if (!m_selectedContainer.isEmpty())
		clearSelectedStatus();

	for (int i = m_lineTextContain.size() - 1; i >= 0; i--)
	{
		if (!m_lineTextContain[i])
			m_lineTextContain.removeOne(m_lineTextContain[i]);
		else 
			if (m_lineTextContain[i]->containsPoint(ev->pos()))
			{
				KDrawTextBox* tmpPtr = dynamic_cast<KDrawTextBox*>(m_lineTextContain[i]);
				m_textBoxSpecial = tmpPtr->getTextBoxObject();
				m_selectedShape = m_lineTextContain[i];
				m_lineTextContain[i]->setSelectedStatus(false);
				m_textBoxSpecial->setFocus();
				flag = true;
				break;
			}
	}

	if (flag)
	{
		m_textBoxSpecial->setVisible(true); //双击之后显示文本编辑框
		m_selectedShape->getMultiSelectContainRect();
	}
}

void KDrawingBoard::clearCanvas()
{
	for (auto& e : m_shapeList)
		delete e;
	if (!m_copyContains.isEmpty())
	{
		for (auto& e : m_copyContains)
			if (e) delete e;
	}
	if (m_svgRead && m_svgRead->isValid())
	{
		delete m_svgRead;
		m_svgRead = nullptr;
	}

	m_copyContains.clear();
	m_shapeList.clear();
	m_lineTextContain.clear();
	m_selectedContainer.clear();
	m_selectedShape = nullptr;
}

void KDrawingBoard::clearSelectedStatus()
{
	for (auto& e : m_selectedContainer)
		e->setSelectedStatus(false);
	m_selectedContainer.clear();
}

void KDrawingBoard::pointInMultiSelectRegion(const QPoint& point)
{
	m_showMultiRect = true;
	bool containsFlag = false;
	for (auto& e : m_selectedContainer)
	{
		if (!e)
			m_selectedContainer.removeOne(e);
		else 
			if (e->containsPoint(point))
			{
				m_storageMovePosition = point / m_stretchRate;
				containsFlag = true;
				m_showMultiRect = false;
				m_mouseMultiSelect = true;
				m_selectedShape = e;
				break;
			}
	}

	if (containsFlag == false)
	{
		clearSelectedStatus();
		//在框选状态下，鼠标的点击点不在框选选中的图形上则取消这些图形的框选状态
		pointInMouseClickRegion(point);
		//可能此时的点击点不在框选的队列中而在其他图形上
	}
}

void KDrawingBoard::pointInMouseClickRegion(const QPoint& point)
{
	bool flags = false;
	if (!m_selectedContainer.isEmpty())
		clearSelectedStatus();

	for (int i = m_shapeList.size() - 1; i >= 0; i--)
	{
		if (m_shapeList[i]->getSelectedStatus())
		{
			m_shapeList[i]->setSelectedStatus(false);
		}

		if (m_shapeList[i]->containsPoint(point))
		{
			if (m_selectedShape != m_shapeList[i] && m_selectedShape)
			{		//这里的操作是判断选中的是否为当前m_selectedShape所指向的哪个图形
				m_selectedShape->setSelectedStatus(false);
			}
			emit toRightButtonIsVisible(true);
			m_selectedShape = m_shapeList[i];
			m_selectedContainer << m_selectedShape;
			m_selectedShape->setSelectedStatus(true);
			//单选的时候直接用m_selectedShape记录选中的对象
			m_storageMovePosition = point / m_stretchRate;
			m_showMultiRect = false; //用来展示是否需要展示框选图
			flags = true;
			m_mouseClickSelect = true;
			m_mouseMultiSelect = false;
			m_mouseMultiSelection = false;
			break;
		}
	}

	if (!flags && m_selectedShape)
	{
		m_selectedShape->setSelectedStatus(false);
		//本次没有单独选中某个图形则需将记录选中图形指针所指对象状态置为未被选中
		m_selectedShape = nullptr;
		emit toRightButtonIsVisible(false);
		emit changeCurrentIndex(0);
	}
	else if (!m_selectedShape)
	{
		emit toRightButtonIsVisible(false);
		emit changeCurrentIndex(0);
	}
	else if (m_selectedShape)
		emit changeCurrentIndex(1);
}

void KDrawingBoard::cancelMultiSelectionState()
{		//取消框选的状态
	if (!m_selectedContainer.isEmpty())
		clearSelectedStatus();

	for (int i = 0; i < m_shapeList.size(); i++)
		m_shapeList[i]->setSelectedStatus(false);
}

void KDrawingBoard::setDrawingBoardColor(const QColor& color)
{
	if (color.isValid())
		m_theWidgetBGColor = color;

	this->setStyleSheet("background-color:" + m_theWidgetBGColor.name() + ";border: 1px solid #B3B3B3;");
}


void KDrawingBoard::setBrushColor(QColor color)
{
	if (!color.isValid())
		return;

	if (m_mouseClickSelect && m_selectedShape->getSelectedStatus())
	{
		//此处是单选选中某个图形
		if (m_switchColor)
			m_selectedShape->setPenColor(color);
		else
			m_selectedShape->setShapeColor(color);
	}
	else
	{
		//此处是框选选中某些图形
		for (int i = 0; i < m_shapeList.size(); i++)
		{
			if (m_shapeList[i]->getSelectedStatus())
			{
				if (m_switchColor)
					m_shapeList[i]->setPenColor(color);

				else
					m_shapeList[i]->setShapeColor(color);
			}
		}
	}
	update();
}

void KDrawingBoard::setStrokeWidth(int& size)
{
	m_strokeWidth = size;
	if (m_mouseClickSelect && m_selectedShape->getSelectedStatus())
	{
		//此处是单选选中某个图形
		m_selectedShape->setStrokeWidth(m_strokeWidth);
	}
	else
	{
		for (int i = 0; i < m_shapeList.size(); i++)
		{
			if (m_shapeList[i]->getSelectedStatus())
			{
				m_shapeList[i]->setStrokeWidth(m_strokeWidth);
			}
		}
	}
	update();
}