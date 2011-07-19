#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QLabel>



class ClickableQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableQLabel(QWidget *parent = 0);
    QImage* getImage();
    static int TOLERANCE;
    void setImage(QImage image);
    void reset();
protected:
    QPixmap originPixmap;
    QImage image;
    QPoint lastPoint;
    QRgb lastRbg;
    void mousePressEvent(QMouseEvent *ev);
signals:

private slots:

public slots:
    void removeBackground();
    void didChangeTolerant();
};

QImage alphaMaskWithPoint( const QImage& srcImage, const QPoint *point );
#endif // CLICKABLEQLABEL_H
