#include "clickableqlabel.h"
#include <QMouseEvent>
#include <QMessageBox>
#include "QFileDialog"
#include "QMessageBox"

static const int TOLERANCE = 20;

ClickableQLabel::ClickableQLabel(QWidget *parent) : QLabel(parent){

    setMouseTracking(true);
}

void ClickableQLabel::mousePressEvent(QMouseEvent *ev){
    ev->accept();
    ev->pos();
    if (this->pixmap()) {
        QImage image = this->pixmap()->toImage();
        QRgb rgb = image.pixel(ev->pos());
//        QColor color(rgb);
//        QMessageBox::information(this, "", tr("Rgb components: ") + QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()));
//        QImage mask = image.createMaskFromColor(rgb, Qt::MaskOutColor);
        QImage mask = createColorAlphaMask(image, rgb);
        image.setAlphaChannel(mask);
//        this->setPixmap(QPixmap::fromImage(image));
        this->setPixmap(QPixmap::fromImage(mask));
    }else {
        this->loadImageFromDialog();
    }
}

QImage ClickableQLabel::createColorAlphaMask( const QImage& srcImage, QRgb rgb )
{
//    int nWidth = m_PatchSetImage.width();
//    int nHeight = m_PatchSetImage.height();
//    int nDepth = m_PatchSetImage.depth();
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

            for ( int x = 0; x < nWidth; x ++ )
            {
//                pSrcData ++;
                if ( *pSrcData++ >> 24 )
                {
                    pMaskData[x/8] |= 1 << (x % 8);   // fill 1111 1111 to every char (8 bit)
//                    pMaskData[x/8] = 15;
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

void ClickableQLabel::loadImageFromDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load image file"), QDir::currentPath());
    if(!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"), tr("Fail to load %1").arg(fileName));
            return;
        }

        // load image into label
        this->setPixmap(QPixmap::fromImage(image));
        // resize image
        this->resize(image.size());
    }
}
