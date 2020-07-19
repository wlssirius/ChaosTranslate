#define NOMINMAX
#include "SelectionCanvas.h"
#include "QGraphicsOpacityEffect"
#include "QLayout"
#include "QImageReader"


SelectionCanvas::SelectionCanvas(Mode mode):
	QDialog(nullptr, 0),
    m_mode(mode)
{
    m_imageLabel = new QLabel;
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_imageLabel);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    if (mode == Mode::Color)
    {
        setWindowTitle(tr("Please pick target font color"));
    }
    else if (mode == Mode::ROI)
    {
        setWindowTitle(tr("Please select target capture region"));
    }
}

void SelectionCanvas::showCanvas(std::shared_ptr<QImage> qImage, RECT rect)
{
    setGeometry(0, 0, rect.right - rect.left, rect.bottom - rect.top);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    m_imageLabel->setPixmap(QPixmap::fromImage(*qImage));
    show();
}

void SelectionCanvas::mousePressEvent(QMouseEvent* event)
{
    if (m_mode == Mode::ROI)
    {
        m_origin = event->pos();
        if (!m_rubberBand)
            m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        m_rubberBand->setGeometry(QRect(m_origin, QSize()));
        m_rubberBand->show();
    }
}

void SelectionCanvas::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mode == Mode::ROI)
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }
}

void SelectionCanvas::mouseReleaseEvent(QMouseEvent* event)
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
