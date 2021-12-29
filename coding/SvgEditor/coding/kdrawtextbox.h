#ifndef KDRAWTEXTBOX_H
#define KDRAWTEXTBOX_H
#pragma once
#include "shapebase.h"

class KDrawTextBox : public ShapeBase
{
	Q_OBJECT
public:
	KDrawTextBox();
	KDrawTextBox(QWidget* parent);
	~KDrawTextBox();
	void paint(QPainter& painter);
	bool containsPoint(const QPointF&);
	void setShapeRect();
	void reSetShapePosition(const double& width, const double& high);
	void writerShapeXml(QXmlStreamWriter& xmlWriter);
	QLineEdit* getTextBoxObject();
	void copyOtherTextLine(ShapeBase* other);
signals:
	void toDeleteTheObject(ShapeBase*);
private slots:
	void setLineEditVisible();
private:
	QFont m_textFontStatus = QFont();
	QLineEdit* m_textLine = nullptr;
	double m_lineTextHigh = 0.0;
	double m_lineTextWidth = 0.0;
};

#endif // KDRAWTEXTBOX_H
