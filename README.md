Задание:
Calculator
Приложение состоит из исполняемого файла - .exe, который имеет консольный интерфейс и способен вычислять выражения с операциями + - / * ().
Ввод: 16 + 4 * (3 – 1)
Вывод: 24
При запуске приложение обращается в папку ./plugins, и из каждой dll загружает и распознает соответствующую функцию. Расширение функционала калькулятора производится динамически (на рантайме, путем анализа файлов в директории plugins), без дополнительной рекомпиляции.
./calc.exe
./plugins/funcsin.dll
./plugins/funcdeg.dll
Что приводит к возможности использовать эти функции сразу же после запуска приложения, делая валидными вычисления:
2^4 + sin(90)
Каждый плагин содержит ровно одну функцию. Имя файла dll не должно участвовать на определение самой функции и ее имени. 
Необходимо определить и обработать исключительные случаи (некорректная dll, нет никаких dll, какая-то из функций бросает исключение, например, ln(-100) )
Оценивается корректность работы с памятью и исключениями, архитектура приложения, работоспособность.



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
