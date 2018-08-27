#ifndef HWDESCRIPTIONPARSER_H
#define HWDESCRIPTIONPARSER_H

#include <string>
#include <QXmlStreamReader>

struct GenericInstrumentDescription_t {
    std::string name;
    std::string type;
    std::string typeOfClass;
    std::string description;
    std::map<std::string, std::string> interface_settings;
    std::vector<std::map<std::string, std::string>> operational_settings;
};

class HWDescriptionParser
{
public:
    HWDescriptionParser();
    std::vector<GenericInstrumentDescription_t> ParseXML();
private:
    void ParsePower(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments);
    void ParseVoltageSource(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments);
    void ParseEnvironment(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments);
    void ParseChiller(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments);
    void ParsePeltier(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments);
    void ParseRaspberry(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments);

};

#endif // HWDESCRIPTIONPARSER_H