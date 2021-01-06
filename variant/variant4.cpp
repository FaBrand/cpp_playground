/*Writing fsm*/

#include <iostream>
#include <optional>
#include <variant>
#include <vector>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

using WalkData = std::string;
using TrotData = std::string;
struct Data {
    WalkData walk = "Walking";
    TrotData trot = "Trotting";
};
struct Trot {
    void update(const TrotData& trot) { std::cout << trot << std::endl; }
};
struct StaticWalk {
    void update(const WalkData& walk) { std::cout << walk << std::endl; }
};

using State = std::variant<Trot, StaticWalk>;

struct EventTrot {
    std::string msg = "trot";
};
struct EventWalk {
    std::string msg = "walk";
};
using Event = std::variant<EventTrot, EventWalk>;

struct Transitions {
    auto operator()(StaticWalk&, const EventTrot& e) -> Trot {
        std::cout << e.msg << std::endl;
        return Trot{};
    }
    auto operator()(Trot&, const EventWalk& e) -> StaticWalk {
        std::cout << e.msg << std::endl;
        return StaticWalk{};
    }
    // template <typename State, typename Event>
    // Trot operator()(State&, const Event&) const {
    //     return Trot{};
    // }
};

template <typename StateVariant, typename EventVariant, typename Transitions>
class Fsm {
    StateVariant state_;

   public:
    void dispatch(const EventVariant& event) {
        state_ = std::visit(Transitions{}, state_, event);
        // if (new_state) {
        // state_ = new_state;
        // }
    }
};

using walking_fsm = Fsm<State, Event, Transitions>;

template <typename Fsm, typename... Events>
void dispatch(Fsm& fsm, Events&&... events) {
    (fsm.dispatch(std::forward<Events>(events)), ...);
}

int main() {
    walking_fsm fsm;
    // fsm.dispatch(EventTrot{});

    State state = StaticWalk{};
    Event event = EventWalk{};
    std::visit(Transitions{}, state, event);
    return 0;
}
