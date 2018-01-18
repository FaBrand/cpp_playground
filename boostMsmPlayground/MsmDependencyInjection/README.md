# Remarks to this implementation
As it seems to me dependency injection is not possible in boost MSM up to now.
I did however solve it in a hacky'ish solution by providing default constructible function pointers which are set to the virtual memberfunction inside the wrapper class.
There are drawbacks to this solution that the statmachines frontent constructor is not typesafe in terms of being able to get the order of the arguments wrong.
See this entry in the [Boost mailing list](http://boost.2283326.n4.nabble.com/msm-State-Machine-and-Dependency-Injection-tt4666017.html#none)
