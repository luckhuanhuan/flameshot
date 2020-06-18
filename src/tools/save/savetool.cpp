// Copyright(c) 2017-2019 Alejandro Sirgo Rica & Contributors
//
// This file is part of Flameshot.
//
//     Flameshot is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     Flameshot is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with Flameshot.  If not, see <http://www.gnu.org/licenses/>.

#include "savetool.h"
#include "src/utils/screenshotsaver.h"
#include <QPainter>
#include <QStandardPaths>
SaveTool::SaveTool(QObject *parent) : AbstractActionTool(parent) {

}

bool SaveTool::closeOnButtonPressed() const {
    return true;
}

QIcon SaveTool::icon(const QColor &background, bool inEditor) const {
    //Q_UNUSED(inEditor);
    //return QIcon(iconPath(background) + "content-save.svg");
    Q_UNUSED(background);
    /*return inEditor ?  QIcon(QStringLiteral(":/img/material/black/") + "content-save.svg") :
                      QIcon(QStringLiteral(":/img/material/white/") + "content-save.svg");*/
    return  QIcon();
}
QString SaveTool::name() const {
    return tr("Save");
}

QString SaveTool::nameID() {
    return QLatin1String("");
}

QString SaveTool::description() const {
    return tr("Save the capture");
}

CaptureTool* SaveTool::copy(QObject *parent) {
    return new SaveTool(parent);
}

void SaveTool::pressed(const CaptureContext &context) {
    QStringList a = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    if (context.savePath.isEmpty()) {
        emit requestAction(REQ_HIDE_GUI);
        if (context.saveType.isEmpty())
        {
            bool ok = ScreenshotSaver().saveToFilesystem(
                    context.selectedScreenshotArea(), a.at(0), ".png");
            if (ok) {
            emit requestAction(REQ_CAPTURE_DONE_OK);
            }
        }
        else {
            bool ok = ScreenshotSaver().saveToFilesystem(
                    context.selectedScreenshotArea(), a.at(0), context.saveType);
            if (ok) {
            emit requestAction(REQ_CAPTURE_DONE_OK);
            }
       }
    }
    else {
        bool ok = ScreenshotSaver().saveToFilesystem(
                    context.selectedScreenshotArea(), context.savePath, context.saveType);
        if (ok) {
            emit requestAction(REQ_CAPTURE_DONE_OK);
        }
    }
}
