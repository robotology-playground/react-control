/* 
 * Copyright: (C) 2015 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Alessandro Roncone <alessandro.roncone@iit.it>
 * website: www.robotcub.org
 * author website: http://alecive.github.io
 * 
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
*/

#ifndef __REACTIPOPT_H__
#define __REACTIPOPT_H__

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>

#include <iCub/iKin/iKinFwd.h>
#include <iCub/iKin/iKinIpOpt.h>

/**
*
* Class for inverting chain's kinematics based on IpOpt lib
*/
class reactIpOpt
{
private:
    // Default constructor: not implemented.
    reactIpOpt();
    // Copy constructor: not implemented.
    reactIpOpt(const reactIpOpt&);
    // Assignment operator: not implemented.
    reactIpOpt &operator=(const reactIpOpt&);    

protected:
    void *App;

    iCub::iKin::iKinChain &chain;
    iCub::iKin::iKinChain chain2ndTask;

    iCub::iKin::iKinLinIneqConstr  noLIC;
    iCub::iKin::iKinLinIneqConstr *pLIC;

    unsigned int ctrlPose;    

    double obj_scaling;
    double x_scaling;
    double g_scaling;
    double lowerBoundInf;
    double upperBoundInf;
    std::string posePriority;

public:
    /**
    * Constructor. 
    * @param c is the Chain object on which the control operates. Do 
    *          not change Chain DOF from this point onwards!!
    * @param _ctrlPose one of the following: 
    *  IKINCTRL_POSE_FULL => complete pose control.
    *  IKINCTRL_POSE_XYZ  => translational part of pose controlled.
    *  IKINCTRL_POSE_ANG  => rotational part of pose controlled. 
    * @param tol exits if 0.5*norm(xd-x)^2<tol.
    * @param max_iter exits if iter>=max_iter (max_iter<0 disables
    *                 this check, IKINCTRL_DISABLED(==-1) by
    *                 default).
    * @param verbose is a integer number which progressively enables 
    *                different levels of warning messages or status
    *                dump. The larger this value the more detailed
    *                is the output (0=>off by default).
    * @param useHessian relies on exact Hessian computation or  
    *                enable Quasi-Newton approximation (true by
    *                default).
    */
    reactIpOpt(iCub::iKin::iKinChain &c, const unsigned int _ctrlPose,
               const double tol, const int max_iter=IKINCTRL_DISABLED,
               const unsigned int verbose=0, bool useHessian=true);

    /**
    * Sets the state of Pose control settings.
    * @param _ctrlPose one of the following: 
    *  IKINCTRL_POSE_FULL => complete pose control.
    *  IKINCTRL_POSE_XYZ  => translational part of pose controlled.
    *  IKINCTRL_POSE_ANG  => rotational part of pose controlled.
    */
    void set_ctrlPose(const unsigned int _ctrlPose);

    /**
    * Returns the state of Pose control settings.
    * @return Pose control settings.
    */
    unsigned int get_ctrlPose() const { return ctrlPose; }

    /**
    * Sets the Pose priority for weighting more either position or 
    * orientation while reaching in full pose. 
    * @param priority can be "position" or "orientation". 
    * @return true/false on success/failure. 
    */
    bool set_posePriority(const std::string &priority);

    /**
    * Returns the Pose priority settings.
    * @return pose priority.
    */
    std::string get_posePriority() const { return posePriority; }

    /**
    * Attach a iKinLinIneqConstr object in order to impose 
    * constraints of the form lB <= C*q <= uB.
    * @param lic is the iKinLinIneqConstr object to attach.
    * @see iKinLinIneqConstr
    */
    void attachLIC(iCub::iKin::iKinLinIneqConstr &lic) { pLIC=&lic; }

    /**
    * Returns a reference to the attached Linear Inequality 
    * Constraints object.
    * @return Linear Inequality Constraints pLIC. 
    * @see iKinLinIneqConstr
    */
    iCub::iKin::iKinLinIneqConstr &getLIC() { return *pLIC; }

    /**
    * Selects the End-Effector of the 2nd task by giving the ordinal
    * number n of last joint pointing at it. 
    * @param n is the ordinal number of last joint pointing at the 
    *          2nd End-Effector.
    */
    void specify2ndTaskEndEff(const unsigned int n);

    /**
    * Retrieves the 2nd task's chain. 
    * @return a reference to the 2nd task's chain.  
    */
    iCub::iKin::iKinChain &get2ndTaskChain();

    /**
    * Sets Maximum Iteration.
    * @param max_iter exits if iter>=max_iter (max_iter<0 
    *                 (IKINCTRL_DISABLED) disables this check).
    */ 
    void setMaxIter(const int max_iter);

    /**
    * Retrieves the current value of Maximum Iteration.
    * @return max_iter. 
    */ 
    int getMaxIter() const;

    /**
    * Sets Tolerance.
    * @param tol exits if norm(xd-x)<tol.
    */
    void setTol(const double tol);

    /**
    * Retrieves Tolerance.
    * @return tolerance.
    */
    double getTol() const;

    /**
    * Sets Verbosity.
    * @param verbose is a integer number which progressively enables 
    *                different levels of warning messages or status
    *                dump. The larger this value the more detailed
    *                is the output.
    */
    void setVerbosity(const unsigned int verbose);

    /**
    * Selects whether to rely on exact Hessian computation or enable
    * Quasi-Newton approximation (Hessian is enabled at start-up by 
    * default). 
    * @param useHessian true if Hessian computation is enabled.
    */
    void setHessianOpt(const bool useHessian);

    /**
    * Enables/disables user scaling factors.
    * @param useUserScaling true if user scaling is enabled. 
    * @param obj_scaling user scaling factor for the objective 
    *                    function.
    * @param x_scaling user scaling factor for variables. 
    * @param g_scaling user scaling factor for constraints. 
    */
    void setUserScaling(const bool useUserScaling, const double _obj_scaling,
                        const double _x_scaling, const double _g_scaling);

    /**
    * Enable\disable derivative test at each call to solve method 
    * (disabled at start-up by default). Useful to check the 
    * derivatives implementation of NLP. 
    * @param enableTest true if derivative test shall be enabled. 
    * @param enable2ndDer true to enable second derivative test as 
    *                     well (false by default).
    */
    void setDerivativeTest(const bool enableTest, const bool enable2ndDer=false);

    /**
    * Returns the lower and upper bounds to represent -inf and +inf.
    * @param lower is a reference to return the lower bound.
    * @param upper is a reference to return the upper bound. 
    */
    void getBoundsInf(double &lower, double &upper);

    /**
    * Sets the lower and upper bounds to represent -inf and +inf.
    * @param lower is the new lower bound. 
    * @param upper is the new upper bound. 
    */
    void setBoundsInf(const double lower, const double upper);

    /**
    * Executes the IpOpt algorithm trying to converge on target. 
    * @param q0 is the vector of initial joint angles values. 
    * @param xd is the End-Effector target Pose to be attained. 
    * @param weight2ndTask weights the second task (disabled if 
    *                      0.0).
    * @param xd_2nd is the second target task traslational Pose to 
    *             be attained (typically a particular elbow xyz
    *             position).
    * @param w_2nd weights each components of the distance vector 
    *              xd_2nd-x_2nd. Hence, the follows holds as second
    *              task: min 1/2*norm2(((xd_i-x_i)*w_i)_i)
    * @param weight3rdTask weights the third task (disabled if 0.0).
    * @param qd_3rd is the third task joint angles target 
    *             positions to be attained.
    * @param w_3rd weights each components of the distance vector 
    *              qd-q. Hence, the follows holds as third task:
    *             min 1/2*norm2(((qd_i-q_i)*w_i)_i)
    * @param exit_code stores the exit code (NULL by default). Test 
    *                  for one of this:
    *                   SUCCESS
    *                   MAXITER_EXCEEDED
    *                   STOP_AT_TINY_STEP
    *                   STOP_AT_ACCEPTABLE_POINT
    *                   LOCAL_INFEASIBILITY
    *                   USER_REQUESTED_STOP
    *                   FEASIBLE_POINT_FOUND
    *                   DIVERGING_ITERATES
    *                   RESTORATION_FAILURE
    *                   ERROR_IN_STEP_COMPUTATION
    *                   INVALID_NUMBER_DETECTED
    *                   TOO_FEW_DEGREES_OF_FREEDOM
    *                   INTERNAL_ERROR
    * @param exhalt checks for an external request to exit (NULL by 
    *               default).
    * @param iterate pointer to a callback object (NULL by default). 
    * @return estimated joint angles.
    */
    virtual yarp::sig::Vector solve(const yarp::sig::Vector &q0, yarp::sig::Vector &xd,
                                    double weight2ndTask, yarp::sig::Vector &xd_2nd, yarp::sig::Vector &w_2nd,
                                    double weight3rdTask, yarp::sig::Vector &qd_3rd, yarp::sig::Vector &w_3rd,
                                    int *exit_code=NULL, bool *exhalt=NULL, iCub::iKin::iKinIterateCallback *iterate=NULL);

    /**
    * Executes the IpOpt algorithm trying to converge on target. 
    * @param q0 is the vector of initial joint angles values. 
    * @param xd is the End-Effector target Pose to be attained. 
    * @param weight2ndTask weights the second task (disabled if 
    *                      0.0).
    * @param xd_2nd is the second target task traslational Pose to 
    *             be attained (typically a particular elbow xyz
    *             position).
    * @param w_2nd weights each components of the distance vector 
    *              xd_2nd-x_2nd. Hence, the follows holds as second
    *              task: min 1/2*norm2(((xd_i-x_i)*w_i)_i)
    * @return estimated joint angles.
    */
    virtual yarp::sig::Vector solve(const yarp::sig::Vector &q0, yarp::sig::Vector &xd,
                                    double weight2ndTask, yarp::sig::Vector &xd_2nd, yarp::sig::Vector &w_2nd);

    /**
    * Executes the IpOpt algorithm trying to converge on target. 
    * @param q0 is the vector of initial joint angles values. 
    * @param xd is the End-Effector target Pose to be attained. 
    * @return estimated joint angles.
    */
    virtual yarp::sig::Vector solve(const yarp::sig::Vector &q0, yarp::sig::Vector &xd);

    /**
    * Default destructor.
    */
    virtual ~reactIpOpt();
};

#endif

