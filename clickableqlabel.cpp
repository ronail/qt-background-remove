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
        this->lastPoint = new QPoint(ev->pos());
        this->removeBackground();
    }
}

static const void writeAlphaMask(const QImage* srcImage, QPoint *point, QImage *alphaMask)
{
    int nWidth = srcImage->width();
    int nHeight = srcImage->height();
    QRgb rgb = srcImage->pixel(*point);
    int matchRed = qRed(rgb);
    int matchBlue = qBlue(rgb);
    int matchGreen = qGreen(rgb);
    static QQueue<long> *pointQueue = new QQueue<long>();
    QSet<long> *checkedSet = new QSet<long>();
    QRgb tRgb;
    int x, y;

    pointQueue->enqueue(point->x() + point->y() * nWidth);

    // handle neighbouring pixel in a sequence of top, right, bottom, left
    while (!pointQueue->isEmpty()) {
        long index = pointQueue->dequeue();
        qDebug("(%d, %d)", x, y);
        x = index % nWidth;
        y = index / nWidth;
        tRgb = srcImage->pixel(x, y);
        if ( abs(qRed(tRgb) - matchRed) <= ClickableQLabel::TOLERANCE && abs(qGreen(tRgb) - matchGreen) < ClickableQLabel::TOLERANCE && abs(qBlue(tRgb) - matchBlue) < ClickableQLabel::TOLERANCE) {
            alphaMask->setPixel(x, y, 1);
            if (y < nHeight - 1) {
                if (!checkedSet->contains(index))
                    pointQueue->enqueue(index);
            }
            if (x < nWidth - 1) {
                if (!checkedSet->contains(index))
                    pointQueue->enqueue(index);
            }
            if (y != 0) {
                if (!checkedSet->contains(index))
                    pointQueue->enqueue(index);
            }
            if (x != 0) {
                if (!checkedSet->contains(index))
                    pointQueue->enqueue(index);
            }
        }
        checkedSet->insert(index);
    }
    delete pointQueue;
    delete checkedSet;
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
    this->clear();
}
