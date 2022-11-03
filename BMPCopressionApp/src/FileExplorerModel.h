#pragma once
#include <QFileSystemModel>
#include <QQuickItem>
#include "BMPCompress.h"

class FileExplorerModel : public QFileSystemModel
{
    Q_OBJECT
    Q_PROPERTY(QString rootFolder READ getRootFolder WRITE setRootFolder NOTIFY
                   rootFolderChanged)
    Q_PROPERTY(bool hideExtensions READ getHideExtensions WRITE
                   setHideExtensions NOTIFY hideExtensionsChanged)
public:
    enum FolderModelRoles
    {
        FileName = Qt::UserRole + 5,
        FilePath,
        FileSize,
        FileIsDir,
        FilesInDir
    };
    Q_ENUMS(FolderModelRoles)

    FileExplorerModel();

    ~FileExplorerModel() = default;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, const int role) const override;

    Q_INVOKABLE QModelIndex getRootIndex() const;
    Q_INVOKABLE QModelIndex getPathIndex(const QString &path) const;
    Q_INVOKABLE QString getParentFolder() const;

    QString getRootFolder() const;
    bool getHideExtensions() const;

public slots:
    void setRootFolder(QString rootfolder);
    void setHideExtensions(bool hideExtensions);

signals:
    void rootFolderChanged(QString rootfolder);
    void hideExtensionsChanged(bool hideExtensions);

private:
    void saveCompressedFile(QByteArray &res) const;
    bool _hideExtensions = false;
};
