#include "iris_chat.h"
#include "client_connection_manager.h"
#include "logging.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QTabWidget>

class IrisChatGUI : public QWidget
{
    Q_OBJECT
public:
    IrisChatGUI(ClientConnectionManager &connectionManager);
    QTextEdit *addChatTab(const QString &name);
    void removeChatTab(const QString &name);

public slots:
    void sendMessage();
    void eventCycle();
    void displayMessage(const std::string &msg);

private:
    IrisChat irisChat;
    QTextEdit *textArea;
    QLineEdit *inputBox;
    QTabWidget *tabWidget;
};