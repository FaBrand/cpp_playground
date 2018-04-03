# Some remarks that did are still itching

In this variant the Subject is specialized for the concrete observer.
The use of CRTP in this case forces the subject to know of the concrete observer ate compiletime.
This is bad since the subject is not able to be used by objects of different types anymore.

In some way it would be nice to have crtp and also hide the concrete observer behind the observer base class.
Right now, publicly inheriting from `std::shared_from_this` requires the complete type to be known when creating the
actual shared pointer to `this`.

To-be-investigated: Maybe type erasure could be used to solve this issue.

