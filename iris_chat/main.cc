#include "iris_chat_gui.h"
#include "client_connection_manager.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ClientConnectionManager connectionManager(ClientType::IRIS_CHAT);
    IrisChatGUI window(connectionManager);
    window.show();

    return app.exec();
}
