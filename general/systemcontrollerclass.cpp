#include <string>
#include <vector>
#include <regex>

#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTime>

#include "general/systemcontrollerclass.h"
#include "additional/hwdescriptionparser.h"
#include "julabowrapper.h"

using namespace std;

SystemControllerClass::SystemControllerClass()
{
    fDaqControl = new DAQControlClass();
    fDatabase = new DatabaseInterfaceClass();
    fConnectRasp = NULL;
}

SystemControllerClass::~SystemControllerClass()
{}

bool SystemControllerClass::Initialize()
{

//    for(size_t i = 0 ; i != fNamesVoltageSources.size() ; i++){
//        getObject(fNamesVoltageSources[i])->initialize();
//        getObject(fNamesSources[i])->offPower(0);
//    }
    //if(fConnectRasp != NULL);
        //fConnectRasp->raspInitialize();

    for(auto &i: fGenericInstrumentMap){
        i.second->initialize();
    }

}
vector<QString>* SystemControllerClass::readFile()
{
    vector<QString> *cVec = new vector<QString>();
    QString cFilter = "*.txt";
    QString cFileName = QFileDialog::getOpenFileName( nullptr , "Open a file" , "/home/" , cFilter);
    QFile cFile(cFileName);
    cFile.open(QFile::ReadOnly);
    QTextStream cStream(&cFile);
    while(!cStream.atEnd()){
        QString cStr = cStream.readLine();
        cVec->push_back(cStr);
    }
    cFile.flush();
    cFile.close();

    return cVec;
}

void SystemControllerClass::Wait(int pSec)
{
    QTime cDieTime= QTime::currentTime().addSecs(pSec);
       while (QTime::currentTime() < cDieTime){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
       }
}

//
void SystemControllerClass::startDoingList()
{
    QThread *cThread = new QThread();
    for(vector<fParameters>::iterator cIter= fListOfCommands.begin() ; cIter != fListOfCommands.end() ; ++cIter){
        string cStr = (*cIter).cName;

        double cValue = (*cIter).cValue;
        if(cStr == "Set Temperature (°C)"){
           // moveToThread(cThread);
            connect(cThread, &QThread::started, [this, cValue]
            {this->setTemperature(cValue);});
            cThread->start();
        }
        if(cStr == "Wait (Sec)"){
            connect(cThread, &QThread::started, [this, cValue]
            {this->Wait(cValue);});
            cThread->start();
        }
        for(size_t i = 0 ; i != fNamesVoltageSources.size() ; i++){
            if(cStr == "On  " + fNamesVoltageSources[i] + "  power supply"){
                connect(cThread, &QThread::started, [this, i]
                {this->onPower(fNamesVoltageSources[i]);});
                cThread->start();
                getObject(fNamesVoltageSources[i])->onPower(1);
                emit sendOnOff(fNamesVoltageSources[i] , true);
            }
            if(cStr == "Off  " + fNamesVoltageSources[i] + "  power supply"){
                connect(cThread, &QThread::started, [this, i]
                {this->offPower(fNamesVoltageSources[i]);});
                cThread->start();
                getObject(fNamesVoltageSources[i])->offPower(1);
                emit sendOnOff(fNamesVoltageSources[i] , false);
            }
        }
    }
}

void SystemControllerClass::setTemperature(double pTemp)
{
//    getGenericInstrObj("JulaboFP50")->SetWorkingTemperature(pTemp);

}

void SystemControllerClass::wait(double pTime)
{
    QThread::sleep(pTime);
}

void SystemControllerClass::onPower(string pSourceName)
{
    getObject(pSourceName)->onPower(0);
}

void SystemControllerClass::offPower(string pSourceName)
{
    getObject(pSourceName)->offPower(0);
}


//reads file and makes map with name and object of power supply
void SystemControllerClass::ParseVSources()
{
    string cConnection , cSetVolt , cSetCurr , cAddress , cPort, cnOutputs;
    //vectors for id , voltage and current
    vector<string> cId;
    vector<string> cVolt;
    vector<string> cCurr;

    for(size_t i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].typeOfClass == "LowVoltageSource"){

            if(fHWDescription[i].classOfInstr == "TTI"){
                cConnection = fHWDescription[i].interface_settings["connection"];
                cAddress = fHWDescription[i].interface_settings["address"];
                cPort = fHWDescription[i].interface_settings["port"];

                for(size_t j = 0 ; j != fHWDescription[i].operational_settings.size() ; j++){

                    cId.push_back(fHWDescription[i].operational_settings[j]["output_id"]);
                    cVolt.push_back(fHWDescription[i].operational_settings[j]["Voltage"]);
                    cCurr.push_back(fHWDescription[i].operational_settings[j]["CurrentLimit"]);

                }
                string cConnectStr = getTypeOfConnection(cConnection , cAddress , cPort);
                PowerControlClass *fPowerLow = new ControlTTiPower(cConnectStr , cId , cVolt, cCurr);
                fMapSources.insert(pair<string , PowerControlClass*>(fHWDescription[i].name , fPowerLow));
                fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fPowerLow));
                fNamesVoltageSources.push_back(fHWDescription[i].name);
                fNamesInstruments.push_back(fHWDescription[i].classOfInstr);
            }
        }

        if(fHWDescription[i].typeOfClass == "HighVoltageSource"){

            if(fHWDescription[i].classOfInstr == "Keithley2410"){
                cConnection = fHWDescription[i].interface_settings["connection"];
                cAddress = fHWDescription[i].interface_settings["address"];
                cPort = fHWDescription[i].interface_settings["port"];
                cSetVolt = fHWDescription[i].operational_settings[0]["Voltage"];
                cSetCurr = fHWDescription[i].operational_settings[0]["CurrentLimit"];
                string cConnectStr = getTypeOfConnection(cConnection , cAddress , cPort);
                PowerControlClass *fPowerHigh = new ControlKeithleyPower(cConnectStr, cSetVolt, cSetCurr);
                fMapSources.insert(pair<string , PowerControlClass*>(fHWDescription[i].name , fPowerHigh));
                fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fPowerHigh));
                fNamesVoltageSources.push_back(fHWDescription[i].name);
                fNamesInstruments.push_back(fHWDescription[i].classOfInstr);

            }
        }
    }
}

void SystemControllerClass::ParseRaspberry()
{
    string cConnection , cAddress , cPort;

    for(size_t i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].classOfInstr == "Raspberry"){
            cConnection = fHWDescription[i].interface_settings["connection"];
            cAddress = fHWDescription[i].interface_settings["address"];
            cPort = fHWDescription[i].interface_settings["port"];

            fConnectRasp = new ConnectionInterfaceClass(cAddress , cPort);
            fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fConnectRasp));
            fNamesInstruments.push_back(fHWDescription[i].name);

            for(size_t j = 0 ; j != fHWDescription[i].operational_settings.size() ; j++){
                fRaspberrySensorsNames.push_back(fHWDescription[i].operational_settings[j]["sensor"]);

            }
        }
    }
}

void SystemControllerClass::ParseChiller()
{
    string cAddress, cConnection;
    for(size_t i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].classOfInstr == "Chiller"){
            cAddress = fHWDescription[i].interface_settings["address"];
            cConnection = fHWDescription[i].interface_settings["connection"];

            GenericInstrumentClass* fJulaboWrapper = new JulaboWrapper(cAddress);
            fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fJulaboWrapper));
            fNamesInstruments.push_back(fHWDescription[i].classOfInstr);

        }
    }
}

GenericInstrumentClass* SystemControllerClass::getGenericInstrObj(string pStr)
{
    return fGenericInstrumentMap[pStr];
}

void SystemControllerClass::ReadXmlFile(std::string pFileName)
{
    HWDescriptionParser cParser;
    fHWDescription = cParser.ParseXML(pFileName);

    this->ParseVSources();
    this->ParseRaspberry();
    this->ParseChiller();

}

//gets the value of key pStr
PowerControlClass* SystemControllerClass::getObject(string pStr)
{
    return fMapSources[pStr];
}

vector<string> SystemControllerClass::getSourceNameVec()
{
    return fNamesVoltageSources;
}

//returns a string for connection to power supply
string SystemControllerClass::getTypeOfConnection(string pConnection, string pAddress , string pPort)
{
    string cStr;

    if(pConnection == "ethernet")
       cStr = "TCPIP::" + pAddress + "::" + pPort + "::SOCKET";

    if(pConnection == "rs232"){
        size_t cPos = pAddress.find('S');
        pAddress = pAddress[cPos+1];
        int cNum = stoi(pAddress);
        pAddress = to_string(cNum + 1);
        cStr = "ASRL" + pAddress + "::INSTR";
        }
    return cStr;
}

void SystemControllerClass::closeConneections()
{
    for(size_t i = 0 ; i != fNamesVoltageSources.size() ; i++){
        getObject(fNamesVoltageSources[i])->closeConnection();
    }
    fConnectRasp->closeConnection();
}

