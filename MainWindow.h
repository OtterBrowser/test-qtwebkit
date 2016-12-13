/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2015 - 2016 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QQueue>
#include <QtWidgets/QMainWindow>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebInspector>
#include <QtWebKitWidgets/QWebPage>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected slots:
	void cloneHistory();
	void acceptFeatureRequest();
	void addressChanged();
	void featurePermissionRequested(QWebFrame *frame, QWebPage::Feature feature);
	void urlChanged(const QUrl &url);
	void setZoom(int zoom);

private:
	QWebInspector *m_inspector;
	QQueue<QPair<QWebFrame*, QWebPage::Feature> > m_featureRequests;
	Ui::MainWindow *m_ui;
};

#endif
