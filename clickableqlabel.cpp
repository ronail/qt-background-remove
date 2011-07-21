#include "clickableqlabel.h"
#include <QMouseEvent>
#include <mainwindow.h>
#include <QTime>

using namespace std;

int ClickableQLabel::TOLERANCE = 30;
static const int enqueueUncheckedNeighbours(QQueue<QPoint *> *queue, QSet<long> *set, int x, int y, int width, int height, long index);

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

static const int enqueueUncheckedNeighbours(QQueue<long> *queue, QSet<long> *set, int x, int y, int width, int height)
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

static const void writeAlphaMask(const QImage* srcImage, QPoint *point, QImage *alphaMask)
{
    int nWidth = srcImage->width();
    int nHeight = srcImage->height();
    const QRgb rgb = srcImage->pixel(*point);
    static QQueue<long> pointQueue;
    static QSet<long> queuedSet;
    int x, y;

    pointQueue.enqueue(point->x() + point->y() * nWidth);

    // handle neighbouring pixel in a sequence of top, right, bottom, left
    while (!pointQueue.isEmpty()) {
        long index = pointQueue.dequeue();
        x = index % nWidth;
        y = index / nWidth;
//        qDebug("(%d, %d), Queue length: %d", x, y, pointQueue->length());
        if (isSimilarPixel(srcImage->pixel(x, y), rgb) ) {
            alphaMask->setPixel(x, y, 1);
            enqueueUncheckedNeighbours(&pointQueue, &queuedSet, x, y, nWidth, nHeight);
        }
//        foreach(long i, *checkedSet)
//            qDebug("%d", i);
    }
    pointQueue.clear();
    queuedSet.clear();
//    delete pointQueue;
//    delete queuedSet;
}

QImage *alphaMaskWithPoint( const QImage* srcImage, QPoint *point )
{
//    int nWidth = m_PatchSetImage.width();
//    int nHeight = m_PatchSetImage.height();
//    int nDepth = m_PatchSetImage.depth();
    int nWidth = srcImage->width();
    int nHeight = srcImage->height();
    int nDepth = srcImage->depth();

    QImage *alphaMask = new QImage( nWidth, nHeight, QImage::Format_MonoLSB );
    alphaMask->setColor( 0, 0xFFFFFFFF );
    alphaMask->setColor( 1, 0 );
    alphaMask->fill(0);
    // initialize everything to 0 first
//    memset( alphaMask.bits(), 0, alphaMask.numBytes() );

    QTime start;
    switch ( nDepth )
    {
    case 32:
        start.start();
        writeAlphaMask(srcImage, point, alphaMask);
        qDebug("Time elapsed: %d ms", start.elapsed());
        break;
    default:
        // Set mask to all 1's if not 32 bit depth
        memset( alphaMask->bits(), 0xFF, alphaMask->numBytes() );
        break;
    }

    return alphaMask;
}


void ClickableQLabel::setImage(QImage* newImage)
{
    // store it for export use
    this->image = newImage;

    // store a original copy
    this->originPixmap = new QPixmap(QPixmap::fromImage(*newImage));

    // scale image to fit label
    QImage scaledImage = newImage->scaled(QSize(this->size().width(), this->size().height()), Qt::KeepAspectRatio);
    // load image into label
    if (!this->pixmap())
        delete this->pixmap();
    this->setPixmap(QPixmap::fromImage(scaledImage));
    // resize image
//        this->resize(image.size());
}

QImage *ClickableQLabel::getImage()
{
    return this->image;
}

void ClickableQLabel::removeBackground()
{
    if (this->lastPoint == 0)
        return;

    QImage scaledImage = this->pixmap()->toImage();
    int x = this->lastPoint->x() * this->image->size().width() / scaledImage.size().width();
    int y = this->lastPoint->y() * this->image->size().height() / scaledImage.size().height();

    QImage *mask = alphaMaskWithPoint(this->image, new QPoint(x,y));
    this->image->setAlphaChannel(*mask);
    this->setImage(this->image);
}

void ClickableQLabel::didChangeTolerant()
{

}

void ClickableQLabel::reset()
{
//    this->lastPoint->setX(-1);
//    this->lastPoint->setY(-1);
    delete this->image;
    delete this->lastPoint;
    delete this->originPixmap;
//    this->clear();
}
