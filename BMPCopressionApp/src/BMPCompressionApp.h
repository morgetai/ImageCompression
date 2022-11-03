#pragma once
#include <string>

#include <QGuiApplication>

class BMPCompressionApp
{
public:
    BMPCompressionApp(int argc, char *argv[]);
    int Run();

private:

    QGuiApplication m_app;
};