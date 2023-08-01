
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>

class CoordHandler;

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(CoordHandler* coordHandler, QObject* parent = nullptr);

    Q_INVOKABLE void processFile(const QString&, const QString&, const QString&);

private:
    CoordHandler* m_coordHandler;
};

#endif // FILEHANDLER_H
