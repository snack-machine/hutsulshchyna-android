
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QFIle>

class CoordHandler;

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(CoordHandler* coordHandler, QObject* parent = nullptr);

    Q_INVOKABLE void processFile(const QString&, QString, const QString&);

Q_SIGNALS:
    void error(const QString& message);
    void success(const QString& message);

private:
    void addToExistingFile(QFile*, QString&, const QString&);
    void addToNewFile(QFile*, QString&, const QString&);

    QString formattedDateTime;
    CoordHandler* m_coordHandler;
};

#endif // FILEHANDLER_H
