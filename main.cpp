#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMenuBar>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include "markdown_processor.h"

// MarkdownViewer class: Main window for the markdown viewer application
class MarkdownViewer : public QMainWindow {
public:
    MarkdownViewer() : currentFileName("") {
        setWindowTitle("mdflash - Markdown Viewer");

        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(30, 30, 30, 30);  // Increased margins around the content

        textEdit = new QTextEdit(this);
        textEdit->setReadOnly(true);
        textEdit->setFont(QFont("Arial", 20));
        textEdit->setFrameStyle(QFrame::NoFrame);
        textEdit->setStyleSheet(
            "QTextEdit { "
            "   background-color: white; "
            "   color: #24292e; "
            "   font-size: 20px; "
            "}"
            "QScrollBar:vertical {"
            "   border: none;"
            "   background: #F6F8FA;"
            "   width: 5px;"  // Reduced scrollbar width
            "   margin: 0px 0px 0px 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "   background: #e5eaef;"  // Lighter color for better visibility
            "   min-height: 20px;"
            "   border-radius: 4px;"
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "   height: 0px;"
            "}"
            "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
            "   background: none;"
            "}"
            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
            "   background: none;"
            "}"
        );
        mainLayout->addWidget(textEdit);
        createMenus();
        setupStyle();
        resize(1920, 1080);
    }

    // Open and display the content of a markdown file
    void openFile(const QString &fileName) {
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                in.setEncoding(QStringConverter::Utf8);
#elif QT_VERSION >= QT_VERSION_CHECK(5, 15, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                in.setCodec("UTF-8");
#else
#error "Qt version less than 5.15 (LTS) is incomatible"
#endif

                QString content = in.readAll();
                textEdit->setHtml(MarkdownProcessor::processContent(content));
                file.close();

                QFileInfo fileInfo(fileName);
                currentFileName = fileInfo.fileName();
            }
        }
    }

private:
    // Create the application menu
    void createMenus() {
        QMenu *fileMenu = menuBar()->addMenu("File");

        QAction *openAction = new QAction("Open", this);
        connect(openAction, &QAction::triggered, this, &MarkdownViewer::openFileDialog);
        fileMenu->addAction(openAction);

        QAction *exitAction = new QAction("Exit", this);
        connect(exitAction, &QAction::triggered, this, &QWidget::close);
        fileMenu->addAction(exitAction);
    }

    // Open a file dialog to select a markdown file
    void openFileDialog() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Markdown File", "", "Markdown Files (*.md);;All Files (*)");
        openFile(fileName);
    }

    // Set up the application's visual style
    void setupStyle() {
        setStyleSheet(
            "QMainWindow {"
            "   background-color: #ffffff;"
            "}"
            "QMenuBar {"
            "   background-color: #f6f8fa;"
            "   border-bottom: 1px solid #d0d7de;"
            "}"
            "QMenuBar::item {"
            "   padding: 5px 10px;"
            "   background-color: transparent;"
            "}"
            "QMenuBar::item:selected {"
            "   background-color: #0366d6;"
            "   color: white;"
            "}"
        );
    }

    QTextEdit *textEdit;
    QString currentFileName;
};

// Main function: Initialize and run the application
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MarkdownViewer viewer;
    viewer.show();

    // If a file path is provided as a command-line argument, open it
    if (argc > 1) {
        viewer.openFile(QString::fromLocal8Bit(argv[1]));
    }

    return app.exec();
}
