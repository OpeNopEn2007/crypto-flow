# Qt State Machine

> **来源 URL**: https://doc.qt.io/qt-6/qtstatemachine-module.html
> **获取日期**: 2026-05-12
> **模块说明**: 状态机框架

---

# Qt State Machine C++ Classes

The Qt State Machine module provides classes for creating and executing state graphs.

## Classes

  [QAbstractState](qabstractstate.html)                            The base class of states of a QStateMachine
  [QAbstractTransition](qabstracttransition.html)                  The base class of transitions between QAbstractState objects
  [QEventTransition](qeventtransition.html)                        QObject-specific transition for Qt events
  [QFinalState](qfinalstate.html)                                  Final state
  [QHistoryState](qhistorystate.html)                              Means of returning to a previously active substate
  [QKeyEventTransition](qkeyeventtransition.html)                  Transition for key events
  [QMouseEventTransition](qmouseeventtransition.html)              Transition for mouse events
  [QSignalTransition](qsignaltransition.html)                      Transition based on a Qt signal
  [QState](qstate.html)                                            General-purpose state for QStateMachine
  [QStateMachine](qstatemachine.html)                              Hierarchical finite state machine
  [QStateMachine::SignalEvent](qstatemachine-signalevent.html)     Represents a Qt signal event
  [QStateMachine::WrappedEvent](qstatemachine-wrappedevent.html)   Inherits QEvent and holds a clone of an event associated with a QObject

## Detailed Description

The [Qt State Machine](qtstatemachine-index.html) page contains information about how to use the module.
