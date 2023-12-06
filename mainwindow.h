#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QDebug"
#include "QFileSystemModel"
#include "QTreeView"
#include "QDir"
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>
#include <QMouseEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
//#include "treeviewstate.h"

//Pridat messageboxi!!!!!!!!!!!!!!!!!!!!!!!!!!!

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //void contextMenuEvent(QContextMenuEvent* event);

    void renameItem();
    void deleteItem();
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


private slots:

    void on_treeView_clicked(const QModelIndex &index);

    void on_restartBtn_clicked();

    void on_backBtn_clicked();

    void on_treeView_entered(const QModelIndex &index) {};

    void on_treeView_expanded(const QModelIndex &index) {};

    void on_treeView_collapsed(const QModelIndex &index) {};

    void on_searchBtn_clicked();

    void on_nextBtn_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_customContextMenuRequested(const QPoint &pos);

private:
    int count = 0;
    QSet<int> expanded_ids_;
    QTreeView* tree_view_;
    Ui::MainWindow *ui;
    QFileSystemModel* dirmodel;
    QFileSystemModel* filemodel;
    std::vector<QString> pastDirPaths;
    std::vector<QString> futureDirPaths;
    QString presentDirPath;
    QString lastAction;
    QString startPath;


};
#endif // MAINWINDOW_H
