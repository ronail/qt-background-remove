#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QLabel>

class ClickableQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableQLabel(QWidget *parent = 0);
    QImage* getImage();
    void setImage(QImage *image);
protected:
    QImage createColorAlphaMask( const QImage& srcImage, const QPoint *point );
    QImage image;
    void mousePressEvent(QMouseEvent *ev);
signals:

private slots:

public slots:
};

#endif // CLICKABLEQLABEL_H
