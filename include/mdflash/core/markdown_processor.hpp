#ifndef MARKDOWN_PROCESSOR_HPP
#define MARKDOWN_PROCESSOR_HPP

#include <QString>

namespace mdflash::Core {
	/**
	 * @brief
	 *
	 * @author	yellow-footed-honeyguide
	 *
	 * @since	0.6.0
	 */
	class MarkdownProcessor {
	public:
		[[nodiscard]]
		static QString processHeader(const QString& line) noexcept;

		[[nodiscard]]
		static QString processLine(const QString& line) noexcept;

		[[nodiscard]]
		static QString processCodeBlock(const QStringList& lines) noexcept;

		[[nodiscard]]
		static QString processContent(const QString& content) noexcept;

		[[nodiscard]]
		static QString processList(const QStringList& items) noexcept;

		[[nodiscard]]
		static QString processBoldText(const QString& text) noexcept;

		[[nodiscard]]
		static QString processItalicText(const QString& text) noexcept;

		[[nodiscard]]
		static QString processNumberedList(const QStringList& items) noexcept;

		[[nodiscard]]
		static QString processInlineCode(const QString& text) noexcept;

		[[nodiscard]]
		static QString processGitHubLinks(const QString& text) noexcept;
	};
}

#endif
