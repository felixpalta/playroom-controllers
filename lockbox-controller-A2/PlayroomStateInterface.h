#ifndef PLAYROOMSTATEINTERFACE_H
#define PLAYROOMSTATEINTERFACE_H

class PlayroomStateInterface
{
public:
    virtual void init() = 0;
    virtual void game_start_button_pressed() = 0;
    virtual void standby_button_pressed() = 0;
    virtual void cleaning_key_enabled() = 0;
    virtual void cleaning_key_disabled() = 0;
    virtual ~PlayroomStateInterface() {}
};

#endif // PLAYROOMSTATEINTERFACE_H
