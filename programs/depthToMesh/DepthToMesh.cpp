// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "DepthToMesh.hpp"

/************************************************************************/

void DepthToMesh::cloud_cb_ (const pcl::PointCloud<pcl::PointXYZ>::ConstPtr &cloud) {
    //cout << "Begin sensorCallback." << endl;
    gettimeofday(&start, NULL);

    //cout << "Begin removeNaNFromPointCloud, size: " << cloud->points.size() << "." << endl;
    std::vector<int> indices;
    pcl::PointCloud<pcl::PointXYZ>::Ptr modCloud1 (new pcl::PointCloud<pcl::PointXYZ>);
    //-- http://docs.pointclouds.org/1.7.0/group__filters.html#gac463283a9e9c18a66d3d29b28a575064
    //-- removeNaNFromPointCloud can be called with cloud_in == cloud_out
    //-- But can't here because we have a ConstPtr
    pcl::removeNaNFromPointCloud(*cloud, *modCloud1, indices);

    //cout << "Begin VoxelGrid, size: " << modCloud1->points.size() << "." << endl;
    pcl::PointCloud<pcl::PointXYZ>::Ptr modCloud2 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::VoxelGrid<pcl::PointXYZ> vg;
    vg.setInputCloud (modCloud1);
    //vg.setLeafSize (0.01f, 0.01f, 0.01f);  // mesh
    vg.setLeafSize (0.05f, 0.05f, 0.05f);  // boxes
    vg.filter (*modCloud2);

    //cout << "Begin normal estimation + Poisson, size: " << modCloud2->points.size() << "." << endl;
    //-- thanks: http://www.pcl-users.org/Access-pcl-PolygonMesh-triangles-data-td4025718.html
    //pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
    pcl::NormalEstimationOMP<pcl::PointXYZ, pcl::Normal> n;  // 
    pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud(modCloud2);
    n.setInputCloud(modCloud2);
    n.setSearchMethod(tree);
    n.setKSearch (40);  // 20
    //n.setKSearch (modCloud2->size());
    //n.setNumberOfThreads(6);  // 1:7.8. 7:7.5. 20:7.4.
    n.compute (*normals);
    // Concatenate the XYZ and normal fields (cloud_with_normals = cloud + normals)
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields (*modCloud2, *normals, *cloud_with_normals);
    // Create search tree
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud (cloud_with_normals);
    pcl::Poisson<pcl::PointNormal> psr;  // Only pcl >= 1.6
    pcl::PolygonMesh triangles;
    psr.setInputCloud(cloud_with_normals);
    psr.setSearchMethod(tree2);
    psr.reconstruct(triangles);

    pcl::io::saveOBJFile("keep.obj",triangles,6);

    //-- Output time
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    cout << "---------------------> Took: " << mtime << " milliseconds." << endl;
}

/************************************************************************/

void DepthToMesh::run () {

    pcl::Grabber* interface = new pcl::OpenNIGrabber();

    boost::function<void (const pcl::PointCloud<pcl::PointXYZ>::ConstPtr&)> f =
        boost::bind (&DepthToMesh::cloud_cb_, this, _1);

    interface->registerCallback (f);

    interface->start ();

    boost::this_thread::sleep (boost::posix_time::seconds (10));
    printf("bye!\n");

    interface->stop ();
}

/************************************************************************/

