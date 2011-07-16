#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QLabel>

class ClickableQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableQLabel(QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *ev);
    QImage createColorAlphaMask( const QImage& srcImage, QRgb rgb );
signals:

private slots:

public slots:
    void loadImageFromDialog();
};

#endif // CLICKABLEQLABEL_H
