#include "listmodel.hpp"

#include <QListView>
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
    lv2.setModel(new EasyModels::List<Person> {
                    { Person { "Julian", 20 },
                      Person { "Niki", 27 },
                      Person { "Emerson", 19 } },
                    { { Qt::DisplayRole, [](Person &p) -> QVariant { return p.name; } },
                      { Qt::ToolTipRole, [](Person &p) -> QVariant {
                            return "<b>" + p.name + ":</b> <i>(aged " + QString::number(p.age) + ")";
                        } } },
                    &lv2
                });
    lv2.show();

    a.exec();
}
