#ifndef STATEMACHINE_EVENTS_H
#define STATEMACHINE_EVENTS_H

namespace statemachine
{

class ActivationEvent
{
  public:
    ActivationEvent(bool is_allowed) : is_allowed_(is_allowed)
    {
    }

    inline bool IsAllowed() const
    {
        return is_allowed_;
    }

  private:
    bool is_allowed_{false};
};

struct DeactivationEvent
{
};

struct StateAction
{
};

}  // namespace statemachine

#endif /* STATEMACHINE_EVENTS_H */
