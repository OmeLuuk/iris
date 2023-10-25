#include "iris_chat.h"
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
    IrisChat irisChat(connectionManager);

    // Main window
    QWidget window;
    window.setWindowTitle("Iris Chat");
    window.resize(400, 300);

    QVBoxLayout layout;

    QTextEdit textArea;
    layout.addWidget(&textArea);
    textArea.setReadOnly(true);

    QLineEdit inputBox;
    layout.addWidget(&inputBox);

    QPushButton sendButton("Send");
    layout.addWidget(&sendButton);

    window.setLayout(&layout);
    window.show();

    // Signal-slot connection
    QObject::connect(&sendButton, &QPushButton::clicked, [&]()
                     {
        textArea.append(inputBox.text());
        std::string line = inputBox.text().toStdString();

        std::string topic = "TEST TOPIC";
        uint8_t topicSize = static_cast<uint8_t>(topic.size());

        std::vector<char> msg;
        msg.push_back(topicSize);
        msg.insert(msg.end(), topic.begin(), topic.end());
        msg.insert(msg.end(), line.begin(), line.end());

        irisChat.sendMessage(connectionManager.getFd(), MessageType::PUBLIC_MESSAGE, msg.data(), msg.size());
        inputBox.clear(); });

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]()
                     { irisChat.EventCycle(); });
    timer.start(10); // Call EventCycle every 10 milliseconds

    return app.exec();
}
