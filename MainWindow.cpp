/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2015 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
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

#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebInspector>
#include <QtWebKitWidgets/QWebPage>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);
	m_ui->webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	m_ui->webView->settings()->setAttribute(QWebSettings::JavaEnabled, true);
	m_ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

	QWebInspector *inspector = new QWebInspector(m_ui->webView);
	inspector->setPage(m_ui->webView->page());
	inspector->setContextMenuPolicy(Qt::NoContextMenu);

	m_ui->splitter->addWidget(inspector);

	connect(m_ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(addressChanged()));
	connect(m_ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::addressChanged()
{
	m_ui->webView->load(QUrl(m_ui->lineEdit->text()));
}

void MainWindow::urlChanged(const QUrl &url)
{
	m_ui->lineEdit->setText(url.toString());
}
