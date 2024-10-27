#include <QApplication>

#include "ui/markdown_viewer.hpp"

// Main function: Initialize and run the application
int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	mdflash::Ui::MarkdownViewer viewer;
	viewer.show();

	// If a file path is provided as a command-line argument, open it
	if (argc > 1) {
		viewer.openFile(QString::fromLocal8Bit(argv[1]));
	}

	return app.exec();
}
