#ifndef PLAYROOMSTATEMACHINEINTERFACE_H
#define PLAYROOMSTATEMACHINEINTERFACE_H

class PlayroomStateMachineInterface
{
public:

typedef enum
{
    STATE_START,
    STATE_CLEANING,
    STATE_CLEANING_DONE,
    STATE_STANDBY,
    STATE_GAME,
} State;

    virtual void game_start_button_pressed_handler() = 0;
    virtual void standby_button_pressed_handler() = 0;
    virtual void cleaning_key_enabled_handler() = 0;
    virtual void cleaning_key_disabled_handler() = 0;

    virtual void set_state(State state) = 0;

    virtual ~PlayroomStateMachineInterface() {}
};

#endif // PLAYROOMSTATEMACHINEINTERFACE_H
