# ConsoleCalculator

Консольный калькулятор на Qt Core для лабораторной работы 1 по основам
кроссплатформенного программирования.

## Возможности

- Сложение, вычитание, умножение и деление.
- Обработка деления на ноль через сигнал `errorOccurred`.
- Индивидуальный вариант 6: слот `maxOfThree(double a, double b, double c)`.
- Второй обработчик сигнала `resultReady`, который записывает результаты в
  файл `history.txt`.

## Сборка и запуск

Откройте проект в Qt Creator или выполните команды из каталога проекта:

```bash
qmake ConsoleCalculator.pro
make
./ConsoleCalculator
```

В Windows при использовании MinGW команда сборки может быть `mingw32-make`.

## Команды программы

```text
add 5 3
sub 10 4
mul 6 7
div 8 2
max3 12 7 19
reset
help
quit
```
