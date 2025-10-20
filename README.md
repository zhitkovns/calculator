Архитектура проекта:
```
Calculator/                   // Корень проекта
├── calc.exe                  // Исполняемый файл (создается после сборки)
├── plugins/                  // Папка с плагинами
│   ├── funcsin.dll
│   ├── funcdeg.dll
│   ├── funccos.dll           // Дополнительные плагины
│   ├── funcln.dll
│   └── ...                   // Любые другие .dll
└── src/                      // Исходный код
    ├── core/
    │   ├── Calculator.h  
    │   ├── Calculator.cpp   
    │   ├── ExpressionParser.h
    │   ├── ExpressionParser.cpp
    │   └── AST/
    │       ├── Node.h
    │       ├── Node.cpp
    │       ├── BinaryNode.h
    │       ├── BinaryNode.cpp
    │       ├── UnaryNode.h
    │       ├── UnaryNode.cpp
    │       ├── NumberNode.h
    │       └── NumberNode.cpp
    ├── operations/
    │   ├── IOperation.h
    │   ├── BuiltinOperations.h
    │   └── BuiltinOperations.cpp
    ├── plugins/
    │   ├── PluginManager.h
    │   ├── PluginManager.cpp
    │   ├── DynamicLibrary.h
    │   └── DynamicLibrary.cpp
    └── main.cpp
```
