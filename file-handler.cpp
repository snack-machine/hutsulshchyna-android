#include "file-handler.h"
#include "coord-handler.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringView>
#include <QUrl>
#include <QLatin1StringView>

#include <QDebug>

FileHandler::FileHandler(CoordHandler* coordHandler, QObject* parent)
    : QObject{parent}, m_coordHandler{coordHandler}
{

}

void FileHandler::processFile(const QString& filePath, const QString& pointName, const QString& pointDescription)
{
//    qDebug() << "FileHandler received: " << filePath;
    qDebug() << "FileHandler received: " << QUrl(filePath).toLocalFile();;
    QString localFile(QUrl(filePath).toLocalFile());
    QFile file(localFile);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filePath;
        return;
    }

    QString xmlData = file.readAll(); // Read the entire file into a QString
    file.close();

    int pos = xmlData.indexOf("</Document>");
    if (pos == -1) {
        qDebug() << "Could not find <Document> element in the file.";
        return;
    }

    // Insert the new elements right after the <Document> element
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
//        "<coordinates>24.9156735,48.3604371</coordinates>\n"
        "<coordinates>" + m_coordHandler->getAverageLong() + ',' + m_coordHandler->getAverageLat() + "</coordinates>\n"
        "</Point>\n"
        "<gx:TimeStamp>\n"
        "<when>2023-07-25T09:13:00Z</when>\n"
        "</gx:TimeStamp>\n"
        "</Placemark>\n"
    );
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return;
    }

//    QXmlStreamWriter xmlWriter(&file);
//    xmlWriter.setAutoFormatting(true);
//    xmlWriter.writeCharacters(xmlData);

    QTextStream out(&file);
    out << xmlData;

    file.close();
    qDebug() << "Elements successfully inserted!";
}


//while (!xmlReader.atEnd() && !xmlReader.hasError()) {
//    if (xmlReader.isStartElement() && xmlReader.name() == "Document") {
//        // Found the <Document> element, start copying its content
//        xmlWriter.writeStartElement(xmlReader.name().toString());

//        // Copy the elements inside <Document> until we reach the closing </Document>
//        while (!xmlReader.atEnd() && !xmlReader.hasError()) {
//            xmlReader.readNext();
//            if (xmlReader.isEndElement() && xmlReader.name() == "Document") {
//                // Reached the closing </Document>, stop copying
//                xmlWriter.writeEndElement();
//                break;
//            } else {
//                // Write the current element to the output file
//                xmlWriter.writeCurrentToken(xmlReader);
//            }
//        }
//    } else {
//        // Write other elements directly to the output file
//        xmlWriter.writeCurrentToken(xmlReader);
//    }

//    // Move to the next element in the input XML
//    xmlReader.readNext();
//}

//// Now, add the new <Placemark> block
//xmlWriter.writeStartElement("Placemark");
//xmlWriter.writeTextElement("name", "2023-07-25 09:13:16");
//xmlWriter.writeStartElement("description");
//xmlWriter.writeCDATA("<!-- desc_gen:start -->\n"
//                     "<font color=\"black\"><table width=\"100%\"><tr><td width=\"100%\" align=\"center\">\n"
//                     "<!-- desc_user:start -->\n"
//                     "Орел\n"
//                     "<!-- desc_user:end -->\n"
//                     "</td></tr><tr><td><table width=\"100%\"></table></td></tr></table></font>\n"
//                     "<!-- desc_gen:end -->");
//xmlWriter.writeEndElement(); // description
//xmlWriter.writeTextElement("styleUrl", "#media-photo.png");
//xmlWriter.writeStartElement("Point");
//xmlWriter.writeTextElement("coordinates", "24.9158735,48.3602371");
//xmlWriter.writeEndElement(); // Point
//xmlWriter.writeStartElement("gx:TimeStamp");
//xmlWriter.writeTextElement("when", "2023-07-25T09:13:25Z");
//xmlWriter.writeEndElement(); // gx:TimeStamp
//xmlWriter.writeEndElement(); // Placemark

//// Truncate the file at the current position to remove any remaining content
//file.resize(file.pos());

//// Close the file
//file.close();
