#include "QCustomStateMachine.h"
#include <QFinalState>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

QCustomStateMachine::QCustomStateMachine(QObject *parent)
	: QObject(parent), m_StateMachine(nullptr), m_idleState(nullptr), m_workingState(nullptr)
{
	this->OnInitMachine();
}

QCustomStateMachine::~QCustomStateMachine()
{
    if (m_StateMachine && m_StateMachine->isRunning())
    {
        m_StateMachine->stop();
        QCoreApplication::processEvents();
    }
}

bool QCustomStateMachine::OnInitMachine()
{
    if (m_StateMachine)
        return false;

    m_StateMachine = new QStateMachine(this);

    m_idleState = new QState(m_StateMachine);
    m_idleState->setObjectName("Idle");

    m_workingState = new QState(m_StateMachine);
    m_workingState->setObjectName("Working");

    // 状态转换
    m_idleState->addTransition(this, &QCustomStateMachine::startRequested, m_workingState);
    m_workingState->addTransition(this, &QCustomStateMachine::stopRequested, m_idleState);

    // 状态进入信号
    connect(m_idleState, &QState::entered, this, [this]() 
    {
        if (m_skipFirstIdle) 
        {
            m_skipFirstIdle = false;
            return;
        }
        emit stateEntered(m_idleState->objectName());
        qDebug() << "Entered Idle";
    });
    connect(m_workingState, &QState::entered, this, [this]() 
    {
        emit stateEntered(m_workingState->objectName());
        qDebug() << "Entered Working";
    });

    m_StateMachine->setInitialState(m_idleState);

    return true;
}

void QCustomStateMachine::Start()
{
    if (!m_StateMachine)
    {
        return;
    }
    if (!m_StateMachine->isRunning())
    {
        m_StateMachine->start();
    }
    QTimer::singleShot(0, this, [this] { emit startRequested(); });
}

void QCustomStateMachine::Stop()
{
    if (!m_StateMachine)
    {
        return;
    }
    emit stopRequested();
}