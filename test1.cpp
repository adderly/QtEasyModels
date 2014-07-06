#include "listmodel.hpp"

#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>

struct Person
{
    QString name;
    int age;
};

int main(int argc, char **argv)
{
    QApplication a { argc, argv };

    QListView lv;
    lv.setWindowTitle("Non-editable 1");
    lv.setModel(new EasyModels::List<Person> {
                    { Person { "Julian", 20 },
                      Person { "Niki", 27 },
                      Person { "Emerson", 19 } },
                    [](Person &p) -> QVariant {
                        return p.name + " (age " + QString::number(p.age) + ")";
                    },
                    &lv });

    lv.show();

    QListView lv2;
    lv2.setWindowTitle("Non-editable 2");
    lv2.setModel(new EasyModels::List<Person> {
                    { Person { "Julian", 20 },
                      Person { "Niki", 27 },
                      Person { "Emerson", 19 } },
                    { { Qt::DisplayRole, [](Person &p) -> QVariant { return p.name; } },
                      { Qt::ToolTipRole, [](Person &p) -> QVariant {
                            return "<b>" + p.name + ":</b> <i>(aged " + QString::number(p.age) + ")";
                        } } },
                    {},
                    &lv2
                });
    lv2.show();

    QListView lv3;
    lv3.setWindowTitle("Editable");
    lv3.setModel(new EasyModels::List<Person> {
                     { Person { "Julian", 20 },
                       Person { "Niki", 27 },
                       Person { "Emerson", 19 } },
                     [](Person &p) -> QVariant {
                         return p.name;
                     },
                     [](Person &p, const QVariant value) -> bool {
                         p.name = value.toString();

                         return true;
                     },
                     &lv3
                 });
    lv3.show();

    auto model4 = new EasyModels::List<Person> {
        {},

        [](Person &p) -> QVariant {
            return "Name: " + p.name + ", age: " + QString::number(p.age);
        },

        [](Person &p, const QVariant &value) -> bool {
            p.name = value.toString(); return true;
        }
    };

    QWidget w;
    w.setWindowTitle("Appendable");
    w.setLayout(new QVBoxLayout { &w });

    QListView lv4;
    lv4.setModel(model4);
    w.layout()->addWidget(&lv4);

    QPushButton button { "Append" };
    QObject::connect(&button, &QPushButton::clicked, [=](){
        model4->append(Person { "Dolly", 17 });
    });
    w.layout()->addWidget(&button);

    w.show();

    a.exec();

    delete model4;
}
