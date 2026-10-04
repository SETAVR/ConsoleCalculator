#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QTextStream>

#include "calculator.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

void printHelp(QTextStream &out)
{
    out << "Доступные команды:\n";
    out << "  add <a> <b>          - сложение\n";
    out << "  sub <a> <b>          - вычитание\n";
    out << "  mul <a> <b>          - умножение\n";
    out << "  div <a> <b>          - деление\n";
    out << "  max3 <a> <b> <c>     - максимум из трех чисел, вариант 6\n";
    out << "  reset                - сброс результата\n";
    out << "  help                 - справка\n";
    out << "  quit                 - выход\n";
}

bool readNumber(const QStringList &parts, int index, double &value)
{
    bool ok = false;
    value = parts.value(index).toDouble(&ok);
    return ok;
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QCoreApplication app(argc, argv);

    QTextStream in(stdin);
    QTextStream out(stdout);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    in.setEncoding(QStringConverter::Utf8);
    out.setEncoding(QStringConverter::Utf8);
#endif

    Calculator calc;

    QFile historyFile("history.txt");
    historyFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream historyStream(&historyFile);

    QObject::connect(&calc, &Calculator::resultReady,
                     [&out](double result) {
                         out << "Результат: " << result << "\n";
                         out.flush();
                     });

    QObject::connect(&calc, &Calculator::resultReady,
                     [&historyStream](double result) {
                         historyStream << "Result: " << result << "\n";
                         historyStream.flush();
                     });

    QObject::connect(&calc, &Calculator::errorOccurred,
                     [&out, &historyStream](const QString &message) {
                         out << "Ошибка: " << message << "\n";
                         out.flush();
                         historyStream << "Error: " << message << "\n";
                         historyStream.flush();
                     });

    out << "=== Консольный калькулятор на Qt ===\n";
    printHelp(out);
    out << "\n> ";
    out.flush();

    QString line;
    while (in.readLineInto(&line)) {
        line = line.trimmed();

        if (line.isEmpty()) {
            out << "> ";
            out.flush();
            continue;
        }

        const QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        const QString command = parts.value(0).toLower();

        if (command == "quit" || command == "exit") {
            out << "До свидания!\n";
            break;
        }

        if (command == "help") {
            printHelp(out);
            out << "> ";
            out.flush();
            continue;
        }

        if (command == "reset") {
            calc.reset();
            out << "> ";
            out.flush();
            continue;
        }

        double a = 0.0;
        double b = 0.0;
        double c = 0.0;

        if (command == "max3") {
            if (parts.size() != 4 || !readNumber(parts, 1, a)
                || !readNumber(parts, 2, b) || !readNumber(parts, 3, c)) {
                calc.reportError("используйте формат: max3 <a> <b> <c>");
            } else {
                calc.maxOfThree(a, b, c);
            }
        } else if (command == "add" || command == "sub"
                   || command == "mul" || command == "div") {
            if (parts.size() != 3 || !readNumber(parts, 1, a)
                || !readNumber(parts, 2, b)) {
                calc.reportError("используйте формат: <команда> <a> <b>");
            } else if (command == "add") {
                calc.add(a, b);
            } else if (command == "sub") {
                calc.subtract(a, b);
            } else if (command == "mul") {
                calc.multiply(a, b);
            } else {
                calc.divide(a, b);
            }
        } else {
            calc.reportError("неизвестная команда: " + command);
        }

        out << "> ";
        out.flush();
    }

    return 0;
}
