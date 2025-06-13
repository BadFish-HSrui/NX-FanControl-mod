#include "main_menu.hpp"
#include "select_menu.hpp"

MainMenu::MainMenu()
{
    ReadConfigFile(&this->_fanCurveTable);

    this->_p0Label = new tsl::elm::ListItem("起转温度: " + std::to_string(this->_fanCurveTable->temperature_c) + "℃");
    this->_p1Label = new tsl::elm::ListItem("满转温度: " + std::to_string((this->_fanCurveTable + 1)->temperature_c) + "℃");

    if (IsRunning() != 0)
    {
        this->_enabledBtn = new tsl::elm::ToggleListItem("调速开关", true);
    }
    else
    {
        this->_enabledBtn = new tsl::elm::ToggleListItem("调速开关", false);
    }
}

tsl::elm::Element* MainMenu::createUI()
{
    this->_tableIsChanged = false;

    auto frame = new tsl::elm::OverlayFrame("NX-FanControl-mod", "v1.0.3-mod by 葡萄糖酸菜鱼");

    auto list = new tsl::elm::List();

    this->_enabledBtn->setStateChangedListener([this](bool state)
    {
	    if (state)
        {
            CreateB2F();
            const NcmProgramLocation programLocation{
                .program_id = SysFanControlID,
                .storageID = NcmStorageId_None,
            };
            u64 pid = 0;
            pmshellLaunchProgram(0, &programLocation, &pid);
            return true;
		}
        else
        {
            RemoveB2F();
            pmshellTerminateProgram(SysFanControlID);
            return true;
        }
	    return false;
    });
    list->addItem(this->_enabledBtn);

    list->addItem(new tsl::elm::CategoryHeader("风扇调速", true));
    this->_p0Label->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A) 
        {
			tsl::changeTo<SelectMenu>(0, this->_fanCurveTable, &this->_tableIsChanged);
			return true;
		}
		return false; 
    });
    list->addItem(this->_p0Label);

    this->_p1Label->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A) 
        {
			tsl::changeTo<SelectMenu>(1, this->_fanCurveTable, &this->_tableIsChanged);
			return true;
		}
		return false;
    });
    list->addItem(this->_p1Label);

    frame->setContent(list);

    return frame;
}

void MainMenu::update()
{
    if(this->_tableIsChanged)
    {
        this->_p0Label->setText("起转温度: " + std::to_string(this->_fanCurveTable->temperature_c) + "℃");
        this->_p1Label->setText("满转温度: " + std::to_string((this->_fanCurveTable + 1)->temperature_c) + "℃");
        
        this->_tableIsChanged = false;
    }
}