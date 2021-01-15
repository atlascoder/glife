#include "lifecontroller.h"
#include "glnaive.h"

#include <QDebug>

LifeController::LifeController(QObject* parent): QThread(parent),
    mAlgo(new GLNaive),
    mInsertingMode(false),
    mTimer(),
    mGenerating(false)
{
    this->QThread::start();
    connect(&mTimer, &QTimer::timeout, this, &LifeController::step);
}

LifeController::~LifeController()
{
    quit();
    wait();
}

int LifeController::width() const
{
    return mAlgo->width();
}

void LifeController::setWidth(const int value)
{
    int w = value - value % 8;
    if (w == mAlgo->width()) return;
    mAlgo->setSize(w, mAlgo->height());
    emit sizeChanged();
    emit dataChanged();
}

int LifeController::height() const
{
    return mAlgo->height();
}

void LifeController::setHeight(const int value)
{
    int h = value - value % 8;
    if (h == mAlgo->height()) return;
    mAlgo->setSize(mAlgo->width(), h);
    emit sizeChanged();
    emit dataChanged();
}

QBitmap LifeController::bitmap() const
{
    return mAlgo->drawUniverse();
}

void LifeController::start()
{
    mTimer.start();
    setSimulationActive(true);
}

void LifeController::stop()
{
    mTimer.stop();
    setSimulationActive(false);
}

void LifeController::step()
{
    if (!mGenerating)
        emit generate();
}

void LifeController::clear()
{
    mAlgo->reset();
    emit dataChanged();
}

void LifeController::randomize()
{
    mAlgo->randomize();
    emit dataChanged();
}

void LifeController::toggle(int x, int y)
{
    mAlgo->toggle(x,y);
    emit dataChanged();
}

long LifeController::generation() const
{
    return mAlgo->generation();
}

long LifeController::population() const
{
    return mAlgo->population();
}

long LifeController::lastGenerationTime() const
{
    return mAlgo->lastGenerationTime();
}

void LifeController::run()
{
    qDebug() << "Run...";
    connect(this, &LifeController::generate, this, &LifeController::doGeneration);
    exec();
}

void LifeController::doGeneration()
{
    mGenerating = true;
    mAlgo->nextGen();
    emit dataChanged();
    mGenerating = false;
}

void LifeController::setInsertingMode(const bool value)
{
    if (value == mInsertingMode) return;
    mInsertingMode = value;
    emit insertingModeChanged();
}

void LifeController::insertSample(const QBitmap &sample,int x, int y, bool clearReact)
{
    mAlgo->insertSample(sample, x, y);
    emit dataChanged();
}

void LifeController::setSimulationActive(const bool value)
{
    if (value == mSimulationActive) return;
    mSimulationActive = value;
    if (value) {
        setInsertingMode(false);
    }
    emit simulationActiveChanged();
}

void LifeController::setInterval(const int value)
{
    if (value == mTimer.interval()) return;
    mTimer.setInterval(value);
    emit intervalChanged();
}
