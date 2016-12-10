#ifndef CUSTOMJOYSTICK
#define CUSTOMJOYSTICK

#include <QWidget>

class CustomJoystick : public QWidget
{
    Q_OBJECT

    public:
        explicit CustomJoystick(QWidget *parent = 0);
    private:
        bool state;
        QPoint jPos;
        QPoint offset;
        QPoint center;
        int radius;
        int jRad;

        void fillEllipse(QPainter *, QPoint _center, int rad, QColor) const;
        double dist(QPoint,QPoint) const;

    protected:
        void paintEvent(QPaintEvent *event);
        void mouseMoveEvent(QMouseEvent * event);
        void mousePressEvent(QMouseEvent * event);
        void resizeEvent(QResizeEvent *event);
};

#endif // CUSTOMJOYSTICK

