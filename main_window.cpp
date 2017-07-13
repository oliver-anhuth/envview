#include "main_window.h"

#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>

// Nicer syntax for connect to member function (kudos to Olivier Goffart)
// e.g. connect(MEM_FUN(&source, signal), MEM_FUN(&target, method))
#define MEM_FN(obj, func) obj, [&]() { \
    using Type = std::remove_reference<decltype(*(obj))>::type; \
    return &Type::func; }()

EnvView::EnvView(QWidget * parent) : QMainWindow{parent}
{
    setWindowTitle(tr("Environment"));
    setWindowIcon(QIcon(":envview.png"));

    // Create model with environment variables (key/value pairs)
    QStandardItemModel * model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Value"));

    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    QStringList names = environment.keys();
    names.sort(Qt::CaseInsensitive);
    for (int i = 0; i < names.size(); ++i) {
        QString name = names[i];
        QString value = environment.value(name);

        QStandardItem * nameItem = new QStandardItem{name};
        model->setItem(i, NameColumnIndex, nameItem);

        QStandardItem * valueItem = new QStandardItem{value};
        model->setItem(i, ValueColumnIndex, valueItem);
    }

    // Create view and set model for environment
    itsView = new QTreeView{this};
    itsView->setModel(model);
    itsView->setAlternatingRowColors(true);
    setCentralWidget(itsView);

    // Set single row select, non editable
    itsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    itsView->selectionModel()->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
    itsView->selectionModel()->setCurrentIndex(model->index(0, 1), QItemSelectionModel::Select);
    itsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Create ui actions
    QAction * exitAction = new QAction{tr("&Exit"), this};
    exitAction->setShortcut(QKeySequence::Quit);
    connect(MEM_FN(exitAction, triggered), [this]() {
        close();
    });

    QAction * copyValueAction = new QAction{tr("Copy &Value"), this};
    copyValueAction->setShortcut(QKeySequence::Copy);
    connect(MEM_FN(copyValueAction, triggered), [this]() {
        copyToClipboard(Value);
    });

    QAction * copyNameAction = new QAction{tr("Copy &Name"), this};
    copyNameAction->setShortcut(Qt::CTRL | Qt::Key_N);
    connect(MEM_FN(copyNameAction, triggered), [this]() {
        copyToClipboard(Name);
    });

    QAction * copyNameValuePairAction = new QAction{tr("Copy Name/Value &Pair"), this};
    copyNameValuePairAction->setShortcut(Qt::CTRL | Qt::Key_P);
    connect(MEM_FN(copyNameValuePairAction, triggered), [this]() {
        copyToClipboard(Name | Value);
    });

    QAction * aboutAction = new QAction{tr("&About"), this};
    connect(MEM_FN(aboutAction, triggered), MEM_FN(this, onAbout));

    // Create main window menus
    QMenuBar * mainMenuBar = menuBar();

    QMenu * fileMenu = mainMenuBar->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    QMenu * editMenu = mainMenuBar->addMenu(tr("&Edit"));
    editMenu->addAction(copyValueAction);
    editMenu->addAction(copyNameAction);
    editMenu->addAction(copyNameValuePairAction);

    QMenu * helpMenu = mainMenuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);

    // Enable and set context menu actions for environment list
    itsView->setContextMenuPolicy(Qt::ActionsContextMenu);
    itsView->addAction(copyValueAction);
    itsView->addAction(copyNameAction);
    itsView->addAction(copyNameValuePairAction);

    // Restore window geometry from settings
    QSettings settings;
    QByteArray geometry = settings.value(SettingGeometry).toByteArray();
    restoreGeometry(geometry);

    // Restore environment list header state from settings
    const QByteArray & headerState = settings.value(SettingHeaderState).toByteArray();
    itsView->header()->restoreState(headerState);
}

EnvView::~EnvView()
{
}

void EnvView::closeEvent(QCloseEvent * event)
{
    QSettings settings;
    settings.setValue(SettingGeometry, saveGeometry());
    settings.setValue(SettingHeaderState, itsView->header()->saveState());
    settings.sync();
    BaseClass::closeEvent(event);
}

// Determine selected row and copy requested columns to clipboard
void EnvView::copyToClipboard(int what)
{
    int row = itsView->currentIndex().row();
    QString key = itsView->model()->index(row, NameColumnIndex).data().toString();
    QString value = itsView->model()->index(row, ValueColumnIndex).data().toString();

    QString text;
    if ((what & Name) != 0) {
        text.append(key);
    }
    if ((((what & Name) != 0)) && (((what & Value) != 0))) {
        text.append("=");
    }
    if ((what & Value) != 0) {
        text.append(value);
    }

    if (!text.isEmpty()) {
        QApplication::clipboard()->setText(text);
    }
}

void EnvView::onAbout()
{
    QString applicationName = tr("Environment Viewer");
    QString version = "1.0.1";
    QString aboutMessage = tr(
                "Environment Viewer %1\n\n"
                "Code by Oliver Anhuth\n\n"
                "Copyright (c) 2015 Oliver Anhuth\n\n"
                "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and "
                "associated documentation files (the \"Software\"), to deal in the Software without restriction, "
                "including without limitation the rights to use, copy, modify, merge, publish, distribute, "
                "sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is "
                "furnished to do so, subject to the following conditions:\n\n"
                "The above copyright notice and this permission notice shall be included in all copies or substantial "
                "portions of the Software.\n\n"
                "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT "
                "NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND "
                "NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES "
                "OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN "
                "CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.").arg(version);
    QMessageBox::about(this, applicationName, aboutMessage);
}
