#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTreeView;

class EnvView : public QMainWindow
{
public:
    using BaseClass = QMainWindow;

    explicit EnvView(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *) override;

private:
    static constexpr const char * SettingGeometry = "Geometry";
    static constexpr const char * SettingHeaderState = "HeaderState";

    enum { NameColumnIndex = 0, ValueColumnIndex = 1 };

    enum What { Name = 0x1, Value = 0x2 };
    void copyToClipboard(int what);

    void onAbout();

    QTreeView * itsView;

    Q_OBJECT
};

#endif // MAINWINDOW_H
