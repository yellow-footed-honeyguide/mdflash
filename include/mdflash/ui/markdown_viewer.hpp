#ifndef MARKDOWN_VIEWER_HPP
#define MARKDOWN_VIEWER_HPP

#include <QMainWindow>

class QTextEdit;

namespace mdflash::Ui {
	/**
	 * @brief	Main window for the markdown viewer application
	 *
	 * @author	yellow-footed-honeyguide
	 *
	 * @since	0.6.0
	 */
	class MarkdownViewer
			: public QMainWindow {
	public:
		/**
		 * @brief    Ctor.
		 */
		MarkdownViewer() noexcept;

		/**
		 * @brief	Open and display the content of a markdown file
		 *
		 * @param	fileName Markdown file path.
		 */
		void openFile(const QString& fileName) noexcept;

	private:
		/**
		 * @brief	Create the application menu
		 */
		void createMenus() noexcept;

		/**
		 * @brief	Open a file dialog to select a markdown file
		 */
		void openFileDialog() noexcept;

		/**
		 * @brief	Set up the application's visual style
		 */
		void setupStyle() noexcept;

		QTextEdit* textEdit;

		QString currentFileName;
	};
}
#endif
