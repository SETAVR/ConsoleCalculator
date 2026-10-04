#include "calculator.h"

#include <QtGlobal>
#include <algorithm>

Calculator::Calculator(QObject *parent)
    : QObject(parent)
    , m_result(0.0)
    , m_hasError(false)
{
}

double Calculator::result() const
{
    return m_result;
}

bool Calculator::hasError() const
{
    return m_hasError;
}

QString Calculator::errorMessage() const
{
    return m_errorMessage;
}

void Calculator::setResult(double value)
{
    m_result = value;
    m_hasError = false;
    m_errorMessage.clear();
    emit resultReady(m_result);
}

void Calculator::add(double a, double b)
{
    setResult(a + b);
}

void Calculator::subtract(double a, double b)
{
    setResult(a - b);
}

void Calculator::multiply(double a, double b)
{
    setResult(a * b);
}

void Calculator::divide(double a, double b)
{
    if (qFuzzyIsNull(b)) {
        reportError("Деление на ноль невозможно");
        return;
    }

    setResult(a / b);
}

void Calculator::maxOfThree(double a, double b, double c)
{
    setResult(std::max({a, b, c}));
}

void Calculator::reset()
{
    setResult(0.0);
}

void Calculator::reportError(const QString &message)
{
    m_hasError = true;
    m_errorMessage = message;
    emit errorOccurred(m_errorMessage);
}
