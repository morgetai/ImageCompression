#include "BMPCompressionApp.h"
#include "FileExplorerModel.h"
#include "BMPCompress.h"

#include <CompressedBitmap.h>
#include <vector>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

BMPCompressionApp::BMPCompressionApp(int argc, char *argv[]) : m_app(argc, argv)
{
}

int BMPCompressionApp::Run()
{
    QQmlApplicationEngine engine;

    QQmlContext* rootContext = engine.rootContext();
    
    QFileSystemModel *fsm = new QFileSystemModel(&engine);

    qmlRegisterType<FileExplorerModel>("MyPlugin", 1, 0, "FolderModel");
    qmlRegisterType<BMPCompress>("MyPlugin", 1, 0, "BMPCompress");

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return m_app.exec();
}