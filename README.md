# Playground rules

This is just a playground project to try random things.
If you see any major errors here, please point them out.
The main reason behind creating this was personal learning and playing around with certain things.

## Building
Simply copy paste

```bash
mkdir _build && cd $_
cmake ../ -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=On
ninja
```

# Disclaimer
This is only meant for personal education.
If used, use at your own risk.

= Get in and out of the bus scenario =
== 1. Approach the bus entrance ==
    The main challegen is to get the position of bus entrance relative to robot
    <br> The main technologies are:

    * VSLAM or Lidar-based SLAM
      - [ ] Attach Slamcore prototype to the front of the robot, repeatedly approach bus under motion capture, evaluate localization metrics of [[file:Cremers2012IROS.pdf]]
      - [ ] Attach Lidar to the top of the robot, repeatedly approach bus under motion capture, evaluate localization metrics of [[file:Cremers2012IROS.pdf]]
    * Localization 
      <br> Since we are dealing with a static environment, map could be built from several runs
      - [ ] Asses separation of mapping and localization and performing mapping offline
    * Visual Servoing - Once the door is in sight of the robot visual tracker that generates velocities of the CoM could take over
      - [ ] Attach camera to the front of the robot, use a visual tracker that generates desired velocities to approach the door, evaluate final position
    * Visual Odometry
        If the starting position is approximately known then approaching the bus could be done via proprioceptive sensing and visual odometry drift correction only
      - [ ] Attach camera to the front of the robot, repeatedly approach bus under motion capture, evaluate localization metrics of [[file:Cremers2012IROS.pdf]]
== 2. Approach the entrance ==
    The main challenge is to position the robot precisly to be able to take steps
    Localizing within a map would introduce unacceptably large errors. Visual Servoing is the most precise approach to that

    - [ ] Attach camera to the front of the robot, use a visual tracker that generates desired velocities to approach the door

== 3. Climb onto the bus ==
    The height of the entrance may be insurmountable for Laikago. We could:
    * Use stairs
    * Use ramp
    * Use WeMo to step on (and being brought to place)

    To do planning of foot locations, the step height needs to be measured and planes fitted
    - [ ] Use adaptation of LAAS's HPP [[file:Lamiraux2016.pdf]] to quadrupeds done in Oxford to climb the bus in simulation [[file:Geisert2019TAROS.pdf]]
    - [ ] Attach camera to the front of the robot to create local maps of the step in front of the robot. Climb the step using HPP.

    The step(s) may done without perfect knowledge of stair height [[file:Wensing2018.pdf]] such that no planning may be necessary.
    However, the robot will still have to be moved close enough to the entrance.
    - [ ] Test if the robot can climb stairs blindly as in [[file:Wensing2018.pdf]]

== 4. Get to the position of inspection ==
    The space between the bus entrance and the inspected location may be varying, however, the dimensions of the bus are known.
    Potentially, moving to the center could be done without exteroceptive sensing
    - [ ] Check the width of the gap to the goal.
        Estimate what maximal position/orientation drift is acceptable to move to center.
        Estimate position error of Slamcore and other localization solutions.
        Estimate the propriceptive drift from experiments with the robot.

== 5. Perform quality check ==
    Scan with a scanner rigidly attached to the robot. To achieve a desired precision the scanner will have to be hold close enough to the inspected surface.
    Considering a manipulator would be too involved in the beginning. The sensor will therefore have to be attached to the robot's body.
    The scanner could rely on external motion trackers positioned outside of the bus.
    The robot needs to be able to identify the inspected location and approach it to a desired distance.
    Alternatively could consist in taking pictures of the interior

    - [ ] Choose scanner based on precision vs distance metric. Attach scanner to robot and perform scanning.

== 6. Get back to entrance ==
    Same as 4

== 7. Descend ==
    Same as 3

== 8. Get back to position ==
    Same as 1
