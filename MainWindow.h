#pragma once

// SYSTEM INCLUDES
#include <QMainWindow>

// APPLICATION INCLUDES
// DEFINES
// MACROS
// EXTERNAL FUNCTIONS
// EXTERNAL VARIABLES
// CONSTANTS
// STRUCTS
// TYPEDEFS
// FORWARD DECLARATIONS

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QBasicTimer;
class QRingBuffer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_start_clicked();

private:
    void timerEvent(QTimerEvent* event) override;
    void resetFormFields() const;
    Ui::MainWindow *ui;
    std::unique_ptr<QBasicTimer> mpTimer;
    std::unique_ptr<QRingBuffer> mpBuffer;
};
