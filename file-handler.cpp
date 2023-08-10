#include "file-handler.h"
#include "coord-handler.h"

//#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringView>
#include <QUrl>
#include <QLatin1StringView>
#include <QDateTime>
#include <QFileInfo>
#include <QQmlFile>

//#include <QDebug>

FileHandler::FileHandler(CoordHandler* coordHandler, QObject* parent)
    : QObject{parent}, m_coordHandler{coordHandler}, formattedDateTime{}
{
}

void FileHandler::processFile(const QString& filePath, QString pointName, const QString& pointDescription)
{
    if (m_coordHandler->getAverageLong().isEmpty() || m_coordHandler->getAverageLat().isEmpty()) {
        emit error(tr("Coordinates not found"));
//        qDebug() << "Not found average coords";
        return;
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    formattedDateTime = currentDateTime.toString("yyyy-MM-dd_hh-mm-ss");

    if (pointName.isEmpty()) {
        pointName = formattedDateTime;
    }
#ifdef PLATFORM_ANDROID
    QFile file(filePath);
#else
    QFile file(QUrl(filePath).toLocalFile());
#endif
    if (file.exists()) {
        addToExistingFile(&file, pointName, pointDescription);
    } else {
        addToNewFile(&file, pointName, pointDescription);
    }
}

void FileHandler::addToExistingFile(QFile* file, QString& pointName, const QString& pointDescription)
{
    if (!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
//        qDebug() << "Failed to open file";
        emit error(tr("Failed to open file"));
        return;
    }
    QString xmlData = file->readAll();
    file->close();

    if (xmlData.isEmpty()) {
        file->close();
        addToNewFile(file, pointName, pointDescription);
        return;
    }
    int pos = xmlData.indexOf("</Document>");
    if (pos == -1) {
        emit error(tr("Invalid file format\nTry to write to a new file"));
        return;
    }

    xmlData.insert(pos,
        "<Placemark>\n"
        "<name>" + pointName + "</name>\n"

        "<description><![CDATA[<!-- desc_gen:start -->\n"
        "<font color=\"black\"><table width=\"100%\"><tr><td width=\"100%\" align=\"center\">\n"
        "<!-- desc_user:start -->\n" + \
        pointDescription + \
        "\n<!-- desc_user:end -->\n"
        "</td></tr><tr><td><table width=\"100%\"></table></td></tr></table></font>\n"
        "<!-- desc_gen:end --]]></description>\n"

        "<styleUrl>#media-photo.png</styleUrl>\n"
        "<Point>\n"
        "<coordinates>" + m_coordHandler->getAverageLong() + ',' + m_coordHandler->getAverageLat() + "</coordinates>\n"
        "</Point>\n"
        "<gx:TimeStamp>\n"
        "<when>" + formattedDateTime + "</when>\n"
        "</gx:TimeStamp>\n"
        "</Placemark>\n"
    );

    if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit error(tr("Failed to open file"));
//        qDebug() << "Failed to open file for writing";
        return;
    }

    QTextStream out(file);
    out << xmlData;

    file->close();
    emit success(tr("Point added successfully"));
//    qDebug() << "Elements successfully inserted!";
}

void FileHandler::addToNewFile(QFile* file, QString& pointName, const QString& pointDescription)
{
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit error(tr("Failed to open file"));
//        qDebug() << "Failed to open file";
        return;
    }

    QFileInfo fileInfo(file->fileName());

    QXmlStreamWriter xmlWriter(file);
    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("kml");
    xmlWriter.writeNamespace("http://www.opengis.net/kml/2.2", "kml");
    xmlWriter.writeNamespace("http://www.google.com/kml/ext/2.2", "gx");
    xmlWriter.writeNamespace("http://www.w3.org/2005/Atom", "atom");

    xmlWriter.writeStartElement("Document");
    xmlWriter.writeTextElement("name", fileInfo.fileName());
    xmlWriter.writeStartElement("atom:author");
    xmlWriter.writeTextElement("atom:name", "Sigma");
    xmlWriter.writeEndElement(); // atom:author

//    xmlWriter.writeStartElement("Style");
//    xmlWriter.writeAttribute("id", "media-photo.png");
//    xmlWriter.writeStartElement("IconStyle");
//    xmlWriter.writeStartElement("Icon");
//    xmlWriter.writeTextElement("href", "files/media-photo.png");
//    xmlWriter.writeEndElement(); // Icon
//    xmlWriter.writeEmptyElement("hotSpot");
//    xmlWriter.writeAttribute("x", "0.5");
//    xmlWriter.writeAttribute("y", "0.0");
//    xmlWriter.writeAttribute("xunits", "fraction");
//    xmlWriter.writeAttribute("yunits", "fraction");
//    xmlWriter.writeEndElement(); // IconStyle
//    xmlWriter.writeEndElement(); // Style

    xmlWriter.writeStartElement("Placemark");
    xmlWriter.writeTextElement("name", pointName);
    xmlWriter.writeStartElement("description");
    xmlWriter.writeCDATA("<!-- desc_gen:start -->\n"
                         "<font color=\"black\"><table width=\"100%\"><tr><td width=\"100%\" align=\"center\">\n"
                         "<!-- desc_user:start -->\n" + \
                         pointDescription +\
                         "<!-- desc_user:end -->\n"
                         "</td></tr><tr><td><table width=\"100%\"></table></td></tr></table></font>\n"
                         "<!-- desc_gen:end -->");
    xmlWriter.writeEndElement(); // description
//    xmlWriter.writeTextElement("styleUrl", "#media-photo.png");
    xmlWriter.writeStartElement("Point");
    xmlWriter.writeTextElement("coordinates", m_coordHandler->getAverageLong() + ',' + m_coordHandler->getAverageLat());
    xmlWriter.writeEndElement(); // Point
    xmlWriter.writeStartElement("gx:TimeStamp");
    xmlWriter.writeTextElement("when", formattedDateTime);
    xmlWriter.writeEndElement(); // gx:TimeStamp
    xmlWriter.writeEndElement(); // Placemark

    xmlWriter.writeEndElement(); // Document
    xmlWriter.writeEndElement(); // kml
    xmlWriter.writeEndDocument();

    file->close();
    emit success(tr("File created successfully"));
//    qDebug() << "File successfully created!";
}
