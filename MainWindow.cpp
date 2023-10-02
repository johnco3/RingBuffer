// SYSTEM INCLUDES
#include <QBasicTimer>
#include <range/v3/all.hpp>
#include <private/qringbuffer_p.h>

// APPLICATION INCLUDES
#include "MainWindow.h"
#include "ui_MainWindow.h"

// EXTERNAL FUNCTIONS
// EXTERNAL VARIABLES
// CONSTANTS
// DEFINES
// NAMESPACE USAGE
// STATIC VARIABLE INITIALIZATIONS

namespace {
    auto gCounter = 0;
    auto gErrorCounter = 0;
    auto gReadBlockCounter = 0;
    auto gWriteBlockCounter = 0;
    auto gWriteBlockSize = 512;
    auto gReadBlockSize = 512;
    const std::vector<char> gTestData =
        ranges::views::closed_iota('A', 'Z')
        | ranges::views::cycle
        | ranges::views::take(1500)
        | ranges::to<std::vector<char>>();
    std::vector<char> readBuffer(gTestData.size());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mpTimer{ std::make_unique<QBasicTimer>() }
    , mpBuffer{ std::make_unique<QRingBuffer>() }
{
    ui->setupUi(this);
    gErrorCounter = gWriteBlockCounter = gReadBlockCounter = 0;
    gWriteBlockSize = gReadBlockSize = 512;
    resetFormFields();
}

void
MainWindow::resetFormFields() const
{
    ui->errorCounter->setText(QString::number(gErrorCounter));
    ui->writeBlockCounter->setText(QString::number(gWriteBlockCounter));
    ui->readBlockCounter->setText(QString::number(gReadBlockCounter));
    ui->ringBufferSize->setText(QString("%L1").arg(mpBuffer->size()));
    ui->readBlockSize->setValue(gReadBlockSize);
    ui->writeBlockSize->setValue(gWriteBlockSize);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == mpTimer->timerId()) {
        if (gCounter++ % 3 == 0) {
            mpBuffer->append(gTestData.data(), gTestData.size());
            ui->writeBlockCounter->setText(QString(
                "%L1").arg(++gWriteBlockCounter));
        } else {
            auto nBytesRemaining = readBuffer.size();
            auto offset = 0;
            while (nBytesRemaining > 0) {
                const auto nBytesRead = mpBuffer->read(readBuffer.data() + offset,
                    static_cast<qint64>(readBuffer.size()/2));
                if (nBytesRead > 0) {
                    offset += nBytesRead;
                    nBytesRemaining -= nBytesRead;
                    ui->readBlockCounter->setText(QString(
                        "%L1").arg(++gReadBlockCounter));
                } else if (nBytesRemaining > 0) {
                    ui->errorCounter->setText(QString(
                        "%L1").arg(++gErrorCounter));
                    break;
                }
            }
            if (nBytesRemaining == 0) {
                // compare the read buffer against the cyclic pattern data
                if (gTestData != readBuffer) {
                    ui->errorCounter->setText(QString(
                        "%L1").arg(++gErrorCounter));
                }
            }
            // reset the buffer
            std::ranges::fill(readBuffer, 0);
        }
        ui->ringBufferSize->setText(QString(
            "%L1").arg(mpBuffer->size()));
    } else { // pass unhandled timer event up the chain
        QObject::timerEvent(event);
    }
}

void
MainWindow::on_start_clicked()
{
    // if already started stop the worker threads
    if (ui->start->text() == "Start") {
        // disable button until protocol started
        ui->start->setText("Stop");
        mpTimer->start(100, Qt::PreciseTimer, this);
    } else {
        if (mpTimer->isActive()) {
            mpTimer->stop();
        }
        gErrorCounter = gWriteBlockCounter = gReadBlockCounter = 0;
        resetFormFields();
        ui->start->setText("Start");
    }
}
