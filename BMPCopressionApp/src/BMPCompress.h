#pragma once
#include <QObject>
#include <QImage>
#include <QByteArray>
#include <QThread>
#include <QFile>

class BMPCompress : public QObject
{
    Q_OBJECT
public:
    explicit BMPCompress(QObject *parent = Q_NULLPTR);

public slots:
    void CompressFile(const QString &path);
    void ReadCompressedFile(const QString &path);

signals:
    void FileCompressed(QString path);
    void FileDeCompressed(const QString &path);
};

