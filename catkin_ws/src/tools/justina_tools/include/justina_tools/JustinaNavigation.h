#pragma once
#include <iostream>
#include <cmath>
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/GetMap.h"
#include "nav_msgs/Path.h"
#include "navig_msgs/CalculatePath.h"
#include "point_cloud_manager/GetRgbd.h"
#include "tf/transform_listener.h"

class JustinaNavigation
{
private:
    static bool is_node_set;
    //Subscriber for checking goal-pose-reached signal
    static ros::Subscriber subGoalReached;
    static ros::Subscriber subGlobalGoalReached;    
    static ros::Subscriber subStopRobot;
    //Publishers and subscribers for operating the simple_move node
    static ros::Publisher pubSimpleMoveDist;
    static ros::Publisher pubSimpleMoveDistAngle;
    static ros::Publisher pubSimpleMoveLateral;
    static ros::Publisher pubSimpleMoveGoalPath;
    static ros::Publisher pubSimpleMoveGoalPose;
    static ros::Publisher pubSimpleMoveGoalRelPose;
    //Services for path calculator
    static ros::ServiceClient cltGetMap;
    static ros::ServiceClient cltGetPointCloud;
    static ros::ServiceClient cltCalculatePath; //Path calculation using only the occupancy grid
    //Publishers and subscribers for localization
    static ros::Subscriber subCurrentRobotPose;
    static tf::TransformListener* tf_listener;
    //Subscribers for obstacle avoidance
    static ros::Publisher pubObsAvoidEnable;
    static ros::Subscriber subObsInFront;
    static ros::Subscriber subCollisionRisk;

    //Variables for navigation
    static float currentRobotX;
    static float currentRobotY;
    static float currentRobotTheta;
    static nav_msgs::Path lastCalcPath;
    static bool _isGoalReached;
    static bool _isGlobalGoalReached;
    static bool _stopReceived;
    static bool _obstacleInFront;
    static bool _collisionRisk;

public:
    //
    //The startSomething functions, only publish the goal pose or path and return inmediately after starting movement
    //The others, block until a goal-reached signal is received
    //
    
    static bool setNodeHandle(ros::NodeHandle* nh);
    static bool isGoalReached();
    static bool isGlobalGoalReached();
    static bool waitForGoalReached(int timeOut_ms);
    static bool waitForGlobalGoalReached(int timeOut_ms);
    static void getRobotPose(float& currentX, float& currentY, float& currentTheta);
    static void getRobotPoseFromOdom(float& currentX, float& currentY, float& currentTheta);
    //Methods for obstacle avoidance
    static bool obstacleInFront();
    static bool collisionRisk();
    static void enableObstacleDetection(bool enable);
    //These methods use the simple_move node
    static void startMoveDist(float distance);
    static void startMoveDistAngle(float distance, float angle);
    static void startMovePath(nav_msgs::Path& path);
    static void startMoveLateral(float distance);
    static void startGoToPose(float x, float y, float angle);
    static void startGoToRelPose(float relX, float relY, float relTheta);
    static bool moveDist(float distance, int timeOut_ms);
    static bool moveDistAngle(float distance, float angle, int timeOut_ms);
    static bool movePath(nav_msgs::Path& path, int timeOut_ms);
    static bool moveLateral(float distance, int timeOut_ms);
    static bool goToPose(float x, float y, float angle, int timeOut_ms);
    static bool goToRelPose(float relX, float relY, float relTheta, int timeOut_ms);

    //These methods use the mvn_pln node.
    static void startGetClose(float x, float y);
    static void startGetClose(float x, float y, float angle);
    static void startGetClose(std::string location);
    static bool getClose(float x, float y, int timeOut_ms);
    static bool getClose(float x, float y, float angle, int timeOut_ms);
    static bool getClose(std::string location, int timeOut_ms);

    //This functions call services, so, they block until a response is received. They use the path_calculator node
    //This function uses the path calculator node, which only calculates a path and nothing more.
    //Instead, mvn_pln calcs path by taking into account kinect, laser and others.
    static bool getOccupancyGrid(nav_msgs::OccupancyGrid& map);
    static bool calcPathAStar(float startX, float startY, float goalX, float goalY, nav_msgs::Path& result);
    
    //Callbacks for subscribers
    static void callbackCurrentRobotPose(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg);
    static void callbackRobotStop(const std_msgs::Empty::ConstPtr& msg);
    static void callbackGoalReached(const std_msgs::Bool::ConstPtr& msg);
    static void callbackGlobalGoalReached(const std_msgs::Bool::ConstPtr& msg);

    //Callbacks for obstacle avoidance
    static void callbackObstacleInFront(const std_msgs::Bool::ConstPtr& msg);
    static void callbackCollisionRisk(const std_msgs::Bool::ConstPtr& msg);
};
