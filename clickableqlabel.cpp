#include "clickableqlabel.h"
#include <QMouseEvent>
#include <QMessageBox>

static const int TOLERANCE = 20;

ClickableQLabel::ClickableQLabel(QWidget *parent){
    setMouseTracking(true);
}

void ClickableQLabel::mousePressEvent(QMouseEvent *ev){
    ev->accept();
    ev->pos();
    QImage image = this->pixmap()->toImage();
    QRgb rgb = image.pixel(ev->pos());
//    QColor color(rgb);
//    QMessageBox::information(this, "", tr("Rgb components: ") + QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()));
//    QImage mask = image.createMaskFromColor(rgb, Qt::MaskOutColor);
    QImage mask = createAlphaMask(image, rgb);
    image.setAlphaChannel(mask);
    this->setPixmap(QPixmap::fromImage(image));
}

QImage ClickableQLabel::createAlphaMask( const QImage& srcImage, QRgb rgb )
{
    // QImage one only creates alpha mask for alpha values greater than 128... need it to be all alpha greater than 0

    int nWidth = srcImage.width();
    int nHeight = srcImage.height();
    int nDepth = srcImage.depth();

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

            for ( int x = 0; x < nWidth; ++x )
            {
                if ( *pSrcData++ >> 24 )
//                if (abs(qRed(*srcRgb) - qRed(rgb)) < TOLERANCE && abs(qBlue(*srcRgb) - qBlue(rgb)) < TOLERANCE && abs(qGreen(*srcRgb) - qGreen(rgb)) < TOLERANCE)
                {
                    pMaskData[x/8] |= 1 << (x % 8);
//                    QColor black(Qt::black);
//                    pMaskData[x/8] = (unsigned char )black.rgb();
//                    *srcRgb++ >> 24;
                }
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
