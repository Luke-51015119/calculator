#include "widget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setFixedHeight(40);

    // 建立所有按鈕
    QStringList buttons = {
        "←", "/", "*", "-",
        "7", "8", "9", "+",
        "4", "5", "6", "=",
        "1", "2", "3", "C",
        "0", ".", ""
    };

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(display, 0, 0, 1, 4); // 顯示框

    int pos = 0;
    for (int row = 1; row <= 5; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (pos >= buttons.size()) break;

            QString text = buttons[pos++];
            if (text.isEmpty()) continue;

            QPushButton *btn = new QPushButton(text, this);
            btn->setFixedSize(60, 40);

            // 特殊按鍵調整大小
            if (text == "0") {
                layout->addWidget(btn, row, col, 1, 2); // 占兩格橫向
                ++col; // 跳過下一格
            }
            else {
                layout->addWidget(btn, row, col);
            }

            connect(btn, &QPushButton::clicked, this, &Widget::onButtonClicked);
        }
    }

    setLayout(layout);
    setWindowTitle("Simple Calculator");
}

void Widget::onButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString text = btn->text();

    if (text == "C") {       // 清除輸入
        display->clear();
    }
    else if (text == "←") {  // 倒退鍵
        display->backspace();
    }
    else if (text == "=" || text == "Enter") {  // 計算
        QString expr = display->text();
        double result = 0.0;

        if (expr.contains("+")) {
            QStringList parts = expr.split("+");
            if (parts.size() == 2)
                result = parts[0].toDouble() + parts[1].toDouble();
        } else if (expr.contains("-")) {
            QStringList parts = expr.split("-");
            if (parts.size() == 2)
                result = parts[0].toDouble() - parts[1].toDouble();
        } else if (expr.contains("*")) {
            QStringList parts = expr.split("*");
            if (parts.size() == 2)
                result = parts[0].toDouble() * parts[1].toDouble();
        } else if (expr.contains("/")) {
            QStringList parts = expr.split("/");
            if (parts.size() == 2) {
                double denom = parts[1].toDouble();
                if (denom != 0)
                    result = parts[0].toDouble() / parts[1].toDouble();
                else {
                    display->setText("Error");
                    return;
                }
            }
        } else { // 單一數字直接輸出
            bool ok;
            double value = expr.toDouble(&ok);
            if (ok) {
                result = value;
            } else {
                display->setText("Error");
                return;
            }
        }

        display->setText(QString::number(result));
    }
    else {  // 一般數字或符號輸入
        display->setText(display->text() + text);
    }
}


Widget::~Widget() {}
