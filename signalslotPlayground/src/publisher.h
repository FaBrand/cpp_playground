#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "topic.h"
#include <signals/signal.h>

using TopicSignal = Signal<topic const&>;

class Publisher
{
public:
    TopicSignal signal;

    void DoStuff(){
        signal.emit(topic{});
    }
};

#endif /* PUBLISHER_H */
