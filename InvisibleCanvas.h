#include "qdialog.h"
#include "qrubberband.h"
#include "qevent.h"
#include <windows.h>
#include "qlabel.h"
#include "qobject.h"
#include <functional>

class InvisibleCanvas: public QDialog
{
public:
	InvisibleCanvas(RECT& rect);

    void showCanvas(RECT windowSize);
    void mousePressEvent(QMouseEvent* event) override;
    void InvisibleCanvas::mouseMoveEvent(QMouseEvent* event) override;
    void InvisibleCanvas::mouseReleaseEvent(QMouseEvent* event) override;

private:
    std::function<void(RECT)> m_callback;
    void loadImage();
    QRubberBand* m_rubberBand = nullptr;
    QPoint m_origin;
    QPoint m_endPoint;
    QLabel* m_imageLabel;
    RECT& m_roiRect;
};