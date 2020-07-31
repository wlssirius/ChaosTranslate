#include "QColorPicker.h"
#include <QColorDialog>

QColorPicker::QColorPicker(QWidget* parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(changeColor()));
}

void QColorPicker::updateColor()
{
    setStyleSheet("background-color: " + color.name());
}

void QColorPicker::changeColor()
{
    QColor newColor = QColorDialog::getColor(color, parentWidget());
    if (newColor != color)
    {
        setColor(newColor);
    }
}

void QColorPicker::setColor(const QColor& color)
{
    this->color = color;
    updateColor();
}

const QColor& QColorPicker::getColor()
{
    return color;
}