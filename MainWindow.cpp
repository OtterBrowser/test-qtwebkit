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

#include <QtCore/QTimer>
#include <QtWebKit/QWebHistory>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	m_inspector(NULL),
	m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);
	m_ui->webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	m_ui->webView->settings()->setAttribute(QWebSettings::JavaEnabled, true);
	m_ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

	m_inspector = new QWebInspector(m_ui->webView);
	m_inspector->setPage(m_ui->webView->page());
	m_inspector->setContextMenuPolicy(Qt::NoContextMenu);

	m_ui->splitter->addWidget(m_inspector);

	const QStringList arguments = QCoreApplication::arguments();

	if (arguments.count() > 1)
	{
		m_ui->webView->load(QUrl::fromUserInput(arguments.at(1)));
	}

	connect(m_ui->addressLineEdit, SIGNAL(returnPressed()), this, SLOT(addressChanged()));
	connect(m_ui->goButton, SIGNAL(clicked()), this, SLOT(addressChanged()));
	connect(m_ui->cloneButton, SIGNAL(clicked()), this, SLOT(cloneHistory()));
	connect(m_ui->zoomSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));
	connect(m_ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
	connect(m_ui->webView->page(), SIGNAL(featurePermissionRequested(QWebFrame*,QWebPage::Feature)), this, SLOT(featurePermissionRequested(QWebFrame*,QWebPage::Feature)));
}

MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::cloneHistory()
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::ReadWrite);
	stream << *(m_ui->webView->page()->history());

	QWebPage *page = new QWebPage(m_ui->webView);
	page->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	page->settings()->setAttribute(QWebSettings::JavaEnabled, true);
	page->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

	m_ui->webView->setPage(page);

	m_inspector->setPage(page);

	stream.device()->reset();
	stream >> *(page->history());
}

void MainWindow::acceptFeatureRequest()
{
	if (!m_featureRequests.isEmpty())
	{
		const QPair<QWebFrame*, QWebPage::Feature> featureRequest = m_featureRequests.dequeue();

		m_ui->webView->page()->setFeaturePermission(featureRequest.first, featureRequest.second, QWebPage::PermissionGrantedByUser);
	}
}

void MainWindow::addressChanged()
{
	m_ui->webView->load(QUrl::fromUserInput(m_ui->addressLineEdit->text()));
}

void MainWindow::featurePermissionRequested(QWebFrame *frame, QWebPage::Feature feature)
{
	if (m_ui->featureRequestsPolicyComboBox->currentIndex() == 1)
	{
		m_ui->webView->page()->setFeaturePermission(frame, feature, QWebPage::PermissionGrantedByUser);
	}
	else if (m_ui->featureRequestsPolicyComboBox->currentIndex() == 2)
	{
		m_featureRequests.enqueue(qMakePair(frame, feature));

		QTimer::singleShot(2500, this, SLOT(acceptFeatureRequest()));
	}
}

void MainWindow::urlChanged(const QUrl &url)
{
	m_ui->addressLineEdit->setText(url.toString());
}

void MainWindow::setZoom(int zoom)
{
	m_ui->webView->setZoomFactor(zoom / 100.);
}
