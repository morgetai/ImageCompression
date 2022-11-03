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

    //connect(worker, &Worker::compressedFile, this, [](){
       // emit this->CompressFile("");
    //}, Qt::ConnectionType::QueuedConnection);

    connect(worker, &Worker::compressedFile, this, &BMPCompress::FileCompressed);

    /*connect(worker, &Worker::compressedFile, this, [&worker, this](QString result)
            {
                worker->deleteLater();
                emit this->FileCompressed(result); });*/

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

    workerThread->start();
}

void Worker::Compress()
{
    QImage tmp(m_file_path, "BMP");
    auto img = tmp.convertedTo(QImage::Format::Format_Mono);

    BitMap bit_map{img.width(), img.height(), {}};
    int rawDataCount = 0;
    for (int y = 0; y < img.height(); y++)
    {
        auto line = img.constScanLine(y);
        bit_map.data.insert(bit_map.data.end(), line, line + img.width());
    }

    std::stringstream ss;
    CompressedBitMap compressed(bit_map.data.data(), bit_map.width, bit_map.height);
    compressed.Save(ss);

    QByteArray b_array(reinterpret_cast<const char*> (ss.str().c_str()), ss.str().size());

    QString save_file_path = m_file_path + ".cmp";
    QFile file(save_file_path);
    file.open(QIODevice::WriteOnly);
    file.write(b_array);
    file.close();

    emit compressedFile(save_file_path);
}

void Worker::ReadCompressedFile()
{
    QFile in(m_file_path);
    in.open(QIODevice::ReadOnly);

    auto content = in.readAll();

    std::stringstream ss;
    ss.write(content.data(), content.size());
    //auto size = content.size();
    //ss << '\0';

    //std::ifstream f(m_file_path.toStdString());

    auto res = CompressedBitMap::ReadCompressedFile(ss);

  
    QImage img(res.data.data(), res.width, res.height, QImage::Format::Format_Mono);

    QString save_file_path = m_file_path + ".dcmp";

      QFile out(save_file_path);
      out.open(QIODevice::WriteOnly);
    out.write(reinterpret_cast<const char *>(res.data.data()), res.height * res.width);
    out.close();


    //img.save(save_file_path);

    emit decompressedFile(save_file_path);
}

#include "BMPCompress.moc"
