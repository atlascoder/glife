#ifndef LIFECONTROLLER_H
#define LIFECONTROLLER_H

#include <QObject>
#include <QBitmap>
#include <QThread>
#include <QMouseEvent>
#include <QTimer>
#include <memory>
#include "glalgo.h"

class LifeController: public QThread
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY sizeChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY sizeChanged)
    Q_PROPERTY(QBitmap bitmap READ bitmap NOTIFY dataChanged)
    Q_PROPERTY(long generation READ generation NOTIFY dataChanged)
    Q_PROPERTY(long population READ population NOTIFY dataChanged)
    Q_PROPERTY(long lastGenerationTime READ lastGenerationTime NOTIFY dataChanged)
    Q_PROPERTY(bool insertingMode READ insertingMode WRITE setInsertingMode NOTIFY insertingModeChanged)
    Q_PROPERTY(bool simulationActive READ simulationActive NOTIFY simulationActiveChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(int borderMode READ borderMode WRITE setBorderMode NOTIFY borderModeChanged)
public:
    LifeController(QObject* parent = nullptr);
    ~LifeController() override;

    Q_INVOKABLE void insertSample(const QBitmap& sample, int x, int y, bool clearReact = false);

    int width() const;
    void setWidth(const int value);

    int height() const;
    void setHeight(const int value);

    bool insertingMode() const { return mInsertingMode; }
    void setInsertingMode(const bool value);

    bool simulationActive() const { return mSimulationActive; }
    void setSimulationActive(const bool value);

    int interval() const { return mTimer.interval(); }
    void setInterval(const int value);

    int borderMode() const { return mBorderMode; }
    void setBorderMode(const int borderMode);

    QBitmap bitmap() const;

    long generation() const;
    long population() const;
    long lastGenerationTime() const;

public slots:
    void start();
    void stop();
    void step();
    void clear();
    void randomize();
    void toggle(int x, int y);

private slots:
    void doGeneration();

signals:
    void sizeChanged();
    void dataChanged();

    void generate();

    void insertingModeChanged();
    void simulationActiveChanged();

    void intervalChanged();
    void borderModeChanged();

protected:
    void run() override;

private:
    GLAlgo* mAlgo;
    bool mInsertingMode;
    bool mSimulationActive;
    QTimer mTimer;
    std::atomic<bool> mGenerating;
    int mBorderMode;
};

#endif // LIFECONTROLLER_H
