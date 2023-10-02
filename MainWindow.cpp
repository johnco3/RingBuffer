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
    auto gBytesWritten = 0;
    auto gBytesRead = 0;
    const std::vector<char> gFixedTestBuffer =
        ranges::views::closed_iota('A', 'Z')
        | ranges::views::cycle
        | ranges::views::take(1500)
        | ranges::to<std::vector<char>>();
    std::vector<char> readBuffer(gFixedTestBuffer.size());
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
    ui->bufferSize->setText(QString("%L1").arg(mpBuffer->size()));
    ui->readBlockSize->setValue(gReadBlockSize);
    ui->writeBlockSize->setValue(gWriteBlockSize);
    ui->bytesWritten->setText(QString("%L1").arg(gBytesWritten));
    ui->bytesRead->setText(QString("%L1").arg(gBytesRead));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == mpTimer->timerId()) {
        if (++gCounter % 3 != 0) {
            // write multiple blocks into buffer
            auto offset = 0;
            const auto blockLen = ui->writeBlockSize->value();
            auto nBlocksWritten = 0;
            while (offset < static_cast<int>(gFixedTestBuffer.size())) {
                const auto nRemaining = static_cast<int>(
                    gFixedTestBuffer.size() - offset);
                const auto nBytesToWrite = std::min(
                    nRemaining, blockLen);
                // this API does not fail
                mpBuffer->append(gFixedTestBuffer.data() + offset, nBytesToWrite);
                offset += nBytesToWrite;
                gBytesWritten += nBytesToWrite;
                nBlocksWritten++;
            }
            gWriteBlockCounter += nBlocksWritten;
            ui->writeBlockCounter->setText(QString(
                "%L1").arg(gWriteBlockCounter));
            ui->bytesWritten->setText(QString(
                "%L1").arg(gBytesWritten));
        } else { // read multiple blocks into readBuffer
            auto offset = 0;
            const auto blockLen = ui->readBlockSize->value();
            auto nBlocksRead = 0;
            while (offset < static_cast<int>(readBuffer.size())) {
                const auto nRemaining = static_cast<int>(
                    readBuffer.size() - offset);
                const auto nBytesToRead = std::min(
                    nRemaining, blockLen);
                const auto nBytesRead = mpBuffer->read(
                    readBuffer.data() + offset, nBytesToRead);
                if (nBytesRead > 0) {
                    offset += nBytesRead;
                    gBytesRead += nBytesRead;
                    nBlocksRead++;
                } else if (offset < static_cast<int>(readBuffer.size())) {
                    // this is an error
                    break;
                }
            }
            gReadBlockCounter += nBlocksRead;
            ui->readBlockCounter->setText(QString(
                "%L1").arg(gReadBlockCounter));
            ui->bytesRead->setText(QString(
                "%L1").arg(gBytesRead));
            if (offset == static_cast<int>(readBuffer.size())) {
                // compare buffers cyclic pattern data
                if (gFixedTestBuffer != readBuffer) {
                    ui->errorCounter->setText(QString(
                        "%L1").arg(++gErrorCounter));
                }
            }
            // reset the buffer
            std::ranges::fill(readBuffer, 0);
        }
        ui->bufferSize->setText(QString(
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
        // reset stats
        gErrorCounter = gWriteBlockCounter = gReadBlockCounter = gBytesWritten = gBytesRead = 0;
        resetFormFields();
        // disable button until protocol started
        ui->start->setText("Stop");
        mpTimer->start(1000, Qt::PreciseTimer, this);
    } else {
        if (mpTimer->isActive()) {
            mpTimer->stop();
        }
        ui->start->setText("Start");
    }
}
