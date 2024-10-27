#include "core/markdown_processor.hpp"

#include <QRegularExpression>

using namespace mdflash::Core;

QString MarkdownProcessor::processHeader(const QString& line) noexcept {
	if (line.startsWith("# ")) {
		QString content = line.mid(2).trimmed();
		return QString("<h1 style='font-size: 2em; margin-bottom: 1px; font-weight: 600;'>%1</h1>"
					   "<hr style='height: 0.25em; padding: 0; margin: 0 0 10px 0; background-color: #e1e4e8; border: 0;'>")
				.arg(content);
	} else if (line.startsWith("## ")) {
		QString content = line.mid(3).trimmed();
		return QString("<div style='margin-top: 1.5em;'></div>" // Added extra space before level 2 headers
					   "<h2 style='font-size: 1.5em; margin-bottom: 1px; font-weight: 600;'>%1</h2>"
					   "<hr style='height: 0.1em; padding: 0; margin: 0 0 10px 0; background-color: #e1e4e8; border: 0;'>")
				.arg(content);
	} else if (line.startsWith("### ")) {
		QString content = line.mid(4).trimmed();
		return QString("<h3 style='font-size: 1.25em; margin-bottom: 10px; font-weight: 600;'>%1</h3>")
				.arg(content);
	}
	return line;
}

QString MarkdownProcessor::processContent(const QString& content) noexcept {
	QStringList lines = content.split("\n");
	QString processedContent;
	bool inCodeBlock = false;
	bool inList = false;
	bool inNumberedList = false;
	QStringList codeBlockLines;
	QStringList listItems;
	int listNumber = 1;

	for (const QString& line : lines) {
		if (line.trimmed().startsWith("```")) {
			if (inCodeBlock) {
				processedContent += processCodeBlock(codeBlockLines);
				codeBlockLines.clear();
				inCodeBlock = false;
			} else {
				if (inList) {
					processedContent += processList(listItems);
					listItems.clear();
					inList = false;
				}
				if (inNumberedList) {
					processedContent += processNumberedList(listItems);
					listItems.clear();
					inNumberedList = false;
					listNumber = 1;
				}
				inCodeBlock = true;
			}
		} else if (inCodeBlock) {
			codeBlockLines << line;
		} else if (line.trimmed().startsWith("- ")) {
			if (!inList) {
				inList = true;
				listItems.clear();
			}
			listItems << line.trimmed().mid(2);
		} else if (QRegularExpression(R"(^\d+\.\s)").match(line).hasMatch()) {
			if (!inNumberedList) {
				inNumberedList = true;
				listItems.clear();
				listNumber = 1;
			}
			listItems << line.trimmed().mid(line.indexOf(" ") + 1);
			listNumber++;
		} else {
			if (inList) {
				processedContent += processList(listItems);
				listItems.clear();
				inList = false;
			}
			if (inNumberedList) {
				processedContent += processNumberedList(listItems);
				listItems.clear();
				inNumberedList = false;
				listNumber = 1;
			}
			QString processedLine = processLine(line);
			processedLine = processBoldText(processedLine);
			processedLine = processItalicText(processedLine);
			processedLine = processInlineCode(processedLine);
			processedLine = processGitHubLinks(processedLine);
			processedContent += processedLine;
		}
	}

	if (inCodeBlock) {
		processedContent += processCodeBlock(codeBlockLines);
	} else if (inList) {
		processedContent += processList(listItems);
	} else if (inNumberedList) {
		processedContent += processNumberedList(listItems);
	}

	return processedContent;
}

QString MarkdownProcessor::processInlineCode(const QString& text) noexcept {
	QRegularExpression inlineCodeRegex("`([^`\n]+)`");
	QString processed = text;
	auto matches = inlineCodeRegex.globalMatch(processed);
	while (matches.hasNext()) {
		auto match = matches.next();
		QString codeText = match.captured(1);
		processed.replace(match.captured(0),
						  QString("<code style='background-color: rgba(27,31,35,0.05); padding: 0.2em 0.4em; "
								  "border-radius: 3px; font-family: monospace; font-size: 85%;'>%1</code>")
						  .arg(codeText.toHtmlEscaped()));
	}
	return processed;
}

QString MarkdownProcessor::processCodeBlock(const QStringList& lines) noexcept {
	QString codeContent = lines.join("\n").toHtmlEscaped();
	return QString("<pre style='background-color: #f6f8fa; border-radius: 6px; padding: 16px; overflow: auto;'>"
				   "<code style='font-family: SFMono-Regular,Consolas,Liberation Mono,Menlo,monospace; font-size: 85%;'>%1</code></pre>")
			.arg(codeContent);
}

QString MarkdownProcessor::processBoldText(const QString& text) noexcept {
	QRegularExpression boldRegex(R"(\*\*(.*?)\*\*)");
	QString processed = text;
	auto matches = boldRegex.globalMatch(processed);
	while (matches.hasNext()) {
		auto match = matches.next();
		QString boldText = match.captured(1);
		processed.replace(match.captured(0), QString("<strong style='font-weight: 600;'>%1</strong>").arg(boldText));
	}
	return processed;
}

QString MarkdownProcessor::processItalicText(const QString& text) noexcept {
	QRegularExpression italicRegex("\\*(.*?)\\*");
	QString processed = text;
	auto matches = italicRegex.globalMatch(processed);
	while (matches.hasNext()) {
		auto match = matches.next();
		QString italicText = match.captured(1);
		processed.replace(match.captured(0), QString("<em style='font-style: italic;'>%1</em>").arg(italicText));
	}
	return processed;
}

QString MarkdownProcessor::processLine(const QString& line) noexcept {
	QString processedLine = processHeader(line);
	if (processedLine == line && !line.trimmed().isEmpty()) {
		return QString("<p style='margin-top: 0; margin-bottom: 16px;'>%1</p>").arg(line);
	}
	return processedLine;
}

QString MarkdownProcessor::processList(const QStringList& items) noexcept {
	QString listHtml = "<ul style='padding-left: 2em; margin-top: 0; margin-bottom: 16px;'>";
	for (const QString& item : items) {
		QString processedItem = processBoldText(item);
		processedItem = processItalicText(processedItem);
		processedItem = processInlineCode(processedItem);
		processedItem = processGitHubLinks(processedItem);
		listHtml += QString("<li style='margin-top: 0.25em;'>%1</li>").arg(processedItem);
	}
	listHtml += "</ul>";
	return listHtml;
}

QString MarkdownProcessor::processNumberedList(const QStringList& items) noexcept {
	QString listHtml = "<ol style='padding-left: 2em; margin-top: 0; margin-bottom: 16px;'>";
	for (const QString& item : items) {
		QString processedItem = processBoldText(item);
		processedItem = processItalicText(processedItem);
		processedItem = processInlineCode(processedItem);
		processedItem = processGitHubLinks(processedItem);
		listHtml += QString("<li style='margin-top: 0.25em;'>%1</li>").arg(processedItem);
	}
	listHtml += "</ol>";
	return listHtml;
}

QString MarkdownProcessor::processGitHubLinks(const QString& text) noexcept {
	QRegularExpression linkRegex(R"(\[([^\]]+)\]\(([^\)]+)\))");
	QString processed = text;
	auto matches = linkRegex.globalMatch(processed);
	while (matches.hasNext()) {
		auto match = matches.next();
		QString linkText = match.captured(1);
		QString linkUrl = match.captured(2);
		processed.replace(match.captured(0),
						  QString("<a href='%1' style='color: #0366d6; text-decoration: none;'>"
								  "<span style='vertical-align: middle;'>%2</span></a>")
						  .arg(linkUrl, linkText));
	}
	return processed;
}
