#include "filehelper.h"
#include <QDir>
#include <QIODevice>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#include <QStringConverter>
#else
#include <QRegExp>
#include <QTextCodec>
#endif
#include <iostream>

FileHelper::FileHelper() {}

QString FileHelper::readAllTxt(const QString &filepath)
{
    QFile file(filepath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        return data;
    }
    throw "READ FAILED";
}

QVariant FileHelper::readAllJson(const QString &filepath)
{
    QString data = FileHelper::readAllTxt(filepath);
    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
    return doc.toVariant();
}

QList<QStringList> FileHelper::readAllCsv(const QString &filepath)
{
    QList<QStringList> records;
    QFile file(filepath); // 打开文件
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList row = line.split(','); // 拆分行
            records.append(row);               // 一行数据存为一条
        }
        file.close();
    }
    return records;
}

QString FileHelper::joinPath(const QString &path1, const QString &path2)
{
    QString path = path1 + "/" + path2;
    /**
     * 使用正则表达式匹配拆分
     * eg: "folder1/folder2\\folder3/folder4"
     * out: ["folder1", "folder2", "folder3", "folder4"]
     */
    QStringList pathList;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    pathList = path.split(QRegularExpression("[/\\\\]"), Qt::SkipEmptyParts);
#else
    pathList = path.split(QRegExp("[/\\\\]"), QString::SkipEmptyParts);
#endif

    path = pathList.join("/"); // 将list中的字符串通过'/'拼接成一个

    // 压缩路径 "./local" ->"local"
    // "local/../bin" -> "bin"
    // "/local/usr/../bin" -> "/local/bin"
    return QDir::cleanPath(path);
}

bool FileHelper::mkPath(const QString &path)
{
    QDir dir;
    return dir.mkpath(path);
}

// 写入文件
void FileHelper::writeFile(const QStringList lines, const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        stream.setEncoding(QStringConverter::Utf8);
#else
        stream.setCodec("UTF-8");
#endif
        for (const auto &line : lines) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            stream << line << Qt::endl;
#else
            stream << line << endl;
#endif
        }
        file.close();
        return;
    }
    throw "写入文件失败";
}
