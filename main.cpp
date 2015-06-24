#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile * in_file = new QFile("input.xml");
    QFile * ou_file = new QFile("out.csv");
    QFile * fo_file = new QFile("fotolist.txt");

    if(!in_file->exists())
    {
        qDebug() << "ERROR: " << in_file->errorString();
        qDebug() << "ОШИБКА: нет фала данных input.xml, переименуйте файл прайса в input.xml и положите его рядом с файлом этой программы";
        return -1;
    }

    if(ou_file->exists())
        ou_file->remove();
    if(fo_file->exists())
        fo_file->rename("fotolist.txt", "fotolist_old.txt");


    if (!in_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "ERROR: " << in_file->errorString();
        qDebug() << "ОШИБКА: не могу открыть файл input.xml";
        return -2;
    }

    if(!ou_file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "ERROR: " << ou_file->errorString();
        qDebug() << "ОШИБКА: не могу открыть файл out.csv";
        return -3;
    }

    if(!fo_file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "ERROR: " << fo_file->errorString();
        qDebug() << "ОШИБКА: не могу открыть файл fotolist.txt";
        return -3;
    }

    QXmlStreamReader xmlRead(in_file);
    QTextStream out(ou_file);
    QTextStream foto(fo_file);

    while (!xmlRead.atEnd() && !xmlRead.hasError())
    {
        QXmlStreamReader::TokenType token = xmlRead.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xmlRead.name() == "japancarru_import_data")
                continue;
            if (xmlRead.name() == "data_list")
                continue;
            if (xmlRead.name() == "data")
            {
                xmlRead.readNext();
                while (!(xmlRead.tokenType() == QXmlStreamReader::EndElement && xmlRead.name() == "data"))
                {
                    if (xmlRead.tokenType() == QXmlStreamReader::StartElement)
                    {
                        if (xmlRead.name() == "partsname"  ||
                            xmlRead.name() == "firm"       ||
                            xmlRead.name() == "model"      ||
                            xmlRead.name() == "kuzov"      ||
                            xmlRead.name() == "engine"     ||
                            xmlRead.name() == "price"      ||
                            xmlRead.name() == "info"       ||
                            xmlRead.name() == "s_presence" ||
                            xmlRead.name() == "modelnumber"||
                            xmlRead.name() == "R_L"        ||
                            xmlRead.name() == "U_D"        ||
                            xmlRead.name() == "F_R"        ||
                            xmlRead.name() == "oem_code")
                        {
                            xmlRead.readNext();
                            out << xmlRead.text().toString() << ";";
                        }

                        if(xmlRead.name() == "photos_list")
                        {
                            xmlRead.readNext();
                            while (!(xmlRead.tokenType() == QXmlStreamReader::EndElement && xmlRead.name() == "photos_list"))
                            {
                                if (xmlRead.tokenType() == QXmlStreamReader::StartElement)
                                {
                                    if(xmlRead.name() == "photo_name")
                                    {
                                        xmlRead.readNext();
                                        QString foto_url = xmlRead.text().toString();
                                        foto_url.replace(" ", "");
                                        foto_url.replace("\n", "");
                                        foto_url.replace("\t", "");
                                        out << foto_url << " ";
                                        foto << foto_url << "\n";
                                    }
                                }
                                xmlRead.readNext();
                            }
                        }

                    }
                    xmlRead.readNext();
                }
                out << '\n';
            }
        }
    }
    in_file->close();
    ou_file->close();
    fo_file->close();
    qDebug() << "Done";
    return a.exec();
}
