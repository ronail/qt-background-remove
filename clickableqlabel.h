#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QLabel>
#include <QQueue>



class ClickableQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableQLabel(QWidget *parent = 0);
    QImage* getImage();
    static int TOLERANCE;
    void setImage(QImage const image);
    void reset();
    static const void writeAlphaMask(const QImage* srcImage, QPoint *point, QImage *alphaMask);
protected:
    QPixmap *originPixmap;
    QImage image;
    QPoint *lastPoint;
    QRgb lastRbg;
    void mousePressEvent(QMouseEvent *ev);
    static const int enqueueUncheckedNeighbours(QQueue<long> *queue, QSet<long> *set, int x, int y, int width, int height);
signals:

private slots:

public slots:
    void removeBackground();
    void didChangeTolerant();
};

QImage alphaMaskWithPoint( const QImage& srcImage, const QPoint *point );
#endif // CLICKABLEQLABEL_H
