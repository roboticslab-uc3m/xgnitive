#ifndef __DTW_RECOGNITION_HPP__
#define __DTW_RECOGNITION_HPP__

#include <vector>

//This class receive two trajectories vector defined with vectors and compare them with DTW. It returns a "discrepancy value"
class DtwRecognition
{

public:

    //Constructor
    DtwRecognition();

    /*
     * With this fuction we just define the generalized trajectory, which
     * at the end is the one we want to achieve
     */
    bool setGeneralized(std::vector<double> &v1);

    /*
     * Compare the vector "v2" with the generalized trajectory
     */
    bool compare(std::vector<double> &v2, double discrepancy);

    ~DtwRecognition();

private:
    std::vector<double> generalizedT;

};

#endif // __DTW_RECOGNITION_HPP__

