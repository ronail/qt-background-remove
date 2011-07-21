#include "clickableqlabel.h"
#include <QMouseEvent>
#include <mainwindow.h>
#include <QTime>
#include <QDebug>

using namespace std;

int ClickableQLabel::TOLERANCE = 30;
static const int enqueueUncheckedNeighbours(QQueue<QPoint *> *queue, QSet<long> *set, int x, int y, int width, int height, long index);
static QQueue<long> pointQueue;
static QSet<long> queuedSet;

ClickableQLabel::ClickableQLabel(QWidget *parent) : QLabel(parent){
    setMouseTracking(true);

    // init variables
    this->lastPoint = 0;
    this->lastRbg = 0;
}

void ClickableQLabel::mousePressEvent(QMouseEvent *ev){
    ev->accept();
    if (this->pixmap()) {
        this->lastPoint = new QPoint(ev->pos());
        this->removeBackground();
    }
}

static const void enqueueUncheckedNeighbours(QQueue<long> *queue, QSet<long> *set, int x, int y, int width, int height)
{
    long index = y * width + x;
    long nextIndex = 0;
    if (y < height - 1) {
        nextIndex = index + width;
        if (!set->contains(nextIndex)){
            queue->enqueue(nextIndex);
            set->insert(nextIndex);
        }
    }
    if (x < width - 1) {
        nextIndex = index + 1;
        if (!set->contains(nextIndex)){
            queue->enqueue(nextIndex);
            set->insert(nextIndex);
        }
    }
    if (y != 0) {
        nextIndex = index - width;
        if (!set->contains(nextIndex)){
            queue->enqueue(nextIndex);
            set->insert(nextIndex);
        }
    }
    if (x != 0) {
        nextIndex = index - 1;
        if (!set->contains(nextIndex)){
            queue->enqueue(nextIndex);
            set->insert(nextIndex);
        }
    }
}

static const bool isSimilarPixel(QRgb targetRgb, QRgb refRgb)
{
    int matchRed = qRed(refRgb);
    int matchBlue = qBlue(refRgb);
    int matchGreen = qGreen(refRgb);
    return abs(qRed(targetRgb) - matchRed) <= ClickableQLabel::TOLERANCE && abs(qGreen(targetRgb) - matchGreen) < ClickableQLabel::TOLERANCE && abs(qBlue(targetRgb) - matchBlue) < ClickableQLabel::TOLERANCE;
}

static const void writeAlphaChannel(QImage *srcImage, const QPoint *point)
{
    int nWidth = srcImage->width();
    int nHeight = srcImage->height();
    const QRgb rgb = srcImage->pixel(*point);
    int x, y;

    pointQueue.enqueue(point->x() + point->y() * nWidth);

    // handle neighbouring pixel in a sequence of top, right, bottom, left
    while (!pointQueue.isEmpty()) {
        long index = pointQueue.dequeue();
        x = index % nWidth;
        y = index / nWidth;
//        qDebug("(%d, %d), Queue length: %d", x, y, pointQueue->length());
        QRgb srcRgb = srcImage->pixel(x, y);
        if (isSimilarPixel(srcRgb, rgb) ) {
            srcImage->setPixel(x, y, 0x00ffffff & srcRgb);
            enqueueUncheckedNeighbours(&pointQueue, &queuedSet, x, y, nWidth, nHeight);
        }
    }
    pointQueue.clear();
    queuedSet.clear();
}


void ClickableQLabel::setImage(QImage const newImage)
{
    // store it for export use
    this->image = QImage(newImage);

    // add alpha channel if not exist
    if(!this->image.hasAlphaChannel())
    {
//        memset( alphaMask->bits(), 0xFF, alphaMask->numBytes() );
        QImage alphaMask(this->image.size(), QImage::Format_MonoLSB);
        alphaMask.fill(0xFFFFFFFF);
        this->image.setAlphaChannel(alphaMask);
    }

    // store a original copy
    this->originPixmap = &QPixmap::fromImage(this->image);

    // scale image to fit label
    QImage scaledImage = this->image.scaled(QSize(this->size().width(), this->size().height()), Qt::KeepAspectRatio);
    // load image into label
//    if (this->pixmap())
//        delete this->pixmap();
    this->setPixmap(QPixmap::fromImage(scaledImage));
    // resize image
//        this->resize(image.size());
}

QImage *ClickableQLabel::getImage()
{
    return &this->image;
}

void ClickableQLabel::removeBackground()
{
    if (this->lastPoint == 0)
        return;

    QImage scaledImage = this->pixmap()->toImage();
    int x = this->lastPoint->x() * this->image.size().width() / scaledImage.size().width();
    int y = this->lastPoint->y() * this->image.size().height() / scaledImage.size().height();

//    QImage *mask = alphaMaskWithPoint(this->image, new QPoint(x,y));
//    this->image->setAlphaChannel(*mask);
    QPoint point(x, y);
    QTime start;
    start.start();
    writeAlphaChannel(&this->image, &point);
    qDebug("Time elapsed: %d ms", start.elapsed());
    this->setImage(this->image);
}

void ClickableQLabel::didChangeTolerant()
{

}

void ClickableQLabel::reset()
{
//    this->lastPoint->setX(-1);
//    this->lastPoint->setY(-1);
//    delete this->image;
    this->lastPoint = 0;
    this->originPixmap = 0;
//    this->clear();
}
