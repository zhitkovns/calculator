# Задание "Calculator"
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



## Архитектура проекта:
```
Calculator/
├── calc.exe                         // Основная программа
├── tests.exe                        // Исполняемый файл тестов
├── plugins/                         // Папка с плагинами
│   ├── lib_cos.dll
│   ├── lib_sin.dll
│   ├── lib_ln.dll
│   └── ...
├── src/                            // Исходный код
│   ├── core/
│   │   ├── Calculator.h
│   │   ├── Calculator.cpp
│   │   ├── ExpressionParser.h
│   │   ├── ExpressionParser.cpp
│   │   ├── OperationFactory.h
│   │   ├── OperationFactory.cpp
│   │   └── AST/
│   │       ├── Node.h
│   │       ├── BinaryNode.h
│   │       ├── BinaryNode.cpp
│   │       ├── UnaryNode.h
│   │       ├── UnaryNode.cpp
│   │       ├── FunctionNode.h
│   │       ├── FunctionNode.cpp
│   │       ├── NumberNode.h
│   │       └── NumberNode.cpp
│   ├── operations/
│   │   ├── IOperation.h
│   │   ├── BuiltinOperations.h
│   │   ├── BuiltinOperations.cpp
│   │   └── wrappers/
│   │       ├── ExtensionOperationWrapper.h
│   │       └── ExtensionOperationWrapper.cpp
│   ├── extensions/
│   │   ├── CMakeLists.txt
│   │   ├── CosExtension/
│   │   │   └── CosExtension.cpp
│   │   └── ...
│   ├── plugins/
│   │   ├── ExtensionAPI.h
│   │   ├── ExtensionRegistry.h
│   │   ├── ExtensionRegistry.cpp
│   │   ├── ExtensionUnit.h
│   │   ├── ExtensionUnit.cpp
│   │   ├── HostIntegration.h
│   │   ├── HostIntegration.cpp
│   │   ├── DynamicLibrary.h
│   │   └── DynamicLibrary.cpp
│   ├── CMakeLists.txt
│   └── main.cpp
├── tests/                          // Папка с тестами
│   ├── unit/                       // Unit-тесты
│   │   ├── TestCalculator.cpp
│   │   ├── TestExpressionParser.cpp
│   │   ├── TestAST.cpp
│   │   ├── TestBuiltinOperations.cpp
│   │   └── TestErrorHandling.cpp
│   ├── integration/                // Интеграционные тесты
│   │   ├── TestFullExpressions.cpp
│   │   └── TestPlugins.cpp
│   ├── test_framework.h            // Framework для тестов
│   └── main.cpp                    // Главный файл тестов
├── CMakeLists.txt                  // Главный CMakeLists
└── README.md
```

## Сборка проекта
1. Через Command Palette:
- Ctrl+Shift+P → CMake: Configure
- Ctrl+Shift+P → CMake: Build
2. Через терминал:
- Конфигурация:
```bash
cmake -B build -DBUILD_TESTS=ON
```
- Сборка:
```bash
cmake --build build
```


## Запуск приложения
После сборки проекта:
```bash
./calc.exe
```


# Тестирование Calculator

## Запуск тестов
После сборки проекта:
```bash
./tests.exe
```

## Unit-тесты

### AST тесты (TestAST.cpp)
- **NumberNodeEvaluation** - вычисление числового узла с различными значениями
- **BinaryNodeEvaluation** - вычисление бинарных операций (сложение, вычитание, умножение, деление)
- **UnaryNodeEvaluation** - вычисление унарных операций (унарный минус)
- **FunctionNodeEvaluation** - вычисление функций с различным количеством аргументов

### BuiltinOperations тесты (TestBuiltinOperations.cpp)
- **AddOperation** - операция сложения с положительными и отрицательными числами
- **SubtractOperation** - операция вычитания с различными комбинациями чисел
- **MultiplyOperation** - операция умножения, включая умножение на ноль
- **DivideOperation** - операция деления с корректными аргументами
- **DivisionByZero** - обработка деления на ноль с выбрасыванием исключения

### ExpressionParser тесты (TestExpressionParser.cpp)
- **SimpleAddition** - парсинг простых выражений сложения
- **OperatorPrecedence** - проверка приоритета операторов (умножение перед сложением)
- **Parentheses** - корректная обработка скобок и изменение приоритета операций
- **ComplexExpression** - парсинг сложных математических выражений
- **InvalidExpressionThrows** - обработка синтаксически некорректных выражений
- **FloatingPointNumbers** - корректный парсинг чисел с плавающей точкой
- **NegativeNumbers** - работа с отрицательными числами и результатами
- **MultipleSpaces** - игнорирование лишних пробелов в выражениях
- **DeeplyNestedParentheses** - обработка выражений с глубоко вложенными скобками

### Calculator тесты (TestCalculator.cpp)
- **CreationAndInitialization** - создание калькулятора и инициализация операций
- **BasicOperations** - выполнение базовых арифметических операций
- **ComplexExpressions** - вычисление сложных математических выражений
- **AvailableOperations** - проверка списка доступных встроенных операций
- **DivisionByZero** - корректная обработка деления на ноль в калькуляторе
- **InvalidExpression** - обработка некорректных выражений с выводом ошибок

### ErrorHandling тесты (TestErrorHandling.cpp)
- **InvalidCharacters** - обработка выражений с недопустимыми символами
- **UnbalancedParentheses** - обнаружение несбалансированных скобок
- **InvalidFunctionCalls** - обработка некорректных вызовов функций (неправильное количество аргументов)

## Интеграционные тесты

### FullExpressions тесты (TestFullExpressions.cpp)
- **ComplexMathematicalExpression** - вычисление сложных математических выражений со смешанными операциями
- **NestedParentheses** - работа с выражениями, содержащими вложенные скобки различной глубины
- **OperatorPrecedenceWithMixedOperations** - проверка приоритета операций в сложных выражениях
- **ComplexExpressionWithAllOperations** - выражения, использующие все типы доступных операций

### Plugin тесты (TestPlugins.cpp)
- **PluginOperationsAvailable** - проверка доступности операций из загруженных плагинов
- **PluginOperationsCalculation** - тестирование корректности вычислений через плагинные операции и обработки ошибок
- **ComplexExpressionWithPlugins** - вычисление сложных выражений, комбинирующих встроенные и плагинные операции