#include "lifecontroller.h"
#include "glnaive.h"

#include <QDebug>

LifeController::LifeController(QObject* parent): QThread(parent),
    mAlgo(new GLNaive),
    mInsertingMode(false),
    mSimulationActive(false),
    mTimer(),
    mGenerating(false),
    mBorderMode(BorderRule::CLOSING)
{
    this->QThread::start();
    connect(&mTimer, &QTimer::timeout, this, &LifeController::step);
}

LifeController::~LifeController()
{
    stop();
    if (QThread::isRunning()) {
        quit();
        wait();
    }
    delete mAlgo;
}

int LifeController::width() const
{
    return mAlgo->width();
}

void LifeController::setWidth(const int value)
{
    unsigned w = static_cast<unsigned>(value - value % 8);
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
    unsigned h = static_cast<unsigned>(value - value % 8);
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
    return static_cast<long>(mAlgo->generation());
}

long LifeController::population() const
{
    return  static_cast<long>(mAlgo->population());
}

long LifeController::lastGenerationTime() const
{
    return mAlgo->lastGenerationTime();
}

void LifeController::run()
{
    connect(this, &LifeController::generate, this, &LifeController::doGeneration);
    exec();
}

void LifeController::doGeneration()
{
    mGenerating = true;
    mAlgo->setBorderRule(BorderRule(mBorderMode));
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
    Q_UNUSED(clearReact)
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

void LifeController::setBorderMode(const int borderMode)
{
    if (borderMode == mBorderMode) return;
    mBorderMode = borderMode;
    emit borderModeChanged();
}
