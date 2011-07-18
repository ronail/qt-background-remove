#include "clickableqlabel.h"
#include <QMouseEvent>
#include "QFileDialog"
#include "iostream"
using namespace std;

static const int TOLERANCE = 20;


ClickableQLabel::ClickableQLabel(QWidget *parent) : QLabel(parent){
    setMouseTracking(true);
}

void ClickableQLabel::mousePressEvent(QMouseEvent *ev){
    ev->accept();
    if (this->pixmap()) {
        QImage scaledImage = this->pixmap()->toImage();
        int x = ev->pos().x() * this->image.size().width() / scaledImage.size().width();
        int y = ev->pos().y() * this->image.size().height() / scaledImage.size().height();
        QPoint *pos = new QPoint(x,y);
        QImage mask = createColorAlphaMask(this->image, pos);
        this->image.setAlphaChannel(mask);
        this->setImage(&(this->image));

    }
}

QImage ClickableQLabel::createColorAlphaMask( const QImage& srcImage, const QPoint *point )
{
//    int nWidth = m_PatchSetImage.width();
//    int nHeight = m_PatchSetImage.height();
//    int nDepth = m_PatchSetImage.depth();
    int nWidth = srcImage.width();
    int nHeight = srcImage.height();
    int nDepth = srcImage.depth();

    QRgb rgb = srcImage.pixel(*point);

    int matchRed = qRed(rgb);
    int matchBlue = qBlue(rgb);
    int matchGreen = qGreen(rgb);


    QImage alphaMask( nWidth, nHeight, QImage::Format_MonoLSB );
    alphaMask.setColor( 0, 0xffffffff );
    alphaMask.setColor( 1, 0xff000000 );

    // initialize everything to 0 first
    memset( alphaMask.bits(), 0, alphaMask.numBytes() );

    switch ( nDepth )
    {
    case 32:
        for ( int y = 0; y < nHeight; ++y )
        {
            unsigned char* pMaskData = alphaMask.scanLine( y );
            unsigned int* pSrcData = (unsigned int*)srcImage.scanLine( y );
            QRgb *srcRgb = (QRgb *)srcImage.scanLine(y);

            for ( int x = 0; x < nWidth; x ++ )
            {
                if ( abs(qRed(*srcRgb) - matchRed) <= TOLERANCE && abs(qGreen(*srcRgb) - matchGreen) < TOLERANCE && abs(qBlue(*srcRgb) - matchBlue) < TOLERANCE)
                {
                    // fill 1 to the corresponding char
                    pMaskData[x/8] |= 1 << (x % 8);
                }
                srcRgb++;
            }
        }
        break;
    default:
        // Set mask to all 1's if not 32 bit depth
        memset( alphaMask.bits(), 0xFF, alphaMask.numBytes() );
        break;
    }

    return alphaMask;
}


void ClickableQLabel::setImage(QImage *image)
{
    // store it for export use
    this->image = *image;

    // scale image to fit label
    QImage scaledImage = image->scaled(QSize(this->size().width(), this->size().height()), Qt::KeepAspectRatio);
    // load image into label
    this->setPixmap(QPixmap::fromImage(scaledImage));
    // resize image
//        this->resize(image.size());
}


QImage *ClickableQLabel::getImage()
{
    return &(this->image);
}
