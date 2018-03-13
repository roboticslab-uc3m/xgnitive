# DRL Execution

The goal of this repository is to develop DRL inside the CGDA framework.

## Dependencies

This repository uses **rllab** as the framework for the developing DRL algorithms. The instructions to install rllab can be found [here](http://rllab.readthedocs.io/en/latest/user/installation.html).

**Add YARP inside rllab environment**
```bash
cp /usr/local/lib/python3.5/site-packages/yarp.py  ~/anaconda2/envs/rllab3/lib/python3.5/site-packages/
cp /usr/local/lib/python3.5/site-packages/_yarp.so  ~/anaconda2/envs/rllab3/lib/python3.5/site-packages/
```

## Run rllab

In order to start rllab, run the following commands in a terminal:

```
export PATH="/home/raul/anaconda2/bin:$PATH" #Activate anaconda
source activate rllab3 #Initialize rllab
export PYTHONPATH=/home/raul/repos/rllab:$PYTHONPATH #Export rllab to pythonpath

```


## Tutorials: 2D grid world example

First train the policy using DRL

```
python grid_2D_world.py
```

The policy params are then saved in the following paths *data/local/experiment_{timestamp}* of your local rllab repository. To simulate an execution of the task using the policy trained, you can use the following commands:

```
python sim_policy.py home/repos/rllab/data/local/experiment_{timestamp}/params.pkl
```

## Tutorials: 3D grid world example

First train the policy using DRL

```
python grid_3D_world.py
```

The policy params are then saved in the following paths *data/local/experiment_{timestamp}* of your local rllab repository. To simulate an execution of the task using the policy trained, you can use the following commands:

```
python sim_policy.py home/repos/rllab/data/local/experiment_{timestamp}/params.pkl
```

## Example: Teo-Paint openrave discrete actions + rllab 
Run rllab. Then execute the following commands:

```
yarpserver
cd ~/repos/openrave-yarp-plugins/examples/python; python openraveYarpPaintSquares.py #From openrave-yarp-plugins 
cd ~/repos/tools/build/bin; ./controlboardStateToIPosition --remote /teoSim/rightArm 
#Run rllab in a new terminal, then in the same terminal:
cd ~/repos/xgnitive/programs/drl; python teo_paint_discrete_yarp.py;
yarp connect /drl/rightArm/state:o /ControlboardStateToIPosition/state:i
```
