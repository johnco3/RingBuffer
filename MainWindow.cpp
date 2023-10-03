// SYSTEM INCLUDES
#include <QBasicTimer>
#include <QStyleFactory>
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
    qint64 gCounter = 0;
    qint64 gUnderruns = 0;
    qint64 gErrorCounter = 0;
    qint64 gReadBlockCounter = 0;
    qint64 gWriteBlockCounter = 0;
    qint64 gBytesWritten = 0;
    qint64 gBytesRead = 0;
    qint64 gBuffersWritten = 0;
    qint64 gBuffersRead = 0;
    constexpr qint64 gTestBufferLength = 1500;
    const std::vector<char> gFixedTestBuffer =
        ranges::views::closed_iota('A', 'Z')
        | ranges::views::cycle
        | ranges::views::take(gTestBufferLength + 500)
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
    // enable dark and light mode support to follow the windows settings.
    qApp->setStyle(QStyleFactory::create("Fusion"));
    gErrorCounter = gWriteBlockCounter = gReadBlockCounter = gUnderruns = 0;
    resetFormFields();
}

void
MainWindow::resetFormFields() const
{
    ui->errorCounter->setText(QString::number(gErrorCounter));
    ui->writeBlockCounter->setText(QString::number(gWriteBlockCounter));
    ui->readBlockCounter->setText(QString::number(gReadBlockCounter));
    ui->bufferSize->setText(QString("%L1").arg(mpBuffer->size()));
    ui->bytesWritten->setText(QString("%L1").arg(gBytesWritten));
    ui->bytesRead->setText(QString("%L1").arg(gBytesRead));
    ui->basicBlockSize->setValue(mpBuffer->chunkSize());
    ui->underruns->setText(QString("%L1").arg(gUnderruns));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == mpTimer->timerId()) {
        // min 1, max 5
        const auto writeReadRatio = ui->writeReadRatio->value();
        // more writes than reads 4:1
        if (++gCounter % (writeReadRatio+1) != 0) {
            // write multiple blocks into buffer
            qint64 offset = 0;
            const auto blockLen = ui->writeBlockSize->value();
            auto nBlocksWritten = 0;
            while (offset < gTestBufferLength) {
                const auto nRemaining = gTestBufferLength - offset;
                const auto nBytesToWrite = std::min<qint64>(
                    nRemaining, blockLen);
                // this API does not fail - pattern slightly different for each gBuffersWritten
                mpBuffer->append(gFixedTestBuffer.data() + offset + gBuffersWritten, nBytesToWrite);
                offset += nBytesToWrite;
                gBytesWritten += nBytesToWrite;
                nBlocksWritten++;
            }
            gWriteBlockCounter += nBlocksWritten;
            // assuming ascii pattern buffer - show buffer data preview
            ui->writePreview->setText(QByteArray(gFixedTestBuffer.data() +
                gBuffersWritten, 18)/*.toHex(',')*/ + "...");
            gBuffersWritten++;
            ui->writeBlockCounter->setText(QString(
                "%L1").arg(gWriteBlockCounter));
            ui->bytesWritten->setText(QString(
                "%L1").arg(gBytesWritten));

        } else {
            // read multiple blocks into readBuffer
            qint64 offset = 0;
            const auto blockLen = ui->readBlockSize->value();
            auto nBlocksRead = 0;
            while (offset < gTestBufferLength) {
                const auto nRemaining = gTestBufferLength - offset;
                const auto nBytesToRead = std::min<qint64>(
                    nRemaining, blockLen);
                const auto nBytesRead = mpBuffer->read(
                    readBuffer.data() + offset, nBytesToRead);
                if (nBytesRead > 0) {
                    offset += nBytesRead;
                    gBytesRead += nBytesRead;
                    nBlocksRead++;
                } else if (offset < gTestBufferLength) {
                    // cannot read from empty buffer
                    ++gUnderruns;
                    ui->underruns->setText(QString(
                        "%L1").arg(gUnderruns));
                    break;
                }
            }
            gReadBlockCounter += nBlocksRead;
            ui->readBlockCounter->setText(QString(
                "%L1").arg(gReadBlockCounter));
            ui->bytesRead->setText(QString(
                "%L1").arg(gBytesRead));
            if (offset == gTestBufferLength) {
                // compare sliding window cyclic buffer pattern
                if (memcmp(readBuffer.data(),
                    gFixedTestBuffer.data() + gBuffersRead,
                    gTestBufferLength) != 0) {
                    ui->errorCounter->setText(QString(
                        "%L1").arg(++gErrorCounter));
                }
                // assuming ascii pattern buffer - show buffer data preview
                ui->readPreview->setText(QByteArray(
                    readBuffer.data(), 18)/*.toHex(',')*/ + "...");
                gBuffersRead++;
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
        gErrorCounter = 0;
        gWriteBlockCounter = 0;
        gReadBlockCounter = 0;
        gBytesWritten = 0;
        gBytesRead = 0;
        mpBuffer->setChunkSize(ui->basicBlockSize->value());
        // not sure if this can change while running
        ui->basicBlockSize->setEnabled(false);
        resetFormFields();
        // disable button until protocol started
        ui->start->setText("Stop");
        mpTimer->start(50, Qt::PreciseTimer, this);
    } else {
        if (mpTimer->isActive()) {
            mpTimer->stop();
        }
        // not sure if this can change while running
        ui->basicBlockSize->setEnabled(true);
        ui->start->setText("Start");
    }
}
