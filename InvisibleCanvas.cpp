#define NOMINMAX
#include "InvisibleCanvas.h"
#include "QGraphicsOpacityEffect"
#include "QLayout"
#include "QImageReader"

InvisibleCanvas::InvisibleCanvas(RECT& rect):
	QDialog(nullptr, 0),
    m_roiRect(rect)
{
    //setGeometry(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    //setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    m_imageLabel = new QLabel;
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_imageLabel);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    //loadImage();
    //m_imageLabel = new QLabel;
    //QVBoxLayout* layout = new QVBoxLayout(this);
    //layout->addWidget(m_imageLabel);
    //layout->setContentsMargins(QMargins(0, 0, 0, 0));
}

void InvisibleCanvas::showCanvas(RECT rect)
{
    setGeometry(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    loadImage();
}

void InvisibleCanvas::mousePressEvent(QMouseEvent* event)
{
    m_origin = event->pos();
    if (!m_rubberBand)
        m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    m_rubberBand->setGeometry(QRect(m_origin, QSize()));
    m_rubberBand->show();
}

void InvisibleCanvas::mouseMoveEvent(QMouseEvent* event)
{
    m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
}

void InvisibleCanvas::mouseReleaseEvent(QMouseEvent* event)
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
    this->destroy();
}

void InvisibleCanvas::loadImage()
{
    QImageReader reader("D:/TestFile.bmp");
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    m_imageLabel->setPixmap(QPixmap::fromImage(newImage));
}
