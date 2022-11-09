#include "BMPCompress.h"
#include <CompressedBitmap.h>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <sstream>
#include <QImageReader>
#include <fstream>

class Worker : public QObject
{
    Q_OBJECT
public slots:
    void Compress();
    void ReadCompressedFile();

signals:
    void compressedFile(QString path);
    void decompressedFile(QString path);

public:
    explicit Worker(QString path) : m_file_path(path){};

private: 
    QString m_file_path;
};

BMPCompress::BMPCompress(QObject *parent) : QObject(parent) {}

void BMPCompress::CompressFile(const QString &path)
{
    QThread* workerThread = new QThread;

    Worker *worker = new Worker(path);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &Worker::Compress);

    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);

    connect(worker, &Worker::compressedFile, this, &BMPCompress::FileCompressed);
    
    connect(worker, &Worker::compressedFile, worker, &Worker::deleteLater);

    workerThread->start();
}

void BMPCompress::ReadCompressedFile(const QString &in)
{
    QThread* workerThread = new QThread;

    Worker *worker = new Worker(in);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &Worker::ReadCompressedFile);

    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);

    connect(worker, &Worker::decompressedFile, this, &BMPCompress::FileDeCompressed);
    
    connect(worker, &Worker::decompressedFile, worker, &Worker::deleteLater);

    workerThread->start();
}

void Worker::Compress()
{
    const auto extension = m_file_path.split('.').last();
    QImage img(m_file_path, extension.toStdString().data());
    img.convertTo(QImage::Format::Format_Grayscale8);

    BitMap bit_map{img.width(), img.height(), {}};
    for (int y = 0; y < img.height(); y++)
    {
        auto line = img.constScanLine(y);
        bit_map.data.insert(bit_map.data.end(), line, line + img.width());
    }

    QString save_file_path = m_file_path + ".cmp";
    std::ofstream f(save_file_path.toStdString());

    ImageCompression<BitEncoding> compressed(bit_map.data.data(), bit_map.width, bit_map.height);
    compressed.Encode();
    compressed.Save(f);

    emit compressedFile(save_file_path);
}

void Worker::ReadCompressedFile()
{
    QFile in(m_file_path);
    in.open(QIODevice::ReadOnly);

    auto content = in.readAll();

    std::stringstream ss;
    ss.write(content.data(), content.size());

    ImageCompression<BitEncoding> decode(ss);

    auto res = decode.Decode();

    QImage img(res.width, res.height, QImage::Format::Format_Grayscale8);

    auto ptr = res.data.data();
    for (int y = 0; y < res.height; y++)
    {
        memcpy(img.scanLine(y), ptr , res.width);
        ptr += img.width();
    }

    const auto file_name = m_file_path.split('.');

    QString save_file_path = file_name.at(0) + ".dcmp." + file_name.at(1);

    img.save(save_file_path, file_name.at(1).toStdString().data());

    emit decompressedFile(save_file_path);
}

#include "BMPCompress.moc"
