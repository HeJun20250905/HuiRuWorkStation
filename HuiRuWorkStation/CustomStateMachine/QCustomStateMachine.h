#pragma once
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTableWidget>

class QCustomStateMachine  : public QObject
{
	Q_OBJECT

public:
	QCustomStateMachine(QObject *parent);
	~QCustomStateMachine();

    bool OnInitMachine();

    void Start();
    void Stop();

private:
    QStateMachine* m_StateMachine;
    QState* m_idleState;
    QState* m_workingState;

    bool m_skipFirstIdle = true;

signals:
    void startRequested();
    void stopRequested();
    void stateEntered(const QString& stateName);
};
