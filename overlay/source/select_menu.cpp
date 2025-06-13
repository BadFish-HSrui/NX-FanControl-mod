#include "select_menu.hpp"
#include "utils.hpp"

SelectMenu::SelectMenu(int i, TemperaturePoint* fanCurveTable, bool* tableIsChanged) 
{
    this->_i = i;
    this->_fanCurveTable = fanCurveTable;
    this->_tableIsChanged = tableIsChanged;

    this->_saveBtn = new tsl::elm::ListItem("Save");
    
    if (this->_i == 0) {
        this->_tempLabel = new tsl::elm::CategoryHeader("起转温度: " + std::to_string((this->_fanCurveTable + this->_i)->temperature_c) + "℃", true);
    } else {
        this->_tempLabel = new tsl::elm::CategoryHeader("满转温度: " + std::to_string((this->_fanCurveTable + this->_i)->temperature_c) + "℃", true);
    }
}

tsl::elm::Element* SelectMenu::createUI(){

    auto frame = new tsl::elm::OverlayFrame("NX-FanControl-mod", "v1.0.3-mod by 葡萄糖酸菜鱼");

    auto list = new tsl::elm::List();

    list->addItem(this->_tempLabel);
    auto stepTemp = new tsl::elm::StepTrackBar("温度", 21);
    stepTemp->setValueChangedListener([this](u8 value)
    {
        if (this->_i == 0) {
            this->_tempLabel->setText("起转温度: " + std::to_string(value * 5) + "℃");
        } else {
            this->_tempLabel->setText("满转温度: " + std::to_string(value * 5) + "℃");
        }
        (this->_fanCurveTable + this->_i)->temperature_c = value * 5;
        this->_saveBtn->setText("Save");
    });
    stepTemp->setProgress(((this->_fanCurveTable + this->_i)->temperature_c) / 5);
    list->addItem(stepTemp);

    

    this->_saveBtn->setClickListener([this](uint64_t keys) 
    {
	    if (keys & HidNpadButton_A) 
        {
            if (this->_i == 0) {
                (this->_fanCurveTable + this->_i)->fanLevel_f = 0.0f;
            } else {
                (this->_fanCurveTable + this->_i)->fanLevel_f = 1.0f;
            }
            
		    WriteConfigFile(this->_fanCurveTable);

            if(IsRunning() != 0)
            {
                pmshellTerminateProgram(SysFanControlID);
                const NcmProgramLocation programLocation
                {
                    .program_id = SysFanControlID,
                    .storageID = NcmStorageId_None,
                };
                u64 pid = 0;
                pmshellLaunchProgram(0, &programLocation, &pid);
            }
                
            this->_saveBtn->setText("Saved!");
            *this->_tableIsChanged = true;
		    return true;
		}
		
        return false;
		
    });

    list->addItem(this->_saveBtn);

    frame->setContent(list);

    return frame;
}