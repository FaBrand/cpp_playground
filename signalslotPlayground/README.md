# Signals and Slots

As a continuation to the observer pattern the signals and slots mechanism decouples the listener completely from
the signal.

The only common point that makes this work is that the signal defines a certain signature the slot must fulfill.

Then a function object to the callable is registered and kept for later invocation with signal values.

