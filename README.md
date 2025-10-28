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
│   │   ├── TestExtensionRegistry.cpp
│   │   ├── TestBuiltinOperations.cpp
│   │   └── TestErrorHandling.cpp
│   ├── integration/                // Интеграционные тесты
│   │   ├── TestFullExpressions.cpp
│   │   └── TestPlugins.cpp
│   ├── mocks/                      // Моки для тестов
│   │   └── MockOperation.h
│   ├── utils/                      // Утилиты для тестов
│   │   └── TestHelpers.h
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
- NumberNodeEvaluation - вычисление числового узла
- BinaryNodeEvaluation - вычисление бинарных операций
- UnaryNodeEvaluation - вычисление унарных операций
- FunctionNodeEvaluation - вычисление функций

### BuiltinOperations тесты (TestBuiltinOperations.cpp)
- AddOperation - сложение
- SubtractOperation - вычитание
- MultiplyOperation - умножение
- DivideOperation - деление
- DivisionByZero - деление на ноль

### ExpressionParser тесты (TestExpressionParser.cpp)
- SimpleAddition - парсинг сложения
- OperatorPrecedence - приоритет операторов
- Parentheses - работа со скобками
- ComplexExpression - сложные выражения
- InvalidExpressionThrows - обработка ошибок парсера
- FloatingPointNumbers - числа с плавающей точкой
- NegativeNumbers - отрицательные результаты
- MultipleSpaces - выражения с пробелами
- DeeplyNestedParentheses - вложенные скобки

### Calculator тесты (TestCalculator.cpp)
- CreationAndInitialization - создание калькулятора
- BasicOperations - базовые операции
- ComplexExpressions - сложные выражения
- AvailableOperations - список операций
- DivisionByZero - обработка деления на ноль
- InvalidExpression - обработка некорректных выражений

### ErrorHandling тесты (TestErrorHandling.cpp)
- InvalidCharacters - недопустимые символы
- UnbalancedParentheses - несбалансированные скобки
- InvalidFunctionCalls - некорректные вызовы функций

## Интеграционные тесты

### FullExpressions тесты (TestFullExpressions.cpp)
- ComplexMathematicalExpression - сложные выражения
- NestedParentheses - вложенные скобки
- OperatorPrecedenceWithMixedOperations - приоритеты операций
- ComplexExpressionWithAllOperations - все типы операций

### Plugin тесты (TestPlugins.cpp)
- PluginOperationsAvailable - доступность плагинов
- PluginOperationsCalculation - вычисления через плагины
- ComplexExpressionWithPlugins - выражения с плагинами