/*
*   Copyright (C) 2019-2020  Wei Lisi (Willis) <weilisi16@gmail.com>
*	This file is part of ChaosTranslate
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SELECTION_CANVAS_HEADER
#define SELECTION_CANVAS_HEADER

#include "qdialog.h"
#include "qrubberband.h"
#include "qevent.h"
#include <windows.h>
#include "qlabel.h"
#include "qobject.h"
#include <functional>
#include <memory>
#include <QColor>


class SelectionCanvas: public QDialog
{
    Q_OBJECT
public:
    enum class Mode
    {
        ROI,
        Color
    };
	SelectionCanvas(Mode mode);
    void showCanvas(std::shared_ptr<QImage> img, RECT rect);
    void mousePressEvent(QMouseEvent* event) override;
    void SelectionCanvas::mouseMoveEvent(QMouseEvent* event) override;
    void SelectionCanvas::mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void setROI(RECT rect);
    void setColor(QColor color);

private:
    Mode m_mode;
    QRubberBand* m_rubberBand = nullptr;
    QPoint m_origin;
    QPoint m_endPoint;
    QLabel* m_imageLabel;
    RECT m_roiRect;
};

#endif