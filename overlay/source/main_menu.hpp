#include <tesla.hpp>
#include <fancontrol.h>
#include "utils.hpp"

class MainMenu : public tsl::Gui 
{
private:
    TemperaturePoint* _fanCurveTable;
    bool _tableIsChanged;

    tsl::elm::ToggleListItem* _enabledBtn;

    tsl::elm::ListItem* _p0Label;
    tsl::elm::ListItem* _p1Label;

public:
    MainMenu();

    virtual tsl::elm::Element* createUI() override;

    virtual void update() override;
};