

## Install Mavros

https://ardupilot.org/dev/docs/ros-install.html

or

https://docs.px4.io/main/en/ros/mavros_installation.html

## Mavlink 

No need for install. Just a reference.

https://mavlink.io/en/getting_started/use_libraries.html

## Mavros PX4 1.13 offborad control example website

https://docs.px4.io/main/en/ros/mavros_offboard_cpp.html



## Four terminal

1. make px4_sitl gazebo
2. roslaunch mavros px4.launch fcu_url:="udp://:14540@127.0.0.1:14557"
3. ./QGroundControl.AppImage 
4. rosrun px4_offboard px4_offboard 

## if no response

then change mod to offborad mode


 
# 08 22

## rostopic 看offboard程序给什么节点发消息

rostopic list

rostopic echo /mavros/setpoint_position/local

# PX4_MAVROS_OFFBOARD
