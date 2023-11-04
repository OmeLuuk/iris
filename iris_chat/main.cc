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
    if (argc != 2)
    {
        log(LL::ERROR, "Usage: " + std::string(argv[0]) + " <username>");
        return 1;
    }

    std::string username = argv[1];

    QApplication app(argc, argv);

    ClientConnectionManager connectionManager(ClientType::IRIS_CHAT);
    IrisChatGUI window(connectionManager, username);
    window.show();

    return app.exec();
}
