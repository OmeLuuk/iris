#include "iris_chat_gui.h"

#include <QListWidget>
#include <QSplitter>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace
{
    std::string getCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::tm *local_tm = std::localtime(&now_time);

        std::ostringstream timeStream;
        timeStream << std::put_time(local_tm, "%H:%M");

        return timeStream.str();
    }
}

IrisChatGUI::IrisChatGUI(ClientConnectionManager &connectionManager, const std::string &username)
    : irisChat(
          connectionManager, [this](const PublicMessage &message)
          { displayMessage(message); },
          username,
          [this](const UserUpdate &userUpdate)
          { changeUserList(userUpdate); }),
      username(username)
{
    // Setting up GUI
    this->setWindowTitle("Iris Chat");
    this->resize(600, 400); // Adjusted for additional list space

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Chat area setup
    QVBoxLayout *chatLayout = new QVBoxLayout();
    tabWidget = new QTabWidget();
    chatLayout->addWidget(tabWidget);
    addChatTab("General");

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputBox = new QLineEdit();
    inputLayout->addWidget(inputBox);
    QPushButton *sendButton = new QPushButton("Send");
    inputLayout->addWidget(sendButton);
    chatLayout->addLayout(inputLayout); // Add the input section below the chat area

    mainLayout->addLayout(chatLayout, 3); // Add the chat layout to the main layout with width 3

    listWidget = new QListWidget();
    mainLayout->addWidget(listWidget, 1); // Add user list to the right of chat area with width 1

    // Connect the Send button to the send
    connect(sendButton, &QPushButton::clicked, this, &IrisChatGUI::sendMessage);

    // Connect the user list item click event to a handler
    connect(listWidget, &QListWidget::itemClicked, this, &IrisChatGUI::onUserClicked);

    // Set the main layout for the widget
    this->setLayout(mainLayout);

    // Set up the event cycle
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &IrisChatGUI::eventCycle);
    timer->start(10); // Call EventCycle every 10 milliseconds

    QListWidgetItem *item = new QListWidgetItem("Other");
    listWidget->addItem(item);
}

QTextEdit *IrisChatGUI::addChatTab(const QString &name)
{
    QTextEdit *chatArea = new QTextEdit(this);
    chatArea->setReadOnly(true);
    tabWidget->addTab(chatArea, name);
    return chatArea;
}

void IrisChatGUI::onUserClicked(QListWidgetItem *item)
{
    QString userName = item->text();

    for (int i = 0; i < tabWidget->count(); ++i)
        if (tabWidget->tabText(i) == userName)
            return;
    
    addChatTab(userName);
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
    QString currentTabName = tabWidget->tabText(tabWidget->currentIndex());

    irisChat.SendChatMessage(currentTabName.toStdString(), line.toStdString());
    inputBox->clear();

    QTextEdit *currentTextArea = static_cast<QTextEdit *>(tabWidget->currentWidget());
    if (currentTextArea)
        currentTextArea->append(QString::fromStdString(getCurrentTime()) + " " + QString::fromStdString(username) + ": " + line);
}

void IrisChatGUI::eventCycle()
{
    irisChat.EventCycle();
}

void IrisChatGUI::displayMessage(const PublicMessage &message)
{
    if (message.sender == username)
        return;

    const QString tabName = message.topic == username ? QString::fromStdString(message.sender) : QString::fromStdString(message.topic);

    QTextEdit *textAreaForTab = nullptr;

    // Loop through all tabs to find the appropriate tab
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        if (tabWidget->tabText(i) == tabName)
        {
            textAreaForTab = qobject_cast<QTextEdit *>(tabWidget->widget(i));
            break;
        }
    }

    // Check if the tab was found and create it otherwise
    if (!textAreaForTab)
    {
        textAreaForTab = addChatTab(tabName);
    }

    // Append the message to the found tab's text area
    textAreaForTab->append(QString::fromStdString(getCurrentTime() + " " + message.sender + ": " + message.content));
}

void IrisChatGUI::changeUserList(const UserUpdate &userUpdate) // userStatus field not used?
{
    if (userUpdate.username == this->username)
        return;

    for (int i = 0; i < listWidget->count(); ++i)
        if (listWidget->item(i)->text() == QString::fromStdString(userUpdate.username))
            return;

    QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(userUpdate.username));
    listWidget->addItem(item);
}
