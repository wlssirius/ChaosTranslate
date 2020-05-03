#define NOMINMAX
#include "InvisibleCanvas.h"
#include "QGraphicsOpacityEffect"
#include "QLayout"
#include "QImageReader"


InvisibleCanvas::InvisibleCanvas(Mode mode):
	QDialog(nullptr, 0),
    m_mode(mode)
{
    m_imageLabel = new QLabel;
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_imageLabel);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
}

void InvisibleCanvas::showCanvas(PIX* pix, RECT rect)
{
    setGeometry(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    QImage qImage(pix->w, pix->h, QImage::Format_ARGB32);
    for (int y = 0; y < pix->h; y++)
    {
        QRgb* destrow = (QRgb*)qImage.scanLine(y);
        for (int x = 0; x < pix->w; x++)
        {
            l_int32 r = 0;
            l_int32 g = 0;
            l_int32 b = 0;           
            pixGetRGBPixel(pix, x, y, &r, &g, &b);
            destrow[x] = qRgba(r, g, b, 255);
        }
    }
    m_imageLabel->setPixmap(QPixmap::fromImage(qImage));
}

void InvisibleCanvas::mousePressEvent(QMouseEvent* event)
{
    if (m_mode == Mode::Color)
    {
        m_origin = event->pos();
        if (!m_rubberBand)
            m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        m_rubberBand->setGeometry(QRect(m_origin, QSize()));
        m_rubberBand->show();
    }
}

void InvisibleCanvas::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mode == Mode::Color)
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }
}

void InvisibleCanvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_mode == Mode::ROI)
    {
        m_endPoint = event->pos();
        m_rubberBand->hide();
        int x0 = m_origin.x();
        int x1 = m_endPoint.x();
        m_roiRect.left = std::min(x0, x1);
        m_roiRect.right = std::max(x0, x1);
        int y0 = m_origin.y();
        int y1 = m_endPoint.y();
        m_roiRect.top = std::min(y0, y1);
        m_roiRect.bottom = std::max(y0, y1);
        emit setROI(m_roiRect);
    }
    else
    {
        int x = event->pos().x();
        int y = event->pos().y();
        QColor color = m_imageLabel->pixmap()->toImage().pixelColor(x, y);
        emit setColor(color);
    }
    this->destroy();
}
