#include "FileExplorerModel.h"

#include <QCoreApplication>
#include <QDir>
#include <QStringList>
#include <QImage>
#include <QDateTime>
#include <set>

FileExplorerModel::FileExplorerModel()
{
    setReadOnly(true);
    setFilter(QDir::AllDirs | QDir::Files);
    setNameFilterDisables(false);
}

QVariant FileExplorerModel::data(const QModelIndex &index, const int role) const
{
    switch (role)
    {
    case FileName:
        return fileInfo(index).fileName();
    case FilePath:
        return QFileSystemModel::filePath(index);
    case FileSize:
        return fileInfo(index).size();
    case FileIsDir:
        return isDir(index);
    case FilesInDir:
    {
        const auto info = fileInfo(index);
        if (!info.isDir())
            return 0;

        auto dir = QDir{info.absoluteFilePath()};
        dir.setFilter(QDir::Files);
        dir.setNameFilters(nameFilters());
        return dir.count();
    }
    }
    return QVariant();
}

QModelIndex FileExplorerModel::getRootIndex() const
{
    return getPathIndex(getRootFolder());
}

QModelIndex FileExplorerModel::getPathIndex(const QString &path) const
{
    return index(path);
}

QString FileExplorerModel::getParentFolder() const
{
    auto dir = rootDirectory();
    dir.cdUp();
    return dir.absolutePath();
}

QString FileExplorerModel::getRootFolder() const
{
    auto res = rootPath();
    return rootPath();
}

void FileExplorerModel::setRootFolder(QString rootfolder)
{
    if (getRootFolder() == rootfolder)
        return;

    setRootPath(rootfolder);
    emit rootFolderChanged(rootfolder);
}

QHash<int, QByteArray> FileExplorerModel::roleNames() const
{
    return {{FileName, "fileName"}, {FilePath, "filePath"}, {FileSize, "fileSize"}, {FileIsDir, "fileIsDir"}, {FilesInDir, "filesInDir"}};
}