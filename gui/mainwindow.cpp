#include <iostream>
#include <typeinfo>

#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QStandardItemModel>
#include <QTimer>
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additional/additionalthread.h"
#include "general/julabowrapper.h"

int nOutputs = 2;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fRowMax = 1;
    fRowClicked = 0;


    ui->setupUi(this);

//    fControl = new SystemControllerClass();

    model = new QStandardItemModel(this);

    QStandardItem *cItem1 = new QStandardItem("Added commands:");
    model->setItem( 0 , 0 , cItem1);
    model->index(0 , 0);
    QStandardItem *cItem2 = new QStandardItem("Start time:");
    model->setItem( 0 , 1 , cItem2);
    model->index(0 , 1);
    QStandardItem *cItem3 = new QStandardItem("End time:");
    model->setItem( 0 , 2 , cItem3);
    model->index(0 , 2);
    QStandardItem *cItem4 = new QStandardItem("Status:");
    model->setItem( 0 , 3 , cItem4);
    model->index(0 , 3);

    ui->AddedComands_tabelView->setModel(model);
    ui->AddedComands_tabelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->AddedComands_tabelView->horizontalHeader()->setStretchLastSection(true);
    ui->AddedComands_tabelView->resizeColumnsToContents();
    ui->AddedComands_tabelView->verticalHeader()->setVisible(false);
    ui->AddedComands_tabelView->horizontalHeader()->setVisible(false);
    ui->AddedComands_tabelView->setDragEnabled(true);
    ui->AddedComands_tabelView->setAcceptDrops(true);
    ui->AddedComands_tabelView->setDropIndicatorShown(true);

    // default disabled everything
    ui->tabWidget->setEnabled(false);
    ui->init_button->setEnabled(false);
    ui->Down_pushButton->setEnabled(false);
    ui->Up_pushButton->setEnabled(false);
    ui->Start_pushButton->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

output_pointer_t MainWindow::SetSourceOutputLayout(std::string pType)
{
    // create pointer list
    output_pointer_t cOutputPointers;

    // create layout
    cOutputPointers.layout = new QVBoxLayout;

    // type of the power source
    QLabel *type = new QLabel(pType.c_str());
    type->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(type);

    // i/v set
    cOutputPointers.i_set = new QDoubleSpinBox();
    cOutputPointers.i_set->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.i_set);
    cOutputPointers.v_set = new QDoubleSpinBox();
    cOutputPointers.v_set->setMaximumHeight(20);
    cOutputPointers.v_set->setMinimum(-100000);
    cOutputPointers.layout->addWidget(cOutputPointers.v_set);

    // applied
    cOutputPointers.i_applied = new QLCDNumber();
    cOutputPointers.i_applied->setMaximumHeight(20);
    cOutputPointers.i_applied->setSegmentStyle(QLCDNumber::Flat);
    cOutputPointers.layout->addWidget(cOutputPointers.i_applied);
    cOutputPointers.v_applied = new QLCDNumber();
    cOutputPointers.v_applied->setMaximumHeight(20);
    cOutputPointers.v_applied->setSegmentStyle(QLCDNumber::Flat);
    cOutputPointers.layout->addWidget(cOutputPointers.v_applied);

    // on off
    cOutputPointers.onoff_button = new QCheckBox("On/Off");
    cOutputPointers.onoff_button->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.onoff_button);

    // return
    return cOutputPointers;
}

output_Raspberry MainWindow::setRaspberryLayout(string pName)
{
    output_Raspberry cOutputRaspberry;

    cOutputRaspberry.layout = new QHBoxLayout;

    cOutputRaspberry.label = new QLabel(QString::fromStdString(pName));
    cOutputRaspberry.label->setMaximumHeight(20);
    cOutputRaspberry.value = new QLCDNumber();
    cOutputRaspberry.value->setMaximumHeight(20);
    cOutputRaspberry.layout->addWidget(cOutputRaspberry.label);
    cOutputRaspberry.value->setSegmentStyle(QLCDNumber::Flat);

    cOutputRaspberry.layout->addWidget(cOutputRaspberry.value);

    return cOutputRaspberry;
}

output_Chiller MainWindow::setChilerLayout(string pType)
{
    output_Chiller cOutputPointers;

    cOutputPointers.layout = new QVBoxLayout;


    cOutputPointers.setTemperature = new QDoubleSpinBox();
    cOutputPointers.setTemperature->setMaximumHeight(20);
    cOutputPointers.setTemperature->setMinimum(-100000);
    cOutputPointers.layout->addWidget(cOutputPointers.setTemperature);


    cOutputPointers.bathTemperature = new QLCDNumber();
    cOutputPointers.bathTemperature->setMaximumHeight(20);
    cOutputPointers.bathTemperature->setSegmentStyle(QLCDNumber::Flat);
    cOutputPointers.layout->addWidget(cOutputPointers.bathTemperature);

    cOutputPointers.workingTemperature = new QLCDNumber();
    cOutputPointers.workingTemperature->setMaximumHeight(20);
    cOutputPointers.workingTemperature->setSegmentStyle(QLCDNumber::Flat);
    cOutputPointers.layout->addWidget(cOutputPointers.workingTemperature);

    cOutputPointers.sensorTemperature = new QLCDNumber();
    cOutputPointers.sensorTemperature->setMaximumHeight(20);
    cOutputPointers.sensorTemperature->setSegmentStyle(QLCDNumber::Flat);
    cOutputPointers.layout->addWidget(cOutputPointers.sensorTemperature);

    cOutputPointers.pressure = new QLCDNumber();
    cOutputPointers.pressure->setMaximumHeight(20);
    cOutputPointers.pressure->setSegmentStyle(QLCDNumber::Flat);
    cOutputPointers.layout->addWidget(cOutputPointers.pressure);


    // on off
    cOutputPointers.onoff_button = new QCheckBox("On/Off");
    cOutputPointers.onoff_button->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.onoff_button);

    // add stretch
    //QSpacerItem *item = new QSpacerItem(5,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
    //cOutputPointers.layout->addItem(item);

    return cOutputPointers;
}

output_Chiller* MainWindow::SetChillerOutput(QLayout *pMainLayout, string pName)
{
    // output pointers
    output_Chiller *cOutputPointers = new output_Chiller[1];

    // horizontal layout
    QGroupBox *group_box = new QGroupBox(pName.c_str());
    QHBoxLayout *group_box_layout = new QHBoxLayout;

    QSize size(80,20);

    // set the labels
    QVBoxLayout *label_layout = new QVBoxLayout;
    label_layout->setMargin(10);


    QLabel *label_t_set = new QLabel("T(set), °C:");
    label_t_set->setMinimumSize(size);
    label_layout->addWidget(label_t_set);
    QLabel *label_t_bath = new QLabel("T(bath), °C:");
    label_t_bath->setMinimumSize(size);
    label_layout->addWidget(label_t_bath);
    QLabel *label_t_working = new QLabel("T(working), °C:");
    label_t_working->setMinimumSize(size);
    label_layout->addWidget(label_t_working);
    QLabel *label_t_sensor = new QLabel("T(sensor), °C:");
    label_t_sensor->setMinimumSize(size);
    label_layout->addWidget(label_t_sensor);
    QLabel *label_pressure = new QLabel("P , Pa");
    label_pressure->setMinimumSize(size);
    label_layout->addWidget(label_pressure);
    QLabel *label_on_off = new QLabel("On/Off");
    label_on_off->setMinimumSize(size);
    label_layout->addWidget(label_on_off);
    // add stretchconnect(fControl, SIGNAL(sendOnOff(string,bool)) , this , SLOT(receiveOnOff(string,bool)));
    label_layout->addSpacing(15);

    // now set
    group_box_layout->addItem(label_layout);

    cOutputPointers[0] = setChilerLayout(pName);
    group_box_layout->addItem(cOutputPointers[0].layout);

    group_box->setLayout(group_box_layout);

    // finally add to the main layout
    pMainLayout->addWidget(group_box);

    // return everything
    return cOutputPointers;
}

output_pointer_t* MainWindow::SetVoltageSource(QLayout *pMainLayout, std::string pName, std::string pType, int pNoutputs)
{
    // create the output pointers
    output_pointer_t* cOutputPointers = new output_pointer_t[pNoutputs];

    // horizontal layout
    QGroupBox *group_box = new QGroupBox(pName.c_str());
    QHBoxLayout *group_box_layout = new QHBoxLayout;

    QSize size(80,20);

    // set the labels
    QVBoxLayout *label_layout = new QVBoxLayout;
    QLabel *label_type = new QLabel("Type:");
    label_type->setMinimumSize(size);
    label_layout->addWidget(label_type);
    QLabel *label_i_set = new QLabel("I(set), A:");
    label_i_set->setMinimumSize(size);
    label_layout->addWidget(label_i_set);
    QLabel *label_v_set = new QLabel("V(set), V:");
    label_v_set->setMinimumSize(size);
    label_layout->addWidget(label_v_set);
    QLabel *label_i_applied = new QLabel("I(applied), A:");
    label_i_applied->setMinimumSize(size);
    label_layout->addWidget(label_i_applied);
    QLabel *label_v_applied = new QLabel("V(applied), V:");
    label_v_applied->setMinimumSize(size);
    label_layout->addWidget(label_v_applied);
    QLabel *label_onoff = new QLabel("On/Off:");
    label_onoff->setMinimumSize(size);
    label_layout->addWidget(label_onoff);
    // now set
    group_box_layout->addItem(label_layout);

    // set the outputs
    for (int i = 0; i < pNoutputs; i++) {
        cOutputPointers[i] = SetSourceOutputLayout(pType);
        group_box_layout->addItem(cOutputPointers[i].layout);
    }

    // set the group box
    group_box->setLayout(group_box_layout);

    // finally add to the main layout
    pMainLayout->addWidget(group_box);

    // return everything
    return cOutputPointers;
}

output_Raspberry* MainWindow::SetRaspberryOutput(QLayout *pMainLayout , vector<string> pNames , string pNameGroupBox)
{
    output_Raspberry* cOutputPointers = new output_Raspberry[pNames.size()];

    QGroupBox *group_box = new QGroupBox(QString::fromStdString(pNameGroupBox));

    QHBoxLayout *cLayout_group_box = new QHBoxLayout;

    QVBoxLayout *cLayout_raspberry = new QVBoxLayout;
    for(size_t i = 0 ; i < pNames.size() ; i++){
        cOutputPointers[i] = setRaspberryLayout(pNames[i]);
        cLayout_raspberry->addItem(cOutputPointers[i].layout);
        cLayout_raspberry->addSpacing(15);
    }
    //cLayout_raspberry->addStretch();
    QSpacerItem *item = new QSpacerItem(0 , 100 ,QSizePolicy::Expanding, QSizePolicy::Fixed);
    cLayout_raspberry->addItem(item);

    cLayout_group_box->addItem(cLayout_raspberry);

    group_box->setLayout(cLayout_group_box);

    pMainLayout->addWidget(group_box);

    return cOutputPointers;
}

void MainWindow::voltageControlWidget()
{
//    QStandardItemModel *model = new QStandardItemModel(this);

//    QStandardItem *cItem1 = new QStandardItem("Source name");
//    model->setItem( 0 , 0 , cItem1);
//    model->index(0 , 0);
//    QStandardItem *cItem2 = new QStandardItem("V_set");
//    model->setItem( 0 , 1 , cItem2);
//    model->index(0 , 1);
//    QStandardItem *cItem3 = new QStandardItem("I_set");
//    model->setItem( 0 , 2 , cItem3);
//    model->index(0 , 2);
//    QStandardItem *cItem4 = new QStandardItem("V_app");
//    model->setItem( 0 , 3 , cItem4);
//    model->index(0 , 3);
//    QStandardItem *cItem5 = new QStandardItem("I_app");
//    model->setItem( 0 , 4 , cItem5);
//    model->index(0 , 4);

//    int j = 1;
//    for(auto const &i: fControl->fMapSources){
//        QStandardItem *cIt = new QStandardItem(QString::fromStdString(i.first));
//        model->setItem(j , 0 , cIt);
//        model->index(j , 0);
//        j++;
//    }

//    ui->voltageTableView->setModel(model);

}
//creates a List with all commands
void MainWindow::doListOfCommands()
{
    size_t cVecSize = fSources.size();
    QStandardItemModel *cModel = new QStandardItemModel(this);

    QStandardItem *cItem1 = new QStandardItem("Set Temperature (°C)");
    cModel->setItem( 0 , cItem1);
    cModel->index( 0 , 0);

    QStandardItem *cItem2 = new QStandardItem("Wait (Sec)");
    cModel->setItem( 1 , cItem2);
    cModel->index( 1 , 0);

    for(int i = 2 ; i != fSources.size()+2 ; i++){
        QString cStr = QString::fromStdString(fSources[i-2]);
        QStandardItem *cItem3 = new QStandardItem("On  " + cStr + "  power supply");
        cModel->setItem(i, cItem3);
        cModel->index(i , 0);

    }
    for(int i = fSources.size() + 2 ; i != 2*fSources.size() + 2 ; i++){
        QString cStr = QString::fromStdString(fSources[i-fSources.size() - 2]);
        QStandardItem *cItem4 = new QStandardItem("Off  " + cStr + "  power supply");
        cModel->setItem(i, cItem4);
        cModel->index(i , 0);
    }

    ui->listOfCommands->setModel(cModel);
    ui->listOfCommands->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//creates a new thread to take info from TTi
void MainWindow::getVoltAndCurr()
{
    AdditionalThread *cThread  = new AdditionalThread("A", fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getVAC()));
    connect(cThread, SIGNAL(sendToThread(PowerControlClass::fVACvalues*)),this,
            SLOT(updateTTiIWidget(PowerControlClass::fVACvalues*)));
    cThread->moveToThread(cQThread);
    cQThread->start();

}


void MainWindow::getMeasurments()
{
    AdditionalThread *cThread = new AdditionalThread("B" , fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getRaspSensors()));
    connect(cThread, SIGNAL(sendToThreadString(QString)),this , SLOT(updateRaspWidget(QString)));
    cThread->moveToThread(cQThread);
    cQThread->start();
}

void MainWindow::getVoltAndCurrKeithley()
{
    AdditionalThread *cThread  = new AdditionalThread("C", fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getVACKeithley()));
    connect(cThread, SIGNAL(sendToThreadKeithley(PowerControlClass::fVACvalues*)),this,
            SLOT(updateKeithleyWidget(PowerControlClass::fVACvalues*)));
    cThread->moveToThread(cQThread);
    cQThread->start();
}

void MainWindow::getChillerStatus()
{
    AdditionalThread *cThread  = new AdditionalThread("C", fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getChillerStatus()));
    connect(cThread, SIGNAL(sendFromChiller(QString)),this,
            SLOT(updateChillerWidget(QString)));
    cThread->moveToThread(cQThread);
    cQThread->start();
}

//func which adds command to the list and vector from listOfCommands to AddedWidget
void MainWindow::on_listOfCommands_doubleClicked(const QModelIndex &pIndex)
{
    SystemControllerClass::fParameters cObj;
    QString cValue;
    QString cStr = pIndex.data().toString();
    //another window to set the value
    fAddWnd = new AdditionalWindow(this);
    if(cStr == "Set Temperature (°C)" || cStr == "Wait (Sec)"){
        fAddWnd->show();
        fAddWnd->exec();
        cValue = fAddWnd->getValue();
        cObj.cName = cStr.toStdString();
        cObj.cValue = cValue.toDouble();
        fControl->fListOfCommands.push_back(cObj);
        cStr = cStr + " =" + cValue;
    }
    //
    else{
        cObj.cName = cStr.toStdString();
        cObj.cValue = 0;
        fControl->fListOfCommands.push_back(cObj);
    }
    //addes object to the added commands
    QStandardItem *cItem = new QStandardItem(cStr);
    model->setItem(fRowMax , 0, cItem);
    model->index(fRowMax, 0);
    fRowMax++;
    ui->AddedComands_tabelView->resizeColumnsToContents();
    ui->Down_pushButton->setEnabled(true);
    ui->Up_pushButton->setEnabled(true);
}

//reads file and writes it to AddWidget, QString into name field of fParam and string after = to value field
void MainWindow::on_readConfig_push_button_clicked()
{
    SystemControllerClass::fParameters cObj;
    QString cStr;
    vector<QString> *cVec = new vector<QString>();

    cVec = fControl->readFile();
    for(vector<QString>::iterator iter = cVec->begin(); iter != cVec->end(); ++iter){
        cStr = (*iter);
        string cString = cStr.toStdString();
        size_t cPos = cString.find("=");
        if( cPos ){
            string cPosStr = cString.substr(cPos+1);
            QString cStrTemp = QString::fromStdString(cPosStr);
            cString = cString.substr(0 , cPos);
            cObj.cName = cString;
            cObj.cValue = cStrTemp.toDouble();
            fControl->fListOfCommands.push_back(cObj);
        }
        else{
            cObj.cName = cStr.toStdString();
            cObj.cValue = 0;
            fControl->fListOfCommands.push_back(cObj);
        }
    }
    for(vector<QString>::iterator iter = cVec->begin(); iter != cVec->end(); ++iter){
        cStr = (*iter);
        QStandardItem *cItem = new QStandardItem(cStr);
        model->setItem(fRowMax , 0, cItem);
        model->index(fRowMax , 0);
        fRowMax++;
        ui->AddedComands_tabelView->setModel(model);
        ui->AddedComands_tabelView->resizeColumnsToContents();
    }
}

//starts making list of commands
void MainWindow::on_Start_pushButton_clicked()
{
    fControl->startDoingList();
}

//reads sensor on rasp and sets info to Raspberry sensors
void MainWindow::updateRaspWidget(QString pStr)
{
    vector<string> cVec;
    string cStr = pStr.toStdString();

    std::istringstream ist(cStr);
    std::string tmp;
    while ( ist >> tmp )
       cVec.push_back(tmp);

    for(int i = 1 ; i != cVec.size() ; i++){
        gui_raspberry[i-1].value->display(QString::fromStdString(cVec[i]).toDouble());
    }
}

void MainWindow::updateChillerWidget(QString pStr)
{
    vector<QString> cVec;

    std::istringstream ist(pStr.toStdString());
    std::string tmp;

    while ( ist >> tmp )
        cVec.push_back(QString::fromStdString(tmp));

    gui_chiller->bathTemperature->display(cVec[0].toDouble());
    gui_chiller->pressure->display(cVec[1].toDouble());
    gui_chiller->sensorTemperature->display(cVec[2].toDouble());
    gui_chiller->workingTemperature->display(cVec[3].toDouble());
}

//deletes highlighted item from the table(double-click on item)
void MainWindow::on_AddedComands_tabelView_doubleClicked(const QModelIndex &pIndex)
{
    int cRow = pIndex.row();
    QMessageBox cMsgBox;
    cMsgBox.setText("Are you sure you want to delete this command?");
    cMsgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    cMsgBox.setIcon(QMessageBox::Warning);
    cMsgBox.setDefaultButton(QMessageBox::Ok);
    int res = cMsgBox.exec();
    if (res == QMessageBox::Ok)
        removeRow(cRow);
}

//removes row from the table with added commands
void MainWindow::removeRow(int pRow)
{
    ui->AddedComands_tabelView->model()->removeRow(pRow);
    vector<SystemControllerClass::fParameters>::iterator iter = fControl->fListOfCommands.begin();
    fControl->fListOfCommands.erase(iter + pRow - 1);
    fRowMax--;
}

void MainWindow::on_AddedComands_tabelView_clicked(const QModelIndex &pIndex)
{
    fIndex = pIndex;
}

//moves command up when up push button clicked
void MainWindow::on_Up_pushButton_clicked()
{
    //get an index of the row
    int cRowIndex = fIndex.row();

    if( cRowIndex > 1 ){
        //take text from clicked item
        QString cData = ui->AddedComands_tabelView->model()->data(fIndex).toString();

        //take text from item that above than clicked item
        QString cTempData = ui->AddedComands_tabelView->model()->data(ui->AddedComands_tabelView->
                                                                      model()->index(cRowIndex-1, 0)).toString();
        //make new items
        QStandardItem *cItem = new QStandardItem(cData);
        QStandardItem *cItemTemp = new QStandardItem(cTempData);

        //set items to the model
        model->setItem(cRowIndex-1 , cItem);
        model->index(cRowIndex-1 , 0);
        model->setItem(cRowIndex , cItemTemp);
        model->index(cRowIndex , 0);

        //set information from items to fObjParam struct(changes position of items in vector)
        SystemControllerClass::fParameters cObject;
        vector<SystemControllerClass::fParameters>::iterator cIter = fControl->fListOfCommands.begin() + cRowIndex -1;
        vector<SystemControllerClass::fParameters>::iterator cIterTemp = fControl->fListOfCommands.begin() +cRowIndex -2;

        cObject.cName = (*cIter).cName;
        cObject.cValue = (*cIter).cValue;

        (*cIter).cName = (*cIterTemp).cName;
        (*cIter).cValue = (*cIterTemp).cValue;

        (*cIterTemp).cName = cObject.cName;
        (*cIterTemp).cValue = cObject.cValue;
    }
}

//moves the command down when down push button clicked
void MainWindow::on_Down_pushButton_clicked()
{

    int cRowIndex = fIndex.row();

    if(cRowIndex < fRowMax - 1 && cRowIndex > 0){
        QString cData = ui->AddedComands_tabelView->model()->data(fIndex).toString();
        QString cTempData = ui->AddedComands_tabelView->model()->data(ui->AddedComands_tabelView->
                                                                      model()->index(cRowIndex+1, 0)).toString();
        QStandardItem *cItem = new QStandardItem(cData);
        QStandardItem *cItemTemp = new QStandardItem(cTempData);
        model->setItem(cRowIndex+1 , cItem);
        model->index(cRowIndex+1 , 0);
        model->setItem(cRowIndex , cItemTemp);
        model->index(cRowIndex , 0);

        SystemControllerClass::fParameters cObject;
        vector<SystemControllerClass::fParameters>::iterator cIter = fControl->fListOfCommands.begin() + cRowIndex -1;
        vector<SystemControllerClass::fParameters>::iterator cIterTemp = fControl->fListOfCommands.begin() +cRowIndex;

        cObject.cName = (*cIter).cName;
        cObject.cValue = (*cIter).cValue;

        (*cIter).cName = (*cIterTemp).cName;
        (*cIter).cValue = (*cIterTemp).cValue;

        (*cIterTemp).cName = cObject.cName;
        (*cIterTemp).cValue = cObject.cValue;
    }
}

void MainWindow::on_OnOff_button_stateChanged(string pSourceName, int pId, bool pArg)
{
    if(pSourceName == "TTI1"){
        if( pArg){

            fControl->getObject(pSourceName)->setVolt(gui_pointers_low_voltage[0]->v_set->value(), pId);
            fControl->getObject(pSourceName)->setCurr(gui_pointers_low_voltage[0]->i_set->value(), pId);
            fControl->getObject(pSourceName)->onPower(pId);
        }
        else{
            fControl->getObject(pSourceName)->offPower(pId);
        }
    }
    if(pSourceName == "Keithley2410"){

        if( pArg){
            fControl->getObject(pSourceName)->setVolt(gui_pointers_high_voltage[0]->v_set->value(), pId);
            fControl->getObject(pSourceName)->setCurr(gui_pointers_high_voltage[0]->i_set->value(), pId);
            AdditionalThread *cThread = new AdditionalThread("keithley", fControl);
            QThread *cQThread = new QThread();
            connect(cQThread , SIGNAL(started()), cThread, SLOT(onVolt()));
            cThread->moveToThread(cQThread);
            cQThread->start();
            //fControl->getObject(pSourceName)->onPower(pId);
        }
        else{
            AdditionalThread *cThread = new AdditionalThread("keithleyOff", fControl);
            QThread *cQThread = new QThread();
            connect(cQThread , SIGNAL(started()), cThread, SLOT(offVolt()));
            cThread->moveToThread(cQThread);
            cQThread->start();
            //fControl->getObject(pSourceName)->offPower(pId);
        }
    }
    if(pSourceName == "JulaboFP50"){
        if(pArg){
//            fControl->getGenericInstrObj(pSourceName)->SetWorkingTemperature(gui_chiller->setTemperature->value());
//            fControl->getGenericInstrObj(pSourceName)->SetCirculatorOn();
        }
        else{
//            fControl->getGenericInstrObj(pSourceName)->SetCirculatorOff();
        }
    }
}

void MainWindow::receiveOnOff(string pSourceName, bool pArg)
{
    if(pSourceName == "Keithley2410")
        gui_pointers_high_voltage[0]->onoff_button->setChecked(pArg);
    if(pSourceName == "TTI1"){
        gui_pointers_low_voltage[0]->onoff_button->setChecked(pArg);
        gui_pointers_low_voltage[1]->onoff_button->setChecked(pArg);
    }
}

//Set func
void MainWindow::on_V_set_doubleSpinBox_valueChanged(string pSourceName, int pId, double pVolt)
{
    fControl->getObject(pSourceName)->setVolt(pVolt, pId);
    QThread::sleep(0.5);
}

void MainWindow::on_I_set_doubleSpinBox_valueChanged(string pSourceName , int pId, double pCurr)
{

    fControl->getObject(pSourceName)->setCurr(pCurr, pId);
    QThread::sleep(0.5);
}

void MainWindow::updateTTiIWidget(PowerControlClass::fVACvalues* pObject)
{
    gui_pointers_low_voltage[0][0].i_set->setValue(pObject->pISet1);
    gui_pointers_low_voltage[0][0].v_set->setValue(pObject->pVSet1);
    gui_pointers_low_voltage[0][0].i_applied->display(pObject->pIApp1);
    gui_pointers_low_voltage[0][0].v_applied->display(pObject->pVApp1);

    gui_pointers_low_voltage[0][1].i_set->setValue(pObject->pISet2);
    gui_pointers_low_voltage[0][1].v_set->setValue(pObject->pVSet2);
    gui_pointers_low_voltage[0][1].i_applied->display(pObject->pIApp2);
    gui_pointers_low_voltage[0][1].v_applied->display(pObject->pVApp2);
}

void MainWindow::updateKeithleyWidget(PowerControlClass::fVACvalues* pObject)
{
    gui_pointers_high_voltage[0]->i_set->setValue(pObject->pISet1);
    gui_pointers_high_voltage[0]->v_set->setValue(pObject->pVSet1);
    gui_pointers_high_voltage[0]->i_applied->display(pObject->pIApp1);
    gui_pointers_high_voltage[0]->v_applied->display(pObject->pVApp1);
}

void MainWindow::initHard()
{
    // init hard
    fControl->Initialize();

    // start threads
    this->getMeasurments();
    this->getVoltAndCurr();
    this->getVoltAndCurrKeithley();
    this->getChillerStatus();

    connect(fControl, SIGNAL(sendOnOff(string,bool)) , this , SLOT(receiveOnOff(string,bool)));

    this->voltageControlWidget();
}

bool MainWindow::readXmlFile()
{
    fControl = new SystemControllerClass();
    QString cFilter  = "*.xml";
    QString cFileName = QFileDialog::getOpenFileName(this, "Open hardware description file", "./settings", cFilter);

    if (cFileName.isEmpty())
        return false;
    else {
        fControl->ReadXmlFile(cFileName.toStdString());
        fSources = fControl->getSourceNameVec();

        QHBoxLayout *low_layout = new QHBoxLayout;
        QHBoxLayout *high_layout = new QHBoxLayout;
        QHBoxLayout *rasp_layout = new QHBoxLayout;
        QHBoxLayout *chiller_layout = new QHBoxLayout;

        for( auto const &i: fControl->fGenericInstrumentMap){
            cout << i.first << endl;
            cout << i.second << endl;
        }
        for(auto &i: fControl->fGenericInstrumentMap){

            if( dynamic_cast<ControlTTiPower*>(i.second) ){
                cout << "Power supply ttI" << endl;
            }
            if( dynamic_cast<ControlKeithleyPower*>(i.second) ){
                cout << "Keithley" << endl;
            }
            if( dynamic_cast<JulaboFP50*>(i.second)){
                cout << "Julabo" << endl;
            }
            if( dynamic_cast<ConnectionInterfaceClass*>(i.second)){
                cout << " Raspberry " << endl;
            }
        }

        for(auto const& i: fControl->fGenericInstrumentMap){

            if( dynamic_cast<ControlTTiPower*>(i.second) ){

                gui_pointers_low_voltage.push_back(SetVoltageSource(low_layout, i.first, "TTI", nOutputs));

                for(int i = 0 ; i != nOutputs ; i++){
                    connect(gui_pointers_low_voltage[0][i].onoff_button, &QCheckBox::toggled, [this,i](bool pArg)
                    {this->on_OnOff_button_stateChanged("TTI1" , i+1, pArg);});
//                    connect(gui_pointers_low_voltage[0][i].v_set, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this,i](double pVolt)
//                    {this->on_V_set_doubleSpinBox_valueChanged("TTI1" , i+1, pVolt);});
//                    connect(gui_pointers_low_voltage[0][i].i_set, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this,i](double pCurr)
//                    {this->on_I_set_doubleSpinBox_valueChanged("TTI1", i+1, pCurr);});
                 }

            }

            if( dynamic_cast<ControlKeithleyPower*>(i.second) ){

                gui_pointers_high_voltage.push_back(SetVoltageSource(high_layout, i.first, "Keithley2410", 1));
                ui->groupBox_2->setLayout(high_layout);

                 connect(gui_pointers_high_voltage[0]->onoff_button, &QCheckBox::toggled, [this](bool pArg)
                 {this->on_OnOff_button_stateChanged("Keithley2410" , 0, pArg);});
                 connect(gui_pointers_high_voltage[0]->v_set, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this](double pVolt)
                 {this->on_V_set_doubleSpinBox_valueChanged("Keithley2410" , 0, pVolt);});
                 connect(gui_pointers_high_voltage[0]->i_set, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this](double pCurr)
                 {this->on_I_set_doubleSpinBox_valueChanged("Keithley2410", 0, pCurr);});
            }

            if( dynamic_cast<ConnectionInterfaceClass*>(i.second) ){
                gui_raspberry = SetRaspberryOutput(rasp_layout , fControl->fRaspberrySensorsNames , i.first);
            }

            if( dynamic_cast<JulaboWrapper*>(i.second) ){
                gui_chiller = SetChillerOutput(chiller_layout , i.first);

                connect(gui_chiller->onoff_button, &QCheckBox::toggled, [this](bool pArg)
                {this->on_OnOff_button_stateChanged("JulaboFP50" , 0, pArg);});
            }
        }

        //set layout to group box
        ui->groupBox->setLayout(low_layout);
        ui->groupBox_2->setLayout(high_layout);
        ui->groupBox_3->setLayout(rasp_layout);
        ui->groupBox_Chiller->setLayout(chiller_layout);

        //make a list with commands( depends on the devices we have read )
        this->doListOfCommands();
        return true;
    }
}

void MainWindow::on_read_conf_button_clicked()
{
    // read the xml file
    bool cSuccess = this->readXmlFile();

    // enable gui
    if (cSuccess) {
        // enable back
        ui->init_button->setEnabled(true);
        ui->tabWidget->setEnabled(true);
    }
}

void MainWindow::on_init_button_clicked()
{
    // init hard
    this->initHard();

    // disable init
    ui->init_button->setEnabled(false);
    ui->Start_pushButton->setEnabled(true);
}
