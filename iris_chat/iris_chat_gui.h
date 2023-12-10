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
#include <QListWidgetItem>

class IrisChatGUI : public QWidget
{
    Q_OBJECT
public:
    IrisChatGUI(ClientConnectionManager &connectionManager, const std::string &username);
    QTextEdit *addChatTab(const QString &name);
    void onUserClicked(QListWidgetItem *item);
    void removeChatTab(const QString &name);

public slots:
    void sendMessage();
    void eventCycle();
    void displayMessage(const PublicMessage &message);
    void changeUserList(const UserUpdate &update);

private:
    IrisChat irisChat;
    QTextEdit *textArea;
    QLineEdit *inputBox;
    QTabWidget *tabWidget;
    QListWidget *listWidget;

    const std::string &username;
};