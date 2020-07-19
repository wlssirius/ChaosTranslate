
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