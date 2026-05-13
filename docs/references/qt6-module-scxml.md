# Qt SCXML

> **来源 URL**: https://doc.qt.io/qt-6/qtscxml-module.html
> **获取日期**: 2026-05-12
> **模块说明**: 状态机 SCXML

---

# Qt SCXML C++ Classes

Provides classes to create and use state machines from SCXML files.

## Namespaces

  [QScxmlExecutableContent](qscxmlexecutablecontent.html)   Contains various types used to interpret executable content in state machines

## Classes

  [QScxmlCompiler](qscxmlcompiler.html)                                                    Compiler for SCXML files
  [QScxmlCompiler::Loader](qscxmlcompiler-loader.html)                                     URI resolver and resource loader for an SCXML compiler
  [QScxmlCppDataModel](qscxmlcppdatamodel.html)                                            C++ data model for a Qt SCXML state machine
  [QScxmlDataModel](qscxmldatamodel.html)                                                  The data model base class for a Qt SCXML state machine
  [QScxmlDataModel::ForeachLoopBody](qscxmldatamodel-foreachloopbody.html)                 Represents a function to be executed on each iteration of an SCXML foreach loop
  [QScxmlDynamicScxmlServiceFactory](qscxmldynamicscxmlservicefactory.html)                Creates SCXML service instances from documents loaded at runtime
  [QScxmlError](qscxmlerror.html)                                                          Describes the errors returned by the Qt SCXML state machine when parsing an SCXML file
  [QScxmlEvent](qscxmlevent.html)                                                          Event for a Qt SCXML state machine
  [QScxmlExecutableContent::AssignmentInfo](qscxmlexecutablecontent-assignmentinfo.html)   AssingmentInfo class represents a data assignment
  [QScxmlExecutableContent::EvaluatorInfo](qscxmlexecutablecontent-evaluatorinfo.html)     Represents a unit of executable content
  [QScxmlExecutableContent::ForeachInfo](qscxmlexecutablecontent-foreachinfo.html)         Represents a foreach construct
  [QScxmlExecutableContent::InvokeInfo](qscxmlexecutablecontent-invokeinfo.html)           Represents a service invocation
  [QScxmlExecutableContent::ParameterInfo](qscxmlexecutablecontent-parameterinfo.html)     Represents a parameter to a service invocation
  [QScxmlInvokableService](qscxmlinvokableservice.html)                                    The base class for services called from state machines
  [QScxmlInvokableServiceFactory](qscxmlinvokableservicefactory.html)                      Creates invokable service instances
  [QScxmlNullDataModel](qscxmlnulldatamodel.html)                                          The null data model for a Qt SCXML stateMachine
  [QScxmlStateMachine](qscxmlstatemachine.html)                                            Interface to the state machines created from SCXML files
  [QScxmlStaticScxmlServiceFactory](qscxmlstaticscxmlservicefactory.html)                  Creates SCXML service instances from precompiled documents
  [QScxmlTableData](qscxmltabledata.html)                                                  Used by compiled state machines

## Detailed Description

To use the module with cmake, use the `find_package()` command to locate the needed module components in the `Qt6` package:

```
find_package(Qt6 REQUIRED COMPONENTS Scxml)
target_link_libraries(mytarget PRIVATE Qt6::Scxml)
```

To configure the module for building with qmake, add the module as a value of the `QT` variable in the project's .pro file:

```
QT += scxml
```

For more information, see [Instantiating State Machines](qtscxml-instantiating-state-machines.html).
