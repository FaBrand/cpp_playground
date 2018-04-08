# Signals and Slots

As a continuation to the observer pattern the signals and slots mechanism decouples the listener completely from
the signal.

The only common point that makes this work is that the signal defines a certain signature the slot must fulfill.

Then a function object to the callable is registered and kept for later invocation with signal values.

# Thanks to
Thanks to [simmesimme](http://simmesimme.github.io/tutorials/2015/09/20/signal-slot) to build an example that is easily understandable and inspired me to 
have a look at this kind of observer implementation.

# Additions
A full fledged implementation of the signal/slot concept within [Boost](https://www.boost.org/doc/libs/1_61_0/doc/html/signals2.html).
Another widespread usage is within [QT](http://doc.qt.io/qt-5/signalsandslots.html)
