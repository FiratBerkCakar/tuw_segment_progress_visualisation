#include <tuw_segment_progress_visualisation/segment_progress_visualisation.h>


namespace tuw_segment_progress_visualisation{

    SegmentProgressVisualisation::SegmentProgressVisualisation(ros::NodeHandle &n): n_(n),path_size_(0),waypoints_ptr_(
            nullptr) {

        sub_path = n_.subscribe("r0/global_planner/planner/plan",1,&SegmentProgressVisualisation::callback_path_,this);
        sub_waypoints_index =  n_.subscribe("r0/segment_progression_index",10,&SegmentProgressVisualisation::callback_path_index_,this);
        pub_segment_progress = n_.advertise<visualization_msgs::Marker>("visualization_marker", 10);

        line_strip.type = visualization_msgs::Marker::LINE_STRIP;
        line_strip.header.frame_id = "map";
        line_strip.scale.x = 0.15;
        line_strip.action = visualization_msgs::Marker::ADD;
        ros::spin();



    }

    void SegmentProgressVisualisation::configure_line_strip(visualization_msgs::Marker & marker, uint32_t index) {

        marker.points.clear();
        marker.colors.clear();
        geometry_msgs::Point p;
        marker.colors.resize(waypoints_ptr_->poses.size());
        
        ROS_INFO("NUMBER OF POINTS IN PATH : %lu",waypoints_ptr_->poses.size());
        for(size_t i=0; i< waypoints_ptr_->poses.size(); i++){
            p.x = waypoints_ptr_->poses[i].pose.position.x;
            p.y = waypoints_ptr_->poses[i].pose.position.y;
            //p.z = waypoints_ptr_->poses[i].pose.position.z;

            marker.points.push_back(p);
            if(i < index){
                marker.colors[i].a = 1.0;
                marker.colors[i].b = 1.0;

            } else{
                marker.colors[i].a = 1.0;
                marker.colors[i].r = 1.0;


            }



        }
        marker.header.stamp = ros::Time::now();
        //marker.color.r = 1.0;
        //marker.color.a = 1.0;
        marker.id = 0;




    }


    void SegmentProgressVisualisation::callback_path_(const nav_msgs::Path::ConstPtr &x ) {
        ROS_INFO("PATH CALLBACK RECEIVED...");
        waypoints_ptr_= x;



    }
    void SegmentProgressVisualisation::callback_path_index_(const std_msgs::Int32 idx) {
        ROS_INFO("INDEX_CALLBACK RECEIVED");
        ROS_INFO("INDEX IS %d",idx.data);
        if(waypoints_ptr_ != nullptr){
        configure_line_strip(line_strip,idx.data);
        pub_segment_progress.publish(line_strip);

        }



    }



}

int main( int argc, char** argv )
{
    ros::init(argc, argv, "Segment_progress_viz");
    ros::NodeHandle n;
    tuw_segment_progress_visualisation::SegmentProgressVisualisation x(n);



}