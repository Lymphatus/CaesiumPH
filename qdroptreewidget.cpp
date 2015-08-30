/*******************************************************************************
#
# Copyright (C) 2010-2013 Matteo Paonessa <matteo.paonessa@gmail.com>
#
# This file is part of the Caesium distribution.
#
# Caesium is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# Caesium is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Caesium; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301 USA.
#
# Author: Matteo Paonessa <matteo.paonessa@gmail.com>
#
# ******************************************************************************/

#include "qdroptreewidget.h"
#include "caesiumph.h"

#include <QStringList>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMetaObject>

QDropTreeWidget::QDropTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{

}

void QDropTreeWidget::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void QDropTreeWidget::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}

void QDropTreeWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    QList<QUrl> urlList = mimeData->urls();
    QStringList fileList;
    if (mimeData->hasFormat("text/uri-list")) {
        foreach (QUrl url, urlList) {
            fileList << url.toLocalFile();
        }
    }
    event->acceptProposedAction();
    emit dropFinished(fileList);
}
