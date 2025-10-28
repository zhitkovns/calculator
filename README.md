Архитектура проекта:
```
Calculator/                   // Корень проекта
├── calc.exe                  // Исполняемый файл (создается после сборки)
├── plugins/                  // Папка с плагинами
│   ├── lib_cos.dll
│   ├── lib_sin.dll           
│   ├── lib_ln.dll
│   └── ...                   // Любые другие .dll
└── src/                      // Исходный код
    ├── core/
    │   ├── Calculator.h  
    │   ├── Calculator.cpp   
    │   ├── ExpressionParser.h
    │   ├── ExpressionParser.cpp
    │   ├── OperationFactory.h
    │   ├── OperationFactory.cpp
    │   └── AST/
    │       ├── Node.h
    │       ├── BinaryNode.h
    │       ├── BinaryNode.cpp
    │       ├── UnaryNode.h
    │       ├── UnaryNode.cpp
    │       ├── FunctionNode.h
    │       ├── FunctionNode.cpp
    │       ├── NumberNode.h
    │       └── NumberNode.cpp
    ├── operations/
    │   ├── IOperation.h
    │   ├── BuiltinOperations.h
    │   ├── BuiltinOperations.cpp
    │   └── wrappers/
    │       ├── ExtensionOperationWrapper.h
    │       └── ExtensionOperationWrapper.cpp
    ├── extensions/
    │   ├── CMakeLists.txt
    │   ├── CosExtension/
    │   │   └── CosExtension.cpp
    │   └── ...
    ├── plugins/
    │   ├── PluginManager.h
    │   ├── PluginManager.cpp
    │   ├── ExtensionAPI.h
    │   ├── ExtensionRegistry.h
    │   ├── ExtensionRegistry.cpp
    │   ├── ExtensionUnit.h
    │   ├── ExtensionUnit.cpp
    │   ├── HostIntegration.h
    │   ├── HostIntegration.cpp
    │   ├── DynamicLibrary.h
    │   └── DynamicLibrary.cpp
    ├── CMakeLists.txt 
    └── main.cpp
```
