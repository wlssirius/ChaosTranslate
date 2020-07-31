#ifndef Q_COLOR_PICKER_HEADER
#define Q_COLOR_PICKER_HEADER

#include <QPushButton>
#include <QColor>

class QColorPicker : public QPushButton
{
    Q_OBJECT
public:
    QColorPicker(QWidget* parent);

    void setColor(const QColor& color);
    const QColor& getColor();

public slots:
    void updateColor();
    void changeColor();

private:
    QColor color;
};

#endif