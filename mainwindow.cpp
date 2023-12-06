#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //TreeViewState* treeViewState = new TreeViewState(ui->treeView);
    ui->setupUi(this);
    tree_view_ = ui->treeView;

    QPixmap backPixmap(":/img/img/back.png");
    QPixmap nextPixmap(":/img/img/next.png");
    QPixmap restartPixmap(":/img/img/restart.png");
    QPixmap searchPixmap(":/img/img/search.png");

    QIcon backBtnIcon(backPixmap);
    QIcon nextBtnIcon(nextPixmap);
    QIcon restartBtnIcon(restartPixmap);
    QIcon searchBtnIcon(searchPixmap);
    ui->backBtn->setIcon(backBtnIcon);
    ui->nextBtn->setIcon(nextBtnIcon);
    ui->restartBtn->setIcon(restartBtnIcon);
    ui->searchBtn->setIcon(searchBtnIcon);

    startPath = "C:/";
    ui->lineEdit->setText(startPath);
    pastDirPaths.push_back(startPath);
    presentDirPath = startPath;
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(startPath);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setColumnWidth(0, 215);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    filemodel->setRootPath(startPath);

    ui->listView->setViewMode(QListView::ListMode);
    ui->listView->setMovement(QListView::Free);
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(true);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listView->setDropIndicatorShown(true);
    ui->listView->setModel(filemodel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        MainWindow::on_searchBtn_clicked();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString text = ui->lineEdit->text(); //vraci directory pred kliknutim
    QString dirPath = dirmodel->fileInfo(index).absoluteFilePath(); //vraci path na ktery bylo kliknuto
    if(dirPath != presentDirPath)
    {
        if(pastDirPaths.size() > 10)
        {
            pastDirPaths.erase(pastDirPaths.begin());
        }
        futureDirPaths.clear();
        pastDirPaths.push_back(text);
        ui->lineEdit->setText(dirPath);
        presentDirPath = dirPath;
        ui->listView->setRootIndex(filemodel->setRootPath(dirPath));
    }
}

void MainWindow::on_restartBtn_clicked()
{
    ui->treeView->collapseAll();
    pastDirPaths.clear();
    futureDirPaths.clear();
    pastDirPaths.push_back(startPath);
    ui->lineEdit->setText(startPath);
    presentDirPath = startPath;
    ui->listView->setRootIndex(QModelIndex());
}


void MainWindow::on_backBtn_clicked()
{
    if(pastDirPaths.size() > 1)
    {
        QString str = ui->lineEdit->text();
        QString lastPath;
        lastPath = pastDirPaths[pastDirPaths.size()-1];

        if(str != lastPath)
        {
            futureDirPaths.push_back(str);
            pastDirPaths.erase(pastDirPaths.end() - 1);
            if(lastPath == startPath)
            {
                ui->listView->setRootIndex(QModelIndex());
            }
            else
            {
                ui->listView->setRootIndex(filemodel->setRootPath(lastPath));
            }

            presentDirPath = lastPath;
            ui->lineEdit->setText(lastPath);
        }
    }
    qDebug() << "pastDirPaths.size(): " << pastDirPaths.size();
    qDebug() << "futureDirPaths.size(): " << futureDirPaths.size();
}

void MainWindow::on_nextBtn_clicked()
{
    if(futureDirPaths.size() > 0)
    {
        QString str = ui->lineEdit->text();
        QString lastPath = futureDirPaths[futureDirPaths.size()-1];

        if(lastPath != str)
        {
            qDebug() << "fuuu";

            pastDirPaths.push_back(str);
            futureDirPaths.erase(futureDirPaths.end() - 1);
            ui->listView->setRootIndex(filemodel->setRootPath(lastPath));
            presentDirPath = lastPath;
            ui->lineEdit->setText(lastPath);
        }
    }
    qDebug() << "pastDirPaths.size(): " << pastDirPaths.size();
    qDebug() << "futureDirPaths.size(): " << futureDirPaths.size();
    lastAction = "next";
}

void MainWindow::on_searchBtn_clicked()
{
    QString text = ui->lineEdit->text();
    QDir directory(text);
    QFile file(text);
    if(directory.exists())
    {
        QModelIndex rootIndex = filemodel->index(directory.path());
        ui->listView->setRootIndex(rootIndex);
        pastDirPaths.push_back(presentDirPath);
        presentDirPath = directory.path();
    }
    else if(file.exists())
    {
        if (QDesktopServices::openUrl(QUrl::fromLocalFile(text))) {
            qDebug() << "Aplikace byla spuštěna.";
            ui->lineEdit->setText(presentDirPath);
        } else {
            QMessageBox::critical(this, "ERROR", "File not loaded.");
        }
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Path does not exist.");
    }
}

void MainWindow::renameItem()
{
    qDebug() << "rename";
}

void MainWindow::deleteItem()
{
  qDebug() << "delete";
}



void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    //QMouseEvent *mouseEvent = QGuiApplication::queryKeyboardModifiers();
    //if (mouseEvent->buttons() & Qt::RightButton)
    //{
    //    qDebug() << "Bylo kliknuto pravým tlačítkem";
    //}
}


void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    //ui->listView->addAction();
    QString clickedFilePath = filemodel->fileInfo(index).absoluteFilePath();
    QDir dir(clickedFilePath);
    if(!dir.exists())
    {
        if (QDesktopServices::openUrl(QUrl::fromLocalFile(clickedFilePath))) {
            qDebug() << "Aplikace byla spuštěna.";
        } else {
            QMessageBox::critical(this, "ERROR", "File doesnt exist.");
        }
    }

}


void MainWindow::on_listView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->listView->indexAt(pos);
    if(index.isValid())
    {
        QMenu contextMenu(this);

        QAction* action1 = contextMenu.addAction("Delete");
        QAction* action2 = contextMenu.addAction("Rename");
        QAction *selectedAction = contextMenu.exec(ui->listView->mapToGlobal(pos));


        QString clickedElementPath = filemodel->fileInfo(index).absoluteFilePath();
        QFileInfo fileInfo(clickedElementPath);
        QDir dir(clickedElementPath);
        QFile file(clickedElementPath);
        //Deletion
        if(selectedAction == action1)
        {
            if(fileInfo.exists())
            {
                QMessageBox::StandardButton answer = QMessageBox::question(this, "Deletion", "Do you really want to delete this element?",
                                                                           QMessageBox::Yes | QMessageBox::No);

                if(answer == QMessageBox::Yes)
                {
                    if(fileInfo.isDir())
                    {
                        qDebug() << "deleting folder";

                        dir.rmpath(".");
                    }
                    else if(fileInfo.isFile())
                    {
                        qDebug() << "deleting file";

                        file.remove();
                    }
                }
                else
                {
                    QMessageBox::critical(this, "ERROR", "Element does not exist anymore.");
                }
            }
        }

        //Rename
        else if(selectedAction == action2)
        {

            bool ok;

            QString userInput = QInputDialog::getText(
                nullptr,
                "Renaming",
                "New name:",
                QLineEdit::Normal,
                "",
                &ok
                );

            if (ok)
            {
                QString body = fileInfo.absolutePath();
                QString newFilePath = body + "/" + userInput;

                QString oldFolderPath = clickedElementPath;
                QString newFolderPath = body + "/" + userInput;
                if(fileInfo.isFile())
                {
                    file.rename(newFilePath);
                }
                else if(fileInfo.isDir())
                {
                    dir.rename(oldFolderPath, newFolderPath);
                }
            }
        }
    }
    else
    {
        QMenu contextMenu(this);
        QMenu subMenu("New file");
        QAction* action1 = contextMenu.addAction("New folder");
        contextMenu.addMenu(&subMenu);
        QAction* action1sub = subMenu.addAction(".txt");
        QAction* action2sub = subMenu.addAction(".cpp");
        QAction* action3sub = subMenu.addAction(".rar");
        QAction* customSub = subMenu.addAction("Custom file");

        QAction *selectedAction = contextMenu.exec(ui->listView->mapToGlobal(pos));
        bool ok;
        QModelIndex index = ui->listView->rootIndex();
        QString currentFolderPath = filemodel->filePath(index);
        if(selectedAction)
        {
            if(selectedAction == action1sub || selectedAction == action2sub || selectedAction == action3sub)
            {
                QString selectedText = selectedAction->text();
                QString userInput = QInputDialog::getText(
                    nullptr,
                    "New" + selectedText + "file",
                    "Name:",
                    QLineEdit::Normal,
                    "",
                    &ok
                    );

                if(ok)
                {
                    QString filePath = currentFolderPath + "/" + userInput + selectedText;
                    QFile file(filePath);
                    if (!file.exists()) {
                        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                            qDebug() << "File created successfully.";
                            qDebug() << filePath;
                        } else {
                            QMessageBox::critical(this, "ERROR", "Couldnt create file.");;
                        }
                    } else {
                        QMessageBox::critical(this, "ERROR", "File already exist.");
                    }
                }
            }
            else if(selectedAction == customSub)
            {

                QString userInput = QInputDialog::getText(
                    nullptr,
                    "New file",
                    "Name with extension(.txt, .docx,..)",
                    QLineEdit::Normal,
                    "",
                    &ok
                    );

                if(ok)
                {
                    QString filePath = currentFolderPath + "/" + userInput;
                    QFile file(filePath);
                    if (!file.exists()) {
                        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                            qDebug() << "File created successfully.";
                            qDebug() << filePath;
                        } else {
                            QMessageBox::critical(this, "ERROR", "Couldnt create file.");;
                        }
                    } else {
                        QMessageBox::critical(this, "ERROR", "File already exist.");
                    }
                }
            }
            else if(selectedAction == action1)
            {
                QString userInput = QInputDialog::getText(
                    nullptr,
                    "New folder",
                    "Name:",
                    QLineEdit::Normal,
                    "",
                    &ok
                    );

                if(ok)
                {
                    QDir dir(currentFolderPath + userInput);
                    if (!dir.exists()) {
                        if (dir.mkdir(currentFolderPath + "/" +  userInput)) {
                            qDebug() << "Folder created successfully.";
                            qDebug() << currentFolderPath + "/" +  userInput;
                        } else {
                            QMessageBox::critical(this, "ERROR", "Couldnt create folder.");;
                        }
                    } else {
                        QMessageBox::critical(this, "ERROR", "Folder already exist.");
                    }
                }
            }
        }
    }

}

