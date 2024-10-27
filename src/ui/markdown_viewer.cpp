#include "ui/markdown_viewer.hpp"

#include "core/markdown_processor.hpp"

#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextEdit>

using namespace mdflash::Ui;

MarkdownViewer::MarkdownViewer() noexcept
	: currentFileName("") {
	QMainWindow::setWindowTitle("mdflash - Markdown Viewer");

	auto centralWidget = new QWidget;
	QMainWindow::setCentralWidget(centralWidget);

	auto mainLayout = new QVBoxLayout;
	centralWidget->setLayout(mainLayout);
	mainLayout->setContentsMargins(30, 30, 30, 30); // Increased margins around the content

	textEdit = new QTextEdit;
	textEdit->setReadOnly(true);
	textEdit->setFont(QFont("Arial", 20));
	textEdit->setFrameStyle(QFrame::NoFrame);
	mainLayout->addWidget(textEdit);
	createMenus();
	setupStyle();
	QMainWindow::resize(1920, 1080);
}

void MarkdownViewer::openFile(const QString& fileName) noexcept {
	if (fileName.isEmpty()) {
		return;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	QTextStream in(&file);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	in.setEncoding(QStringConverter::Utf8);
#elif QT_VERSION >= QT_VERSION_CHECK(5, 15, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	in.setCodec("UTF-8");
#else
#error "Qt version less than 5.15 (LTS) is incomatible"
#endif

	QString content = in.readAll();
	textEdit->setHtml(Core::MarkdownProcessor::processContent(content));
	file.close();

	currentFileName = fileName;
}

void MarkdownViewer::createMenus() noexcept {
	QMenu* fileMenu = menuBar()->addMenu("File");

	auto openAction = new QAction("Open");
	std::ignore = connect(openAction, &QAction::triggered, this, &MarkdownViewer::openFileDialog);
	fileMenu->addAction(openAction);

	auto exitAction = new QAction("Exit");
	std::ignore = connect(exitAction, &QAction::triggered, this, &QWidget::close);
	fileMenu->addAction(exitAction);
}

void MarkdownViewer::openFileDialog() noexcept {
	QString fileName = QFileDialog::getOpenFileName(this,
													"Open Markdown File",
													"",
													"Markdown Files (*.md);;All Files (*)");
	openFile(fileName);
}

void MarkdownViewer::setupStyle() noexcept {
	QMainWindow::setStyleSheet("QMainWindow {"
							   "    background-color: #ffffff;"
							   "}"
							   "QMenuBar {"
							   "    background-color: #f6f8fa;"
							   "    border-bottom: 1px solid #d0d7de;"
							   "}"
							   "QMenuBar::item {"
							   "    padding: 5px 10px;"
							   "    background-color: transparent;"
							   "}"
							   "QMenuBar::item:selected {"
							   "    background-color: #0366d6;"
							   "    color: white;"
							   "}"
							   "QTextEdit { "
							   "    background-color: white; "
							   "    color: #24292e; "
							   "    font-size: 20px; "
							   "}"
							   "QScrollBar:vertical {"
							   "    border: none;"
							   "    background: #F6F8FA;"
							   "    width: 5px;" // Reduced scrollbar width
							   "    margin: 0px 0px 0px 0px;"
							   "}"
							   "QScrollBar::handle:vertical {"
							   "    background: #e5eaef;" // Lighter color for better visibility
							   "    min-height: 20px;"
							   "    border-radius: 4px;"
							   "}"
							   "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
							   "    height: 0px;"
							   "}"
							   "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
							   "    background: none;"
							   "}"
							   "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
							   "    background: none;"
							   "}"
							  );
}
