#pragma once

#include <QObject>
#include <QLabel>

class ViewLabel  : public QLabel
{
	Q_OBJECT

public:
	ViewLabel(QWidget *parent);
	~ViewLabel();

	void show(const QRect& rect, const QPixmap& pixmap);
	void show(int x, int y, int w, int h, const QPixmap& pixmap);
	void show(const QSize& size, const QPixmap& pixmap);

};
