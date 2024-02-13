// Copyright 2024 MATF studenti
#include "../include/notes.h"
#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../dependencies/qsourcehighliter.h"

Notes::Notes(Ui::MainWindow* mw)
    :m_mw(mw) {
}

void Notes::openFile(QString fileName, Ui::MainWindow *ui, QWidget *parent) {
    QFile file(fileName);
    m_currentFile = fileName;

    if (!file.open(QIODevice::ReadOnly | QFile::Text))
        return;

    parent->setWindowTitle(fileName.right(
        fileName.length() - fileName.lastIndexOf("/")-1));
    QTextStream in(&file);
    QString text = in.readAll();

    changeLanguage(fileName, ui);

    if (m_fileContentUnchanged)
        delete m_fileContentUnchanged;

    m_fileContentUnchanged = new QString(text);
    ui->textEdit->setText(text);
    file.close();
}

void Notes::newClicked(Ui::MainWindow *ui, QWidget *parent) {
    m_currentFile.clear();
    ui->textEdit->setText(QString());
    parent->setWindowTitle("Untitled Document");
}

void Notes::changeLanguage(QString fileName, Ui::MainWindow *ui) {
    std::map<QString,  QSourceHighlite::QSourceHighliter::Language>
        extToLanguageMap = {
        { ".cpp",  QSourceHighlite::QSourceHighliter::Language::CodeCpp },
        { ".hpp", QSourceHighlite::QSourceHighliter::Language::CodeCpp },
        { ".js", QSourceHighlite::QSourceHighliter::Language::CodeJs },
        { ".c", QSourceHighlite::QSourceHighliter::Language::CodeC },
        { ".h", QSourceHighlite::QSourceHighliter::Language::CodeC },
        { ".php", QSourceHighlite::QSourceHighliter::Language::CodePHP },
        { ".qml", QSourceHighlite::QSourceHighliter::Language::CodeQML },
        { ".py", QSourceHighlite::QSourceHighliter::Language::CodePython },
        { ".rs", QSourceHighlite::QSourceHighliter::Language::CodeRust },
        { ".java", QSourceHighlite::QSourceHighliter::Language::CodeJava },
        { ".class", QSourceHighlite::QSourceHighliter::Language::CodeJava},
        { ".cs", QSourceHighlite::QSourceHighliter::Language::CodeCSharp },
        { ".go", QSourceHighlite::QSourceHighliter::Language::CodeGo },
        { ".sql", QSourceHighlite::QSourceHighliter::Language::CodeSQL },
        { ".json", QSourceHighlite::QSourceHighliter::Language::CodeJSON },
        { ".xml", QSourceHighlite::QSourceHighliter::Language::CodeXML },
        { ".html", QSourceHighlite::QSourceHighliter::Language::CodeXML },
        { ".css", QSourceHighlite::QSourceHighliter::Language::CodeCSS },
        { ".ts", QSourceHighlite::QSourceHighliter::Language::CodeTypeScript },
        { ".yaml", QSourceHighlite::QSourceHighliter::Language::CodeYAML },
        { ".ini", QSourceHighlite::QSourceHighliter::Language::CodeINI },
        { ".vex", QSourceHighlite::QSourceHighliter::Language::CodeVex },
        { ".s", QSourceHighlite::QSourceHighliter::Language::CodeAsm },
        { ".lua", QSourceHighlite::QSourceHighliter::Language::CodeLua },
    };

    QString extension = fileName.right(fileName.length()
                                       - fileName.lastIndexOf("."));

    auto it = extToLanguageMap.find(extension);
    if (it != extToLanguageMap.end()) {
        if (!m_highliter)
            m_highliter = new QSourceHighlite::QSourceHighliter(
                                        m_mw->textEdit->document());
        m_highliter->setCurrentLanguage(it->second);
    } else if (fileName.startsWith("#! /usr/bin/python")) {
        m_highliter->setCurrentLanguage(
            QSourceHighlite::QSourceHighliter::Language::CodePython);
    } else if (fileName.startsWith("#! /bin/bash")) {
        m_highliter->setCurrentLanguage(
            QSourceHighlite::QSourceHighliter::Language::CodeBash);
    } else if (m_highliter) {
        qDebug() << "1";
        m_defaultFormat.setForeground(Qt::black);  // Adjust color as needed
        ui->textEdit->setCurrentCharFormat(m_defaultFormat);
        qDebug() << "2";
    }
}

void Notes::openClicked(Ui::MainWindow *ui, QWidget *parent) {
    QString fileName = QFileDialog::getOpenFileName(
        parent,
        "Otvori novu datoteku",
        Config::getConfig()->getHubPath());

    // Bez provere je izbacivao gresku kada se korisnik predomisli
    // i zatvori prozor za odabir datoteke
    if (QString::compare(fileName, "")) {
        openFile(fileName, ui, parent);
    }
}

void Notes::saveClicked(Ui::MainWindow *ui, QWidget *parent) {
    if (m_currentFile.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(
            parent,
            "Sacuvaj.",
            Config::getConfig()->getHubPath());
        QFile file(fileName);

        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            return;
        }

        m_currentFile = fileName;
        parent->setWindowTitle(
            fileName.right(fileName.length() - fileName.lastIndexOf("/")-1));

        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;

        file.close();
    } else {
        QFile file(m_currentFile);

        if (!file.open(QFile::WriteOnly | QFile::Text))
            return;

        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        if (m_fileContentUnchanged)
            delete m_fileContentUnchanged;

        m_fileContentUnchanged = new QString(text);
        file.close();
    }
    parent->setWindowTitle(
        m_currentFile.right(
            m_currentFile.length() - m_currentFile.lastIndexOf("/")-1));
    changeLanguage(m_currentFile, ui);
}

void Notes::copyClicked(Ui::MainWindow *ui) {
    ui->textEdit->copy();
}
void Notes::pasteClicked(Ui::MainWindow *ui) {
    ui->textEdit->paste();
}
void Notes::cutClicked(Ui::MainWindow *ui) {
    ui->textEdit->cut();
}
void Notes::undoClicked(Ui::MainWindow *ui) {
    ui->textEdit->undo();
}
void Notes::redoClicked(Ui::MainWindow *ui) {
    ui->textEdit->redo();
}
void Notes::notesContentChanged(QWidget* parent, Ui::MainWindow* ui) {
    if (QString::compare(m_currentFile, "")) {
        if (QString::compare(
                ui->textEdit->toPlainText(),
                *m_fileContentUnchanged)) {
           parent->setWindowTitle(
                m_currentFile.right(
                    m_currentFile.length()
                    - m_currentFile.lastIndexOf("/")-1) + "*");
        } else {
            parent->setWindowTitle(m_currentFile.right(
                m_currentFile.length() - m_currentFile.lastIndexOf("/")-1));
        }
    }
}
