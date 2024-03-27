#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSauvegarder_triggered()
{

}


bool writeToJson(const QString & filename, const QJsonObject & obj)
{

}

bool loadFromJson(const QString & filename)
{

}

void MainWindow::saveFile(const QString & filename)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);

        ts << ui->textEdit->document()->toPlainText();
        file.close();

        ui->textEdit->setProperty("filename", filename);
        ui->textEdit->document()->setModified(false);

        QFileInfo fi(filename);
        QString shortFilename = fi.fileName();
        setWindowTitle(shortFilename);
        emit messageChanged(tr("Saved file : ") + shortFilename, 5000);
    }
    else
    {
        QMessageBox::critical(this, tr("Save failed"), tr("Could not save ") + filename);
    }
}

void MainWindow::saveModified()
{
    if (ui->textEdit->document()->isModified())
    {
        QVariant fileNameProperty = ui->textEdit->property("filename");
        QString filename;
        if (fileNameProperty.isValid())
        {
            filename = fileNameProperty.toString();
        }

        if (filename.isEmpty())
        {
            filename = QFileDialog::getSaveFileName(this, tr("Save file ?"), QDir::currentPath(), "Text Files (*.txt)");
        }

        if (!filename.isEmpty())
        {
            QFileInfo fi(filename);
            int button = QMessageBox::question(this, tr("Save modified ?"), tr("Save file ") + fi.fileName());
            if (button == QMessageBox::Yes)
            {
                saveFile(filename);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Empty file name"), tr("Can't save empty file name"));
        }
    }
}
