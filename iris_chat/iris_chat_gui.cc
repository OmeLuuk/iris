#include "iris_chat_gui.h"

IrisChatGUI::IrisChatGUI(ClientConnectionManager &connectionManager,
                         const std::string &username) : irisChat(
                                                            connectionManager,
                                                            [this](const std::string &s1, const std::string &s2, const std::string &s3)
                                                            { displayMessage(s1, s2, s3); },
                                                            username),
                                                        username(username)
{
    // Setting up GUI
    this->setWindowTitle("Iris Chat");
    this->resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Initialize the tab widget and add a General tab
    tabWidget = new QTabWidget(this);
    layout->addWidget(tabWidget);
    addChatTab("General"); // Example tab

    inputBox = new QLineEdit(this);
    layout->addWidget(inputBox);

    QPushButton *sendButton = new QPushButton("Send", this);
    layout->addWidget(sendButton);

    // Connect the Send button to the send logic
    connect(sendButton, &QPushButton::clicked, this, &IrisChatGUI::sendMessage);

    // Set up the event cycle
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &IrisChatGUI::eventCycle);
    timer->start(10); // Call EventCycle every 10 milliseconds
}

QTextEdit *IrisChatGUI::addChatTab(const QString &name)
{
    QTextEdit *chatArea = new QTextEdit(this);
    chatArea->setReadOnly(true);
    tabWidget->addTab(chatArea, name);
    return chatArea;
}

void IrisChatGUI::removeChatTab(const QString &name)
{
    for (int i = 0; i < tabWidget->count(); i++)
    {
        if (tabWidget->tabText(i) == name)
        {
            QWidget *widget = tabWidget->widget(i);
            tabWidget->removeTab(i);
            delete widget;
            break;
        }
    }
}

void IrisChatGUI::sendMessage()
{
    QString line = inputBox->text();
    irisChat.SendChatMessage(line.toStdString());
    inputBox->clear();
}

void IrisChatGUI::eventCycle()
{
    irisChat.EventCycle();
}

void IrisChatGUI::displayMessage(const std::string& topic, const std::string& sender, const std::string &msg)
{
    // For now, display the message in the currently active tab
    QTextEdit *currentTextArea = static_cast<QTextEdit *>(tabWidget->currentWidget());
    if (currentTextArea)
    {
        currentTextArea->append(QString::fromStdString(msg));
    }
}