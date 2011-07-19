#include "clickableqlabel.h"
#include <QMouseEvent>
#include <QQueue>
#include <mainwindow.h>
#include <QTime>

using namespace std;

int ClickableQLabel::TOLERANCE = 30;

ClickableQLabel::ClickableQLabel(QWidget *parent) : QLabel(parent){
    setMouseTracking(true);
}

void ClickableQLabel::mousePressEvent(QMouseEvent *ev){
    ev->accept();
    if (this->pixmap()) {
        this->lastPoint = ev->pos();
        this->removeBackground();
    }
}

static const void writeAlphaMask(const QImage srcImage, QPoint *point, QImage *alphaMask)
{
    int nWidth = srcImage.width();
    int nHeight = srcImage.height();
    QRgb rgb = srcImage.pixel(*point);
    int matchRed = qRed(rgb);
    int matchBlue = qBlue(rgb);
    int matchGreen = qGreen(rgb);
    QQueue<QPoint> pointQueue;
    QSet<long> checkedSet;
    QPoint p;
    QPoint tp;
    QRgb tRgb;

    pointQueue.enqueue(QPoint(point->x(), point->y()));

    // handle neighbouring pixel in a sequence of top, right, bottom, left
    while (!pointQueue.isEmpty()) {
        p = pointQueue.dequeue();
        long setValue = p.x() + p.y() * nWidth;
        tRgb = srcImage.pixel(p);
        if ( abs(qRed(tRgb) - matchRed) <= ClickableQLabel::TOLERANCE && abs(qGreen(tRgb) - matchGreen) < ClickableQLabel::TOLERANCE && abs(qBlue(tRgb) - matchBlue) < ClickableQLabel::TOLERANCE) {
            alphaMask->setPixel(p, 1);
            if (p.y() < nHeight - 1) {
                tp = QPoint(p.x(), p.y() + 1);
                if (!checkedSet.contains(setValue))
                    pointQueue.enqueue(tp);
            }
            if (p.x() < nWidth - 1) {
                tp = QPoint(p.x() + 1, p.y());
                if (!checkedSet.contains(setValue))
                    pointQueue.enqueue(tp);
            }
            if (p.y() != 0) {
                tp = QPoint(p.x(), p.y() - 1);
                if (!checkedSet.contains(setValue))
                    pointQueue.enqueue(tp);
            }
            if (p.x() != 0) {
                tp = QPoint(p.x() - 1, p.y());
                if (!checkedSet.contains(setValue))
                    pointQueue.enqueue(tp);
            }
        }
        checkedSet.insert(setValue);
    }
}

QImage alphaMaskWithPoint( const QImage& srcImage, QPoint *point )
{
//    int nWidth = m_PatchSetImage.width();
//    int nHeight = m_PatchSetImage.height();
//    int nDepth = m_PatchSetImage.depth();
    int nWidth = srcImage.width();
    int nHeight = srcImage.height();
    int nDepth = srcImage.depth();

    QImage alphaMask( nWidth, nHeight, QImage::Format_MonoLSB );
    alphaMask.setColor( 0, 0xFFFFFFFF );
    alphaMask.setColor( 1, 0 );
    alphaMask.fill(0);
    // initialize everything to 0 first
//    memset( alphaMask.bits(), 0, alphaMask.numBytes() );

    QTime start;
    switch ( nDepth )
    {
    case 32:
        start.start();
        writeAlphaMask(srcImage, point, &alphaMask);
        qDebug("Time elapsed: %d ms", start.elapsed());
        break;
    default:
        // Set mask to all 1's if not 32 bit depth
        memset( alphaMask.bits(), 0xFF, alphaMask.numBytes() );
        break;
    }

    return alphaMask;
}


void ClickableQLabel::setImage(QImage newImage)
{
    // store it for export use
    this->image = newImage;

    // store a original copy
    QImage tempImage(newImage.size(), newImage.format());
    memcpy(tempImage.bits(), newImage.bits(), tempImage.numBytes());
    this->originPixmap = QPixmap::fromImage(tempImage);

    // scale image to fit label
    QImage scaledImage = newImage.scaled(QSize(this->size().width(), this->size().height()), Qt::KeepAspectRatio);
    // load image into label
    this->setPixmap(QPixmap::fromImage(scaledImage));
    // resize image
//        this->resize(image.size());
}

QImage *ClickableQLabel::getImage()
{
    return &(this->image);
}

void ClickableQLabel::removeBackground()
{
    if (lastPoint.x() < 0 || lastPoint.y() < 0)
        return;

    QImage scaledImage = this->pixmap()->toImage();
    int x = this->lastPoint.x() * this->image.size().width() / scaledImage.size().width();
    int y = this->lastPoint.y() * this->image.size().height() / scaledImage.size().height();

    QImage mask = alphaMaskWithPoint(this->image, new QPoint(x,y));
    this->image.setAlphaChannel(mask);
    this->setImage(this->image);
}

void ClickableQLabel::didChangeTolerant()
{

}

void ClickableQLabel::reset()
{
    this->lastPoint = QPoint(-1, -1);
}
